#include "EngineInclude.h"
#include "Logger.h"

namespace Disorder
{

	void FbxSceneImporter::Init()
	{
		 //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
		_sdkManager = FbxManager::Create();
		BOOST_ASSERT(_sdkManager);

		if( !_sdkManager )
		{
			GLogger->Error("Error: Unable to create FBX Manager!\n");
			return;
		}
 
		//Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* ios = FbxIOSettings::Create(_sdkManager, IOSROOT);
		BOOST_ASSERT(ios);
		_sdkManager->SetIOSettings(ios);

	}

	void FbxSceneImporter::Exit()
	{
		if( _sdkManager ) 
			_sdkManager->Destroy();
	}
 
	

	LevelPtr FbxSceneImporter::Import(std::string const& fileName)
	{
		std::string fbxFile = GConfig->sResourceFBXPath + fileName;
		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor,  lSDKMinor,  lSDKRevision;	 
		bool lStatus = false;
		// Get the file version number generate by the FBX SDK.
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		//Create an FBX scene. This object holds most objects imported/exported from/to files.
		FbxScene* lscene = FbxScene::Create(_sdkManager, "Disorder Scene");
		
		BOOST_ASSERT(lscene);

		if( !lscene )
		{
			GLogger->Error("Error: Unable to create FBX scene!\n");
		    return NULL;
		}

		// Create an importer.
		FbxImporter* lImporter = FbxImporter::Create(_sdkManager,"");

		// Initialize the importer by providing a filename.
		const bool lImportStatus = lImporter->Initialize(fbxFile.c_str(), -1, _sdkManager->GetIOSettings());
		
		if( !lImportStatus )
		{
			GLogger->Error("Call to FbxImporter::Initialize() failed.\n");
			GLogger->Error(lImporter->GetLastErrorString());

			if (lImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedYet ||
				lImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedAnymore)
			{
				std::stringstream stream;
				stream << "FBX file format version for this FBX SDK is " << lSDKMajor <<"," << lSDKMinor<<","<< lSDKRevision;
				GLogger->Error(stream.str());
			}

			return NULL;
		}
 
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

		if (lImporter->IsFBX())
		{
			std::stringstream stream;
			stream << "FBX file format version for file "<< fileName<<" is " << lFileMajor<<","<< lFileMinor<<","<< lFileRevision;
			GLogger->Info(stream.str());
 
			// Set the import states. By default, the import states are always set to 
			// true. The code below shows how to change these states.
			/*_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL,        true);
			_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE,         true);
			_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK,            true);
			_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE,           true);
			_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO,            true);
			_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION,       true);
			_sdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);*/
		}

		// Import the scene.
		lStatus = lImporter->Import(lscene);

		if(lStatus == false && lImporter->GetLastErrorID() == FbxIOBase::ePasswordError)
		{
			GLogger->Error("FBX file need a password!!!!");
			return NULL;
		/*	FBXSDK_printf("Please enter password: ");

			lPassword[0] = '\0';

			FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
			FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

			IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
			IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

			lStatus = lImporter->Import(pScene);

			if(lStatus == false && lImporter->GetLastErrorID() == FbxIOBase::ePasswordError)
			{
				FBXSDK_printf("\nPassword is wrong, import aborted.\n");
			}*/
		}

		LevelPtr level = boost::make_shared<Level>();

		ProcessGlobalSetting(lscene,level);
 
		ProcessHierarchy(lscene,level);



		// Destroy the importer.
		lImporter->Destroy();
		return level;
	}

	void FbxSceneImporter::ProcessGlobalSetting(FbxScene* lscene,LevelPtr const& level)
	{
		FbxGlobalSettings &rGlobalSetting =  lscene->GetGlobalSettings();
		FbxColor ambient = rGlobalSetting.GetAmbientColor();
		level->SetAmbientColor(Vector4(ambient.mRed,ambient.mGreen,ambient.mBlue,ambient.mAlpha));

	}

	 

    void FbxSceneImporter::ProcessHierarchy(FbxScene* lscene,LevelPtr const& level)
	{
		FbxNode* lNode = lscene->GetRootNode();

		if(lNode)
		{
			for(int i = 0; i < lNode->GetChildCount(); i++)
			{
				FbxNode* pNode = lNode->GetChild(i);
				GameObjectPtr gameObject = ProcessTranform(pNode);
				ProcessContent(pNode,gameObject);
				level->AddGameObject(gameObject);

				int childnum = pNode->GetChildCount();
				for(int ic = 0;ic < childnum;ic++ )
				{
					FbxNode* pChildNode = pNode->GetChild(ic);
					ProcessHierarchy(pChildNode,gameObject);
				}
			}
		}
	}

