#include "EngineInclude.h"
 

namespace Disorder
{
	SceneImporter *GSceneImporter;

	FbxSceneImporter::FbxSceneImporter()
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

	FbxSceneImporter::~FbxSceneImporter()
	{
		if (_sdkManager)
		{
			_sdkManager->Destroy();
			_sdkManager = NULL;
		}
	}
 
	

	Level* FbxSceneImporter::Import(std::string const& fileName)
	{
		std::string fbxFile = GConfig->ResourceFBXPath + fileName;
		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor,  lSDKMinor,  lSDKRevision;	 
		bool lStatus = false;
		// Get the file version number generate by the FBX SDK.
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		//Create an FBX scene. This object holds most objects imported/exported from/to files.
		size_t dotPos = fileName.find_first_of('.');
		_cachedCurrentScene = dotPos == std::string::npos ? fileName : fileName.substr(0, dotPos);
		FbxScene* lscene = FbxScene::Create(_sdkManager, _cachedCurrentScene.c_str());
	 
		BOOST_ASSERT(lscene);

		if (!lscene)
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
			GLogger->Error(std::string("Call to FbxImporter::Initialize() failed.") + lImporter->GetStatus().GetErrorString());
			_cachedCurrentScene = "";
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

		if(lStatus == false )
		{
			GLogger->Error("FBX file need a password!!!!");
			_cachedCurrentScene = "";
			return NULL;
		}
 
		Level* level = new Level(_cachedCurrentScene);

		PreProcessGlobalSetting(lscene, level);

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter lGeomConverter(_sdkManager);
		lGeomConverter.Triangulate(lscene, /*replace*/true);

		// Split meshes per material, so that we only have one material per mesh (for VBO support)
		lGeomConverter.SplitMeshesPerMaterial(lscene, /*replace*/true);
 
		ProcessHierarchy(lscene, level);

		PostProcessGlobalSetting(lscene, level);
		// Destroy the importer.
		lImporter->Destroy();

		GWorld->AddLevel(level);

		_cachedCurrentScene = "";

		return level;
	}

	void FbxSceneImporter::PreProcessGlobalSetting(FbxScene* lscene,Level* level)
	{
		FbxGlobalSettings &rGlobalSetting =  lscene->GetGlobalSettings();
 
		//Axis
		FbxAxisSystem SceneAxisSystem = rGlobalSetting.GetAxisSystem();
		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if (SceneAxisSystem != OurAxisSystem)
		{
			OurAxisSystem.ConvertScene(lscene);
		}

		//system unit
		FbxSystemUnit SceneSystemUnit = rGlobalSetting.GetSystemUnit();
		if (SceneSystemUnit.GetScaleFactor() != 1.0)
		{
			//The unit is centimeter.
			FbxSystemUnit::cm.ConvertScene(lscene);
		}
 

		FbxColor ambient = rGlobalSetting.GetAmbientColor();
		level->SetAmbientColor(glm::vec4((float)ambient.mRed, (float)ambient.mGreen, (float)ambient.mBlue, (float)ambient.mAlpha));
 
	}

	void FbxSceneImporter::PostProcessGlobalSetting(FbxScene* lscene,Level* level)
	{
		FbxGlobalSettings &rGlobalSetting =  lscene->GetGlobalSettings();

		FbxString& defaultCamera = rGlobalSetting.GetDefaultCamera();
		if( !defaultCamera.IsEmpty())
		{
			std::string strCamera = defaultCamera.Buffer();
			Camera* cameraPtr = GSceneManager->GetCamera(strCamera);
			if(cameraPtr != NULL )
			{
				GSceneManager->SetDefaultCamera(cameraPtr);
			}
		}

	

	}
	 

    void FbxSceneImporter::ProcessHierarchy(FbxScene* lscene,Level* level)
	{
		FbxNode* lNode = lscene->GetRootNode();

		if(lNode)
		{
			for(int i = 0; i < lNode->GetChildCount(); i++)
			{
				FbxNode* pNode = lNode->GetChild(i);
				GameObject* gameObject = ProcessTranform(pNode);
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

	void FbxSceneImporter::ProcessHierarchy(FbxNode* pNode,GameObject* gameObject)
	{
		GameObject* childObject = ProcessTranform(pNode);
		ProcessContent(pNode,childObject);
		gameObject->AddChild(childObject);
		int childnum = pNode->GetChildCount();
		for(int ic = 0;ic < childnum;ic++ )
		{
			FbxNode* pChildNode = pNode->GetChild(ic);
			ProcessHierarchy(pChildNode,gameObject);
		}
				 
	}

	void FbxSceneImporter::ProcessContent(FbxNode* pNode,GameObject* gameObject)
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
            ProcessCamera(pNode,gameObject);
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

	void FbxSceneImporter::ProcessCamera(FbxNode* pNode,GameObject* gameObject)
	{
		FbxCamera *pCamera = (FbxCamera*)pNode->GetNodeAttribute();
		BOOST_ASSERT(pCamera);

		Camera* cameraPtr = new Camera(pNode->GetName());
		BOOST_ASSERT(pNode->GetTarget() == NULL); // don't process target node now.

		FbxDouble3& position = pCamera->Position.Get();
		FbxDouble3& target = pCamera->InterestPosition.Get();
		FbxDouble3& upVec = pCamera->UpVector.Get();

		glm::vec3 vpos((float)position[0],(float)position[1],(float)position[2]);
		glm::vec3 vtarget((float)target[0],(float)target[1],(float)target[2]);
		glm::vec3 vup((float)upVec[0],(float)upVec[1],(float)upVec[2]);

		cameraPtr->LookAt(vpos,vtarget,vup);
		cameraPtr->ProjCalculate((float)pCamera->FieldOfView.Get(),(float)pCamera->NearPlane.Get(),(float)pCamera->FarPlane.Get());
		gameObject->AddComponent(cameraPtr);
		
	}

	void FbxSceneImporter::ProcessGeometry(FbxMesh *pMesh,Geometry* geometry)
	{
		int lPolygonCount = pMesh->GetPolygonCount();
        FbxVector4* lControlPoints = pMesh->GetControlPoints(); 
		int vertexCount = pMesh->GetControlPointsCount();

		geometry->Name = pMesh->GetName();
		geometry->ControllPositions.resize(vertexCount);
		// Vertex read
		for(int i=0;i<vertexCount;i++)
		{
			geometry->ControllPositions[i].x = (float)lControlPoints[i][0]; 
			geometry->ControllPositions[i].y = (float)lControlPoints[i][1];
			geometry->ControllPositions[i].z = (float)lControlPoints[i][2];
		}
 
		// calculate bounding box
		geometry->CalculateBounding();

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
				vertexId++;
			} // for polygonSize
		} // for polygonCount
 
		vertexId = 0;
		if( pMesh->GetElementVertexColorCount() > 0 )
			geometry->Colors.resize(geometry->Indices.size());
		if( pMesh->GetElementUVCount() > 0 )
			geometry->Texcoords.resize(geometry->Indices.size());
		if( pMesh->GetElementNormalCount() > 0 )
			geometry->Normals.resize(geometry->Indices.size());
		if( pMesh->GetElementTangentCount() > 0 )
			geometry->Tangents.resize(geometry->Indices.size());
		if( pMesh->GetElementBinormalCount() > 0 )
			geometry->Binormals.resize(geometry->Indices.size());

		for (int i = 0; i < lPolygonCount; i++)
		{
			int lPolygonSize = pMesh->GetPolygonSize(i);
			for (int j = 0; j < lPolygonSize; j++)
			{
				int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
				// vertex color.
				for ( int l = 0; l < pMesh->GetElementVertexColorCount(); l++)
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
										geometry->Colors[vertexId].x = (float)fColor.mRed;
										geometry->Colors[vertexId].y = (float)fColor.mGreen;
										geometry->Colors[vertexId].z = (float)fColor.mBlue;
										geometry->Colors[vertexId].w = (float)fColor.mAlpha;
										break;
									}
								case FbxGeometryElement::eIndexToDirect:
									{
										int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
										FbxColor fColor = leVtxc->GetDirectArray().GetAt(id);
										geometry->Colors[vertexId].x = (float)fColor.mRed;
										geometry->Colors[vertexId].y = (float)fColor.mGreen;
										geometry->Colors[vertexId].z = (float)fColor.mBlue;
										geometry->Colors[vertexId].w = (float)fColor.mAlpha;
										break;
									}
								default:
									BOOST_ASSERT(0);
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
											geometry->Colors[vertexId].x = (float)fColor.mRed;
											geometry->Colors[vertexId].y = (float)fColor.mGreen;
											geometry->Colors[vertexId].z = (float)fColor.mBlue;
											geometry->Colors[vertexId].w = (float)fColor.mAlpha;
										}
										break;
									case FbxGeometryElement::eIndexToDirect:
										{
											int id = leVtxc->GetIndexArray().GetAt(vertexId);
											FbxColor fColor = leVtxc->GetDirectArray().GetAt(id);
											geometry->Colors[vertexId].x = (float)fColor.mRed;
											geometry->Colors[vertexId].y = (float)fColor.mGreen;
											geometry->Colors[vertexId].z = (float)fColor.mBlue;
											geometry->Colors[vertexId].w = (float)fColor.mAlpha;
										}
										break;
									default:
										BOOST_ASSERT(0);
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
				for ( int l = 0; l <  pMesh->GetElementUVCount(); ++l)
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
									geometry->Texcoords[vertexId].x = (float)uv[0];
									geometry->Texcoords[vertexId].y = (float)uv[1];
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
									FbxVector2 uv = leUV->GetDirectArray().GetAt(id);
									geometry->Texcoords[vertexId].x = (float)uv[0];
									geometry->Texcoords[vertexId].y = (float)uv[1];
								}
								break;
							default:
								BOOST_ASSERT(0);
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
										geometry->Texcoords[vertexId].x = (float)uv[0];
									    geometry->Texcoords[vertexId].y = (float)uv[1];
									}
									break;
								default:
									BOOST_ASSERT(0);
									break; // other reference modes not shown here!
							}
						}
						break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						GLogger->Error("Fbx UV not supported mapping mode!");
						break;
					}
				}

				// normal
				unsigned int normalNumber =  pMesh->GetElementNormalCount();
				 
				for(unsigned int l = 0; l < normalNumber; ++l)
				{
					FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
					FbxGeometryElement::EMappingMode NormalMapMode = leNormal->GetMappingMode();
					if( NormalMapMode == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
								{
									FbxVector4 normal = leNormal->GetDirectArray().GetAt(vertexId);
									normal.Normalize();
									geometry->Normals[vertexId].x = (float)normal[0];
									geometry->Normals[vertexId].y = (float)normal[1];
									geometry->Normals[vertexId].z = (float)normal[2];
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leNormal->GetIndexArray().GetAt(vertexId);
									FbxVector4 normal = leNormal->GetDirectArray().GetAt(id);
									normal.Normalize();
									geometry->Normals[vertexId].x = (float)normal[0];
									geometry->Normals[vertexId].y = (float)normal[1];
									geometry->Normals[vertexId].z = (float)normal[2];
								}
								break;
							default:
								BOOST_ASSERT(0);
								break; // other reference modes not shown here!
						}
					}
					else if( NormalMapMode == FbxGeometryElement::eByControlPoint)
					{
						switch (leNormal->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
								{
									FbxVector4 normal = leNormal->GetDirectArray().GetAt(lControlPointIndex);
									normal.Normalize();
                                    geometry->Normals[vertexId].x = (float)normal[0];
									geometry->Normals[vertexId].y = (float)normal[1];
									geometry->Normals[vertexId].z = (float)normal[2];
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);
									FbxVector4 normal = leNormal->GetDirectArray().GetAt(id);
									normal.Normalize();
									geometry->Normals[vertexId].x = (float)normal[0];
									geometry->Normals[vertexId].y = (float)normal[1];
									geometry->Normals[vertexId].z = (float)normal[2];
								}
								break;
							default:
								BOOST_ASSERT(0);
								break;
						}
					}
					else
						GLogger->Error("Fbx Normal not supported mapping mode!");
				}

				// tangent
				unsigned int taNumber = pMesh->GetElementTangentCount();
				 
				for(unsigned int l = 0; l < taNumber ; ++l)
				{
					FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l);
				    FbxGeometryElement::EMappingMode tangentMapMode = leTangent->GetMappingMode();

					if(tangentMapMode == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leTangent->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
								{
									FbxVector4 tangent = leTangent->GetDirectArray().GetAt(vertexId);
									tangent.Normalize();
									geometry->Tangents[vertexId].x = (float)tangent[0];
									geometry->Tangents[vertexId].y = (float)tangent[1];
									geometry->Tangents[vertexId].z = (float)tangent[2]; 
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leTangent->GetIndexArray().GetAt(vertexId);
									FbxVector4 tangent = leTangent->GetDirectArray().GetAt(id);
									tangent.Normalize();
									geometry->Tangents[vertexId].x = (float)tangent[0];
									geometry->Tangents[vertexId].y = (float)tangent[1];
									geometry->Tangents[vertexId].z = (float)tangent[2]; 
								}
								break;
							default:
								BOOST_ASSERT(0);
								break; // other reference modes not shown here!
						}
					}
					else if( tangentMapMode == FbxGeometryElement::eByControlPoint )
					{
						switch (leTangent->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
								{
									FbxVector4 tangent = leTangent->GetDirectArray().GetAt(lControlPointIndex);
									tangent.Normalize();
								    geometry->Tangents[vertexId].x = (float)tangent[0];
									geometry->Tangents[vertexId].y = (float)tangent[1];
									geometry->Tangents[vertexId].z = (float)tangent[2]; 
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leTangent->GetIndexArray().GetAt(lControlPointIndex);
									FbxVector4 tangent = leTangent->GetDirectArray().GetAt(id);
									tangent.Normalize();
									geometry->Tangents[vertexId].x = (float)tangent[0];
									geometry->Tangents[vertexId].y = (float)tangent[1];
									geometry->Tangents[vertexId].z = (float)tangent[2]; 
								}
								break;
							default:
								BOOST_ASSERT(0);
								break; // other reference modes not shown here!
						}
					}
					else
					{
					   GLogger->Error("Fbx tangent not supported mapping mode!");
					}

				}

				//binormal
				unsigned int biNumber = pMesh->GetElementBinormalCount();
			 
				for(unsigned int l = 0; l < biNumber ; ++l)
				{
					FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal( l);
					FbxGeometryElement::EMappingMode BinormalMapNode = leBinormal->GetMappingMode();
					if( BinormalMapNode == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leBinormal->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
								{
									FbxVector4 binormal = leBinormal->GetDirectArray().GetAt(vertexId);
									binormal.Normalize();
									 
									geometry->Binormals[vertexId].x = (float)binormal[0]; 
									geometry->Binormals[vertexId].y = (float)binormal[1];
									geometry->Binormals[vertexId].z = (float)binormal[2];
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leBinormal->GetIndexArray().GetAt(vertexId);
									FbxVector4 binormal = leBinormal->GetDirectArray().GetAt(id);
									binormal.Normalize();
									geometry->Binormals[vertexId].x = (float)binormal[0]; 
									geometry->Binormals[vertexId].y = (float)binormal[1];
									geometry->Binormals[vertexId].z = (float)binormal[2];
								}
								break;
							default:
								BOOST_ASSERT(0);
								break; // other reference modes not shown here!
						}
					}
					else if( BinormalMapNode == FbxGeometryElement::eByControlPoint)
					{
						switch (leBinormal->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
								{
									FbxVector4 binormal = leBinormal->GetDirectArray().GetAt(lControlPointIndex);
									binormal.Normalize();
									geometry->Binormals[vertexId].x = (float)binormal[0]; 
									geometry->Binormals[vertexId].y = (float)binormal[1];
									geometry->Binormals[vertexId].z = (float)binormal[2];
								}
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leBinormal->GetIndexArray().GetAt(lControlPointIndex);
									FbxVector4 binormal = leBinormal->GetDirectArray().GetAt(id);
									binormal.Normalize();
									geometry->Binormals[vertexId].x = (float)binormal[0]; 
									geometry->Binormals[vertexId].y = (float)binormal[1];
									geometry->Binormals[vertexId].z = (float)binormal[2];
								}
								break;
							default:
								BOOST_ASSERT(0);
								break; // other reference modes not shown here!
						}
					}
				}
				vertexId++;
			}
		}
				
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

	void FbxSceneImporter::ProcessLight(FbxNode* pNode,GameObject* gameObject)
	{
		 FbxLight* lLight = (FbxLight*) pNode->GetNodeAttribute();
	 
		 if(lLight->LightType.Get() == FbxLight::ePoint )
		 {
			 PointLight* lightObj = new PointLight(gameObject->Name);
			 lightObj->Range = 50;
			 lightObj->Color.x = (float)lLight->Color.Get()[0];
		     lightObj->Color.y= (float)lLight->Color.Get()[1];
		     lightObj->Color.z = (float)lLight->Color.Get()[2];

		     lightObj->Intensity = (float)lLight->Intensity.Get()/100.f;
			 gameObject->AddComponent(lightObj);
		 }
		 else if(lLight->LightType.Get() == FbxLight::eDirectional )
		 {
			 DirectionLight* lightObj = new DirectionLight(gameObject->Name);
			 lightObj->Color.x = (float)lLight->Color.Get()[0];
		     lightObj->Color.y = (float)lLight->Color.Get()[1];
		     lightObj->Color.z = (float)lLight->Color.Get()[2];

		     lightObj->Intensity = (float)lLight->Intensity.Get()/100.f;
			 gameObject->AddComponent(lightObj);
		 }
		 else if(lLight->LightType.Get() == FbxLight::eSpot )
		 {
			 SpotLight* lightObj = new SpotLight(gameObject->Name);
			 lightObj->Range = 50;
			 lightObj->Color.x = (float)lLight->Color.Get()[0];
		     lightObj->Color.y = (float)lLight->Color.Get()[1];
		     lightObj->Color.z = (float)lLight->Color.Get()[2];

		     lightObj->Intensity = (float)lLight->Intensity.Get()/100.f;
			 lightObj->SpotInnerAngle = (float)lLight->InnerAngle.Get() * Math::fDeg2Rad;
			 lightObj->SpotOuterAngle = (float)lLight->OuterAngle.Get() * Math::fDeg2Rad;

			 gameObject->AddComponent(lightObj);
		 }
		 else
		 {
			 GLogger->Error("Not supported light Type!");
			 BOOST_ASSERT(0);
		 }
 
	}

	void FbxSceneImporter::ProcessTextures(FbxSurfaceMaterial *pMaterial, SurfaceMaterial* materials)
	{
		if (!pMaterial)
			return;

		FbxProperty lFbxProp = pMaterial->GetFirstProperty();
		while (lFbxProp.IsValid())
		{
			if (lFbxProp.GetSrcObjectCount<FbxTexture>() > 0)
			{
				for (int j = 0; j<lFbxProp.GetSrcObjectCount<FbxFileTexture>(); ++j)
				{
					FbxFileTexture *lTex = lFbxProp.GetSrcObject<FbxFileTexture>(j);
					ProcessFileTexture(lFbxProp.GetNameAsCStr(), lTex, materials);
				}
				for (int j = 0; j<lFbxProp.GetSrcObjectCount<FbxLayeredTexture>(); ++j)
				{
					FbxLayeredTexture *lTex = lFbxProp.GetSrcObject<FbxLayeredTexture>(j);
					ProcessLayerTexture(lFbxProp.GetNameAsCStr(), lTex, materials);
				}
				for (int j = 0; j<lFbxProp.GetSrcObjectCount<FbxProceduralTexture>(); ++j)
				{
					FbxProceduralTexture *lTex = lFbxProp.GetSrcObject<FbxProceduralTexture>(j);
					ProcessProceduralTexture(lFbxProp.GetNameAsCStr(), lTex, materials);
				}
			}

			lFbxProp = pMaterial->GetNextProperty(lFbxProp);
		}
	}

	void FbxSceneImporter::ProcessMaterials(FbxMesh *pMesh,std::vector<SurfaceMaterial*> & materials, int &usedMaterial)
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
				FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);
				SurfaceMaterial* material = new SurfaceMaterial(lMaterial->GetName());
				//Get the implementation to see if it's a hardware shader.
				const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
				if(lImplementation)
				{
					//material->Type = MT_HLSL;
				}
				else
				{
					lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
					if(lImplementation)
					{
						//material->Type = MT_CGFX;
					}
					else
					{
						lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_MENTALRAY);
						//if(lImplementation)
							//material->Type = MT_MentalRay;
						//else
							//material->Type = MT_None;
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

					for( int i=0;i < (int)lEntryNum; ++i)
					{
						const FbxBindingTableEntry& lEntry = lTable->GetEntry(i);
						const char* lEntrySrcType = lEntry.GetEntryType(true); 
						FbxProperty lFbxProp;
 
						FbxString lTest = lEntry.GetSource();
						
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
									ProcessFileTexture(lFbxProp.GetNameAsCStr(),lTex, material);
								}
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxLayeredTexture>(); ++j)
								{
									FbxLayeredTexture *lTex = lFbxProp.GetSrcObject<FbxLayeredTexture>(j);
									ProcessLayerTexture(lFbxProp.GetNameAsCStr(), lTex, material);
								}
								for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxProceduralTexture>(); ++j)
								{
									FbxProceduralTexture *lTex = lFbxProp.GetSrcObject<FbxProceduralTexture>(j);
									ProcessProceduralTexture(lFbxProp.GetNameAsCStr(), lTex, material);
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
				else
				{
					if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
					{
						// We found a Phong material.  Display its properties.
						// Display the Ambient Color
						material->ShaderModel = lMaterial->ShadingModel.Get().Buffer();

						FbxSurfacePhong* lPhoneMaterial = (FbxSurfacePhong *)lMaterial;

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
						material->Transparency = (float)(lPhoneMaterial->TransparencyFactor.Get());

						//textures
						ProcessTextures(lPhoneMaterial, material);

						materials.push_back(material);
					}
					else if (lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
					{
						// We found a Lambert material. Display its properties.
						// Display the Ambient Color
						material->ShaderModel = lMaterial->ShadingModel.Get().Buffer();

						FbxSurfaceLambert* lLambertMaterial = (FbxSurfaceLambert *)lMaterial;

						// Display the Diffuse Color
						material->DiffuseColor.x = (float)lLambertMaterial->Diffuse.Get()[0];
						material->DiffuseColor.y = (float)lLambertMaterial->Diffuse.Get()[1];
						material->DiffuseColor.z = (float)lLambertMaterial->Diffuse.Get()[2];

						// Display the Emissive
						material->EmissiveColor.x = (float)lLambertMaterial->Emissive.Get()[0];
						material->EmissiveColor.y = (float)lLambertMaterial->Emissive.Get()[1];
						material->EmissiveColor.z = (float)lLambertMaterial->Emissive.Get()[2];

						// Display the Opacity
						material->Transparency = (float)(lLambertMaterial->TransparencyFactor.Get());

						//textures
						ProcessTextures(lLambertMaterial, material);
						materials.push_back(material);

					}
					else
					{
						GLogger->Warning("Unkown Material type!");
					}
				}
				material->UpdateTextureResource();
           }//for (int lCount = 0; lCount < lMaterialCount; lCount ++)
        }

		//Material Map
		for(int l = 0; l < pMesh->GetElementMaterialCount(); ++l)
		{
			FbxGeometryElementMaterial* leMaterial = pMesh->GetElementMaterial(l);
			FbxGeometryElement::EMappingMode MaterialMapMode = leMaterial->GetMappingMode();
			FbxLayerElement::EReferenceMode ReferenceMode = leMaterial->GetReferenceMode();
			BOOST_ASSERT( MaterialMapMode == FbxGeometryElement::eAllSame );  
			usedMaterial = leMaterial->GetIndexArray().GetAt(0);
			 
		}
	}

	/*void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode)
	{
		FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
		FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);

		DisplayString("            Name: \"", (char *)pTexture->GetName(), "\"");
		if (lFileTexture)
		{
			DisplayString("            Type: File Texture");
			DisplayString("            File Name: \"", (char *)lFileTexture->GetFileName(), "\"");
		}
		else if (lProceduralTexture)
		{
			DisplayString("            Type: Procedural Texture");
		}
		DisplayDouble("            Scale U: ", pTexture->GetScaleU());
		DisplayDouble("            Scale V: ", pTexture->GetScaleV());
		DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
		DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
		DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
		DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
		DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
		DisplayDouble("            Rotation W: ", pTexture->GetRotationW());

		const char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

		DisplayString("            Alpha Source: ", lAlphaSources[pTexture->GetAlphaSource()]);
		DisplayDouble("            Cropping Left: ", pTexture->GetCroppingLeft());
		DisplayDouble("            Cropping Top: ", pTexture->GetCroppingTop());
		DisplayDouble("            Cropping Right: ", pTexture->GetCroppingRight());
		DisplayDouble("            Cropping Bottom: ", pTexture->GetCroppingBottom());

		const char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical",
			"Box", "Face", "UV", "Environment" };

		DisplayString("            Mapping Type: ", lMappingTypes[pTexture->GetMappingType()]);

		if (pTexture->GetMappingType() == FbxTexture::ePlanar)
		{
			const char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

			DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
		}

		const char* lBlendModes[] = { "Translucent", "Add", "Modulate", "Modulate2" };
		if (pBlendMode >= 0)
			DisplayString("            Blend Mode: ", lBlendModes[pBlendMode]);
		DisplayDouble("            Alpha: ", pTexture->GetDefaultAlpha());

		if (lFileTexture)
		{
			const char* lMaterialUses[] = { "Model Material", "Default Material" };
			DisplayString("            Material Use: ", lMaterialUses[lFileTexture->GetMaterialUse()]);
		}

		const char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map",
			"Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

		DisplayString("            Texture Use: ", pTextureUses[pTexture->GetTextureUse()]);
		DisplayString("");

	}*/
 
	void FbxSceneImporter::ProcessFileTexture(const char* propertyName, FbxFileTexture* pTexture, SurfaceMaterial* materials)
	{
		if (materials->TextureChannelMap.find(propertyName) != materials->TextureChannelMap.end())
			return;

		const FbxString lFileName = pTexture->GetFileName();

		FbxTexture::EWrapMode wrapU = pTexture->GetWrapModeU();
		FbxTexture::EWrapMode wrapV = pTexture->GetWrapModeV();
		double scaleU = pTexture->GetScaleU();
		double scaleV = pTexture->GetScaleV();
		double transU = pTexture->GetTranslationU();
		double transV = pTexture->GetTranslationV();

		std::string LoadFileName = lFileName.Buffer();
		boost::filesystem::path p(LoadFileName);
		if (!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p))
		{
			size_t pos = LoadFileName.find("/Fbx/");
			if ( pos == std::string::npos )
				pos = LoadFileName.find("\\Fbx\\");

			if (pos == std::string::npos)
				return;

			if (pos + 5 < LoadFileName.size())
			{
				LoadFileName = LoadFileName.substr(pos + 5);
				LoadFileName = GConfig->ResourceFBXPath + LoadFileName;

				p = boost::filesystem::path(LoadFileName);
				if (!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p))
				{

					GLogger->Warning(std::string(propertyName) + std::string("Can't find file:") + lFileName.Buffer());
					return;

				}
			}
			else
			{
				GLogger->Warning(std::string(propertyName) + std::string("Can't find file:") + lFileName.Buffer());
				return;
			}
		}
 
		std::string suffix = lFileName.Right(3).Buffer();
		if (!GImageManager->IsSupported(suffix))
		{
			GLogger->Error("Not supported texture format! :" + suffix);
			return;
		}
 
		SamplerDesc desc;
		if (wrapU == FbxTexture::eRepeat)
			desc.AddressU = TAM_Wrap;
		else if (wrapU == FbxTexture::eClamp)
			desc.AddressU = TAM_Clamp;

		if (wrapV == FbxTexture::eRepeat)
			desc.AddressV = TAM_Wrap;
		else if (wrapV == FbxTexture::eClamp)
			desc.AddressV = TAM_Clamp;

		Image* img = GImageManager->Load(LoadFileName, &desc);
		if (img != NULL && img->GetResource() != NULL )
		{
			RenderTexture2D* texRes = img->GetResource();
			materials->TextureChannelMap.insert(std::pair<std::string, RenderTexture2D*>(propertyName, texRes));
		}
	}

	void FbxSceneImporter::ProcessLayerTexture(const char* propertyName, FbxLayeredTexture* pTexture, SurfaceMaterial* materials)
	{
		GLogger->Error("Not support layer texture now!");
		BOOST_ASSERT(0);
	}

	void FbxSceneImporter::ProcessProceduralTexture(const char* propertyName, FbxProceduralTexture* pTexture, SurfaceMaterial* materials)
	{
		GLogger->Error("Not support procedural texture now!");
		BOOST_ASSERT(0);
	}
 
	void FbxSceneImporter::ProcessMesh(FbxNode *pNode,GameObject* gameObject)
	{
		 FbxMesh* lMesh = (FbxMesh*) pNode->GetNodeAttribute ();
		 Geometry* geometry = new Geometry(lMesh->GetName());
		 GeometryRenderer* geometryRender = new GeometryRenderer(gameObject->Name);
	 
		 // skip meta data connections...
 
		 ProcessGeometry(lMesh,geometry);

		 std::vector<SurfaceMaterial*> matArray;
		 int usedMaterial = 0;
		 ProcessMaterials(lMesh,matArray,usedMaterial);

		 // use default material now
		 if( matArray.size() > 0 )
			 geometryRender->SetGeometry(geometry,matArray[usedMaterial]);
		 else
		 {
			 SurfaceMaterial* mat = new SurfaceMaterial("DefaultMaterial");
		     geometryRender->SetGeometry(geometry,mat);
		 }

		//DisplayMaterialMapping(lMesh);
		/*DisplayMaterial(lMesh);
		DisplayTexture(lMesh);
		DisplayMaterialConnections(lMesh);*/
		
		// skip link...

		// skip shape...

		 gameObject->AddComponent(geometryRender);
	}

	//////////////////////////////////////////////////////////////////////////
	/// The fbx transform same to maya and as follow
	/// WorldTransform = ParentWorldTransform * T * Roff * Rp * Rpre * R * Rpost -1 * Rp -1 * Soff * Sp * S * Sp -1
	//////////////////////////////////////////////////////////////////////////
	GameObject* FbxSceneImporter::ProcessTranform(FbxNode* pNode)
	{
		// now we don't take care of pose information
		FbxVector4 lGeometryPostion = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 lTmpVector = pNode->EvaluateLocalTranslation();
		glm::vec3 position((float)lTmpVector[0], (float)lTmpVector[1], (float)lTmpVector[2]);
		lTmpVector =  pNode->EvaluateLocalRotation();
	
		glm::quat rot = glm::angleAxis((float)lTmpVector[2] * Math::fDeg2Rad, glm::vec3(0, 0, 1)) *
			            glm::angleAxis((float)lTmpVector[1] * Math::fDeg2Rad, glm::vec3(0, 1, 0)) *
			            glm::angleAxis((float)lTmpVector[0] * Math::fDeg2Rad, glm::vec3(1, 0, 0));

		lTmpVector = pNode->EvaluateLocalScaling();
		glm::vec3 scale((float)lTmpVector[0], (float)lTmpVector[1], (float)lTmpVector[2]);
		std::string name(pNode->GetName());
		GameObject* gameObject = new GameObject(name,position,rot,scale);
		return gameObject;
	}
}