	void FbxSceneImporter::ProcessHierarchy(FbxNode* pNode,GameObjectPtr const& gameObject)
	{
		GameObjectPtr childObject = ProcessTranform(pNode);
		ProcessContent(pNode,childObject);
		gameObject->AddChild(childObject);
		int childnum = pNode->GetChildCount();
		for(int ic = 0;ic < childnum;ic++ )
		{
			FbxNode* pChildNode = pNode->GetChild(ic);
			ProcessHierarchy(pChildNode,gameObject);
		}
				 
	}

	void FbxSceneImporter::ProcessContent(FbxNode* pNode,GameObjectPtr const& gameObject)
	{
		if(pNode->GetNodeAttribute() == NULL)
			return;

        FbxNodeAttribute::EType lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        case FbxNodeAttribute::eMarker:  
            //DisplayMarker(pNode);
			GLogger->Info("Node has marker info!");
            break;

        case FbxNodeAttribute::eSkeleton:  
            //DisplaySkeleton(pNode);
			GLogger->Info("Node has Skeleton info!");
            break;

        case FbxNodeAttribute::eMesh:      
			ProcessMesh(pNode,gameObject);
            break;

        case FbxNodeAttribute::eNurbs:      
            //DisplayNurb(pNode);
			GLogger->Info("Node has Nurbs info!");
            break;

        case FbxNodeAttribute::ePatch:     
            //DisplayPatch(pNode);
			GLogger->Info("Node has Patch info!");
            break;

        case FbxNodeAttribute::eCamera:    
            //DisplayCamera(pNode);
			GLogger->Info("Node has Camera info!");
            break;

        case FbxNodeAttribute::eLight:     
			ProcessLight(pNode,gameObject);
            break;

        case FbxNodeAttribute::eLODGroup:
            //DisplayLodGroup(pNode);
			GLogger->Info("Node has LodGroup info!");
            break;
        }   


    
	}

	void FbxSceneImporter::ProcessGeometry(FbxMesh *pMesh,GeometryPtr const& geometry)
	{
		int lPolygonCount = pMesh->GetPolygonCount();
        FbxVector4* lControlPoints = pMesh->GetControlPoints(); 
		int vertexCount = pMesh->GetControlPointsCount();

		// Vertex read
		for(int i=0;i<vertexCount;i++)
		{
			geometry->ControllPositions.push_back(Vector3(lControlPoints[i][0],lControlPoints[i][1],lControlPoints[i][2]));
		}
 
		unsigned int vertexId = 0;
		for (int i = 0; i < lPolygonCount; i++)
		{
			// skip element polygon group layer element ...		 
			int lPolygonSize = pMesh->GetPolygonSize(i);
			if( lPolygonSize != 3 )
			{
				GLogger->Error("Not support polygon size not equal to 3!");
				return;
			}

			for (int j = 0; j < lPolygonSize; j++)
			{
				int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			 
				geometry->Positions.push_back(lControlPointIndex);
				geometry->Indices.push_back(vertexId);
				// vertex color.
				for (int l = 0; l < pMesh->GetElementVertexColorCount(); l++)
				{
					FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor( l);
					 
					switch (leVtxc->GetMappingMode())
					{
					case FbxGeometryElement::eByControlPoint:
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								FbxColor fColor = leVtxc->GetDirectArray().GetAt(lControlPointIndex);
								Vector4 color(fColor.mRed,fColor.mGreen,fColor.mBlue,fColor.mAlpha);
								geometry->Colors.push_back(color);
								break;
							}
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
								FbxColor fColor = leVtxc->GetDirectArray().GetAt(id);
								Vector4 color(fColor.mRed,fColor.mGreen,fColor.mBlue,fColor.mAlpha);
							    geometry->Colors.push_back(color);
								break;
							}
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
						{
							switch (leVtxc->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								{
									FbxColor fColor = leVtxc->GetDirectArray().GetAt(vertexId);
									Vector4 color(fColor.mRed,fColor.mGreen,fColor.mBlue,fColor.mAlpha);
								    geometry->Colors.push_back(color);
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leVtxc->GetIndexArray().GetAt(vertexId);
									FbxColor fColor = leVtxc->GetDirectArray().GetAt(id);
									Vector4 color(fColor.mRed,fColor.mGreen,fColor.mBlue,fColor.mAlpha);
							        geometry->Colors.push_back(color);
								}
								break;
							default:
								break; // other reference modes not shown here!
							}
						}
						break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						GLogger->Error("Fbx vertex color not supported mapping mode!");
						break;
					}
				}

				// uv 
				for (int l = 0; l < pMesh->GetElementUVCount(); ++l)
				{
					FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);
				 
					switch (leUV->GetMappingMode())
					{
					case FbxGeometryElement::eByControlPoint:
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								FbxVector2 uv = leUV->GetDirectArray().GetAt(lControlPointIndex);
								Vector2 vuv(uv[0],uv[1]);
								geometry->Texcoords.push_back(vuv);
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
								FbxVector2 uv = leUV->GetDirectArray().GetAt(id);
								Vector2 vuv(uv[0],uv[1]);
								geometry->Texcoords.push_back(vuv);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
						{
							int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
							switch (leUV->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
								{
									FbxVector2 uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
									Vector2 vuv(uv[0],uv[1]);
								    geometry->Texcoords.push_back(vuv);
								}
								break;
							default:
								break; // other reference modes not shown here!
							}
						}
						break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}

				// normal
				for(int l = 0; l < pMesh->GetElementNormalCount(); ++l)
				{
					FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
					 
					if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								FbxVector4 normal = leNormal->GetDirectArray().GetAt(vertexId);
								normal.Normalize();
								Vector3 vNormal(normal[0],normal[1],normal[2]);
								geometry->Normals.push_back(vNormal);
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leNormal->GetIndexArray().GetAt(vertexId);
								FbxVector4 normal = leNormal->GetDirectArray().GetAt(id);
								normal.Normalize();
								Vector3 vNormal(normal[0],normal[1],normal[2]);
								geometry->Normals.push_back(vNormal);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
				}

				// tangent
				for(int l = 0; l < pMesh->GetElementTangentCount(); ++l)
				{
					FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l);
				 
					if(leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leTangent->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								FbxVector4 tangent = leTangent->GetDirectArray().GetAt(vertexId);
								tangent.Normalize();
								Vector3 vTangent(tangent[0],tangent[1],tangent[2]);
								geometry->Tangents.push_back(vTangent);
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leTangent->GetIndexArray().GetAt(vertexId);
								FbxVector4 tangent = leTangent->GetDirectArray().GetAt(id);
								tangent.Normalize();
								Vector3 vTangent(tangent[0],tangent[1],tangent[2]);
								geometry->Tangents.push_back(vTangent);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}

				//binormal
				for(int l = 0; l < pMesh->GetElementBinormalCount(); ++l)
				{
					FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal( l);
					 
					if(leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leBinormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{
								FbxVector4 binormal = leBinormal->GetDirectArray().GetAt(vertexId);
								binormal.Normalize();
								Vector3 vBinormal(binormal[0],binormal[1],binormal[2]);
								geometry->Binormals.push_back(vBinormal);
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leBinormal->GetIndexArray().GetAt(vertexId);
								FbxVector4 binormal = leBinormal->GetDirectArray().GetAt(id);
								binormal.Normalize();
								Vector3 vBinormal(binormal[0],binormal[1],binormal[2]);
								geometry->Binormals.push_back(vBinormal);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
				}
				vertexId++;
			} // for polygonSize
		} // for polygonCount
 
		// Should check if color uv normal equal to index

		if( geometry->Colors.size() > 0 && geometry->Indices.size() != geometry->Colors.size() )
		{
			GLogger->Error("Mesh vertex color size not equal to index size!");
			BOOST_ASSERT(0);
		}

		if( geometry->Texcoords.size() > 0 && geometry->Indices.size() != geometry->Texcoords.size() )
		{
			GLogger->Error("Mesh vertex uv size not equal to index size!");
			BOOST_ASSERT(0);
		}

		if( geometry->Normals.size() > 0 && geometry->Indices.size() != geometry->Normals.size() )
		{
			GLogger->Error("Mesh vertex normals size not equal to index size!");
			BOOST_ASSERT(0);
		}

		if( geometry->Binormals.size() > 0 && geometry->Indices.size() != geometry->Binormals.size() )
		{
			GLogger->Error("Mesh vertex binormals size not equal to index size!");
			BOOST_ASSERT(0);
		}

		if( geometry->Tangents.size() > 0 && geometry->Indices.size() != geometry->Tangents.size() )
		{
			GLogger->Error("Mesh vertex tangents size not equal to index size!");
			BOOST_ASSERT(0);
		}

	}

	void FbxSceneImporter::ProcessLight(FbxNode* pNode,GameObjectPtr const& gameObject)
	{
		 FbxLight* lLight = (FbxLight*) pNode->GetNodeAttribute();
		 LightPtr lightObj = boost::make_shared<Light>();
		
		 if(lLight->LightType.Get() == FbxLight::ePoint )
			 lightObj->LType = LT_Point;
		 else if(lLight->LightType.Get() == FbxLight::eDirectional )
			 lightObj->LType = LT_Parallel;
		 else if(lLight->LightType.Get() == FbxLight::eSpot )
			 lightObj->LType = LT_Spot;
		 else
		 {
			 GLogger->Error("Not supported light Type!");
			 BOOST_ASSERT(0);
		 }

		 lightObj->Color.x = (float)lLight->Color.Get()[0];
		 lightObj->Color.y = (float)lLight->Color.Get()[1];
		 lightObj->Color.z = (float)lLight->Color.Get()[2];

		 lightObj->Intensity = (float)lLight->Intensity.Get();
		 lightObj->Range = (float)lLight->InnerAngle.Get();

		 gameObject->AddComponent(lightObj);
	}

	void FbxSceneImporter::ProcessMaterials(FbxMesh *pMesh,std::vector<MaterialPtr> & materials)
	{
		int lMaterialCount = 0;
		FbxNode* lNode = pMesh->GetNode();
		if( lNode )
			lMaterialCount = lNode->GetMaterialCount();
 
		if (lMaterialCount > 0)
		{
			FbxPropertyT<FbxDouble3> lKFbxDouble3;
			FbxPropertyT<FbxDouble> lKFbxDouble1;
			FbxColor theColor;

			for (int lCount = 0; lCount < lMaterialCount; lCount ++)
			{
				MaterialPtr material = boost::make_shared<Material>();
				FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);
				material->Name = (char *) lMaterial->GetName();
			    material->ShaderModel = lMaterial->ShadingModel.Get().Buffer();

				//Get the implementation to see if it's a hardware shader.
				const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
				if(lImplementation)
				{
					material->Type = MT_HLSL;
				}
				else
				{
					lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
					if(lImplementation)
					{
						material->Type = MT_CGFX;
					}
					else
					{
						lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_MENTALRAY);
						if(lImplementation)
							material->Type = MT_MentalRay;
						else
							material->Type = MT_None;
					}
				}
 
				if(lImplementation)
				{
					//Now we have a hardware shader, let's read it					 
					FbxBindingTable const* lRootTable = lImplementation->GetRootTable();
					FbxString lFileName = lRootTable->DescAbsoluteURL.Get();
					FbxString lTechniqueName = lRootTable->DescTAG.Get(); 
 
					FbxBindingTable const* lTable = lImplementation->GetRootTable();
					size_t lEntryNum = lTable->GetEntryCount();

					for(int i=0;i <(int)lEntryNum; ++i)
					{
						const FbxBindingTableEntry& lEntry = lTable->GetEntry(i);
						const char* lEntrySrcType = lEntry.GetEntryType(true); 
						FbxProperty lFbxProp;
 
						FbxString lTest = lEntry.GetSource();
						FBXSDK_printf("            Entry: %s\n", lTest.Buffer());
 
						if ( strcmp( FbxPropertyEntryView::sEntryType, lEntrySrcType ) == 0 )
						{   
							lFbxProp = lMaterial->FindPropertyHierarchical(lEntry.GetSource()); 
							if(!lFbxProp.IsValid())
							{
								lFbxProp = lMaterial->RootProperty.FindHierarchical(lEntry.GetSource());
							}
						}
						else if( strcmp( FbxConstantEntryView::sEntryType, lEntrySrcType ) == 0 )
						{
							lFbxProp = lImplementation->GetConstants().FindHierarchical(lEntry.GetSource());
						}
						if(lFbxProp.IsValid())
						{
							if( lFbxProp.GetSrcObjectCount<FbxTexture>() > 0 )
							{
								//do what you want with the textures
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxFileTexture>(); ++j)
								{
									FbxFileTexture *lTex = lFbxProp.GetSrcObject<FbxFileTexture>(j);
									FBXSDK_printf("           File Texture: %s\n", lTex->GetFileName());
								}
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxLayeredTexture>(); ++j)
								{
									FbxLayeredTexture *lTex = lFbxProp.GetSrcObject<FbxLayeredTexture>(j);
									FBXSDK_printf("        Layered Texture: %s\n", lTex->GetName());
								}
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxProceduralTexture>(); ++j)
								{
									FbxProceduralTexture *lTex = lFbxProp.GetSrcObject<FbxProceduralTexture>(j);
									FBXSDK_printf("     Procedural Texture: %s\n", lTex->GetName());
								}
							}
							else
							{
								FbxDataType lFbxType = lFbxProp.GetPropertyDataType();
								FbxString blah = lFbxType.GetName();
								if(FbxBoolDT == lFbxType)
								{
									bool b = lFbxProp.Get<FbxBool>();
								}
								else if ( FbxIntDT == lFbxType ||  FbxEnumDT  == lFbxType )
								{
									int i = lFbxProp.Get<FbxInt>();
								}
								else if ( FbxFloatDT == lFbxType)
								{
									float f = lFbxProp.Get<FbxFloat>();

								}
								else if ( FbxDoubleDT == lFbxType)
								{
									double d = lFbxProp.Get<FbxDouble>();
								}
								else if ( FbxStringDT == lFbxType
									||  FbxUrlDT  == lFbxType
									||  FbxXRefUrlDT  == lFbxType )
								{
									std::string s = lFbxProp.Get<FbxString>().Buffer();
								}
								else if ( FbxDouble2DT == lFbxType)
								{
									FbxDouble2 lDouble2 = lFbxProp.Get<FbxDouble2>();
									FbxVector2 lVect;
									lVect[0] = lDouble2[0];
									lVect[1] = lDouble2[1];
								}
								else if ( FbxDouble3DT == lFbxType || FbxColor3DT == lFbxType)
								{
									FbxDouble3 lDouble3 = lFbxProp.Get<FbxDouble3>();

									FbxVector4 lVect;
									lVect[0] = lDouble3[0];
									lVect[1] = lDouble3[1];
									lVect[2] = lDouble3[2];
								}

								else if ( FbxDouble4DT == lFbxType || FbxColor4DT == lFbxType)
								{
									FbxDouble4 lDouble4 = lFbxProp.Get<FbxDouble4>();
									FbxVector4 lVect;
									lVect[0] = lDouble4[0];
									lVect[1] = lDouble4[1];
									lVect[2] = lDouble4[2];
									lVect[3] = lDouble4[3];						
								}
								else if ( FbxDouble4x4DT == lFbxType)
								{
									FbxDouble4x4 lDouble44 = lFbxProp.Get<FbxDouble4x4>();
									for(int j=0; j<4; ++j)
									{

										FbxVector4 lVect;
										lVect[0] = lDouble44[j][0];
										lVect[1] = lDouble44[j][1];
										lVect[2] = lDouble44[j][2];
										lVect[3] = lDouble44[j][3];
									}

								}
							}

						}   
					}
				} //if(lImplementation)
				else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
				{
					// We found a Phong material.  Display its properties.
					// Display the Ambient Color
					material->Type = MT_Phong;
					FbxSurfacePhong* lPhoneMaterial = (FbxSurfacePhong *) lMaterial;
					material->AmbientColor.x = (float)lPhoneMaterial->Ambient.Get()[0];
					material->AmbientColor.y = (float)lPhoneMaterial->Ambient.Get()[1];
					material->AmbientColor.z = (float)lPhoneMaterial->Ambient.Get()[2];

					// Display the Diffuse Color
					material->DiffuseColor.x = (float)lPhoneMaterial->Diffuse.Get()[0];
					material->DiffuseColor.y = (float)lPhoneMaterial->Diffuse.Get()[1];
					material->DiffuseColor.z = (float)lPhoneMaterial->Diffuse.Get()[2];

					// Display the Specular Color (unique to Phong materials)
					material->SpecularColor.x = (float)lPhoneMaterial->Specular.Get()[0];
					material->SpecularColor.y = (float)lPhoneMaterial->Specular.Get()[1];
					material->SpecularColor.z = (float)lPhoneMaterial->Specular.Get()[2];

					// Display the Emissive Color
					material->EmissiveColor.x = (float)lPhoneMaterial->Emissive.Get()[0];
					material->EmissiveColor.y = (float)lPhoneMaterial->Emissive.Get()[1];
					material->EmissiveColor.z = (float)lPhoneMaterial->Emissive.Get()[2];

					//Opacity is Transparency factor now
					material->Opacity = (float)(1.0-lPhoneMaterial->TransparencyFactor.Get());

					// Display the Shininess
					material->Shininess = (float)lPhoneMaterial->Shininess.Get();

					// Display the Reflectivity
					material->Reflectivity =  (float)lPhoneMaterial->ReflectionFactor.Get();
				}
				else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
				{
					// We found a Lambert material. Display its properties.
					// Display the Ambient Color
					material->Type = MT_Lambert;
					FbxSurfaceLambert* lLambertMaterial = (FbxSurfaceLambert *)lMaterial;
					material->AmbientColor.x = (float)lLambertMaterial->Ambient.Get()[0];
					material->AmbientColor.y = (float)lLambertMaterial->Ambient.Get()[1];
					material->AmbientColor.z = (float)lLambertMaterial->Ambient.Get()[2];

					// Display the Diffuse Color
					material->DiffuseColor.x = (float)lLambertMaterial->Diffuse.Get()[0];
					material->DiffuseColor.y = (float)lLambertMaterial->Diffuse.Get()[1];
					material->DiffuseColor.z = (float)lLambertMaterial->Diffuse.Get()[2];

					// Display the Emissive
					material->EmissiveColor.x = (float)lLambertMaterial->Emissive.Get()[0];
					material->EmissiveColor.y = (float)lLambertMaterial->Emissive.Get()[1];
					material->EmissiveColor.z = (float)lLambertMaterial->Emissive.Get()[2];

					// Display the Opacity
					material->Opacity = (float)(1.0 - lLambertMaterial->TransparencyFactor.Get());
				}
                else
				{
					GLogger->Warning("Unkown Material type!");
				}
 
				materials.push_back(material);
          
           }//for (int lCount = 0; lCount < lMaterialCount; lCount ++)
        }
	}

	void FbxSceneImporter::ProcessMesh(FbxNode *pNode,GameObjectPtr const& gameObject)
	{
		 FbxMesh* lMesh = (FbxMesh*) pNode->GetNodeAttribute ();
		 GeometryPtr geometry = boost::make_shared<Geometry>();
		 GeometryRendererPtr geometryRender = boost::make_shared<GeometryRenderer>();
		 
		 geometry->Name = pNode->GetName();
   
		 // skip meta data connections...
  
   
		 ProcessGeometry(lMesh,geometry);

		 std::vector<MaterialPtr> matArray;
		 ProcessMaterials(lMesh,matArray);

		 // use default material now
		 MaterialPtr mat = MaterialGenerator::GeneratePhong(Vector3(0.1f),Vector3(0.6f),Vector3(0.7f),1);
		 geometryRender->SetGeometry(geometry,mat);


		//DisplayMaterialMapping(lMesh);
		/*DisplayMaterial(lMesh);
		DisplayTexture(lMesh);
		DisplayMaterialConnections(lMesh);*/
		
		// skip link...

		// skip shape...

		 gameObject->AddComponent(geometryRender);
	}

	GameObjectPtr FbxSceneImporter::ProcessTranform(FbxNode* pNode)
	{
		FbxVector4 lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		Vector3 position(lTmpVector[0],lTmpVector[1],lTmpVector[2]);
		lTmpVector =  pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		Matrix3 rotMatrix;
		rotMatrix.FromEulerAnglesXYZ((float)lTmpVector[0],(float)lTmpVector[1],(float)lTmpVector[2]);
		Quaternion rot;
		rot.FromRotationMatrix(rotMatrix);
		lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		Vector3 scale(lTmpVector[0],lTmpVector[1],lTmpVector[2]);
		std::string name(pNode->GetName());
		GameObjectPtr gameObject = boost::make_shared<GameObject>(name,position,rot,scale);
		return gameObject;
	}
}