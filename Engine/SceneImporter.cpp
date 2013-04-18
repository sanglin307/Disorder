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
		std::string fbxFile = Disorder::GConfig->sRunningDictioary + "\\Resource\\Fbx\\" + fileName;
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
				stream.clear();
				stream << "FBX file format version for file " << fileName << " is "<< lFileMajor<<","<< lFileMinor<<","<< lFileRevision;
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

	void FbxSceneImporter::ProcessHierarchy(FbxNode* node,GameObjectPtr const& gameObject)
	{
		GameObjectPtr childObject = ProcessTranform(node);
		gameObject->AddChild(childObject);
		int childnum = node->GetChildCount();
		for(int ic = 0;ic < childnum;ic++ )
		{
			FbxNode* pChildNode = node->GetChild(ic);
			ProcessHierarchy(pChildNode,gameObject);
		}
				 
	}

	GameObjectPtr FbxSceneImporter::ProcessTranform(FbxNode* pNode)
	{
		FbxVector4 lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		Vector3 position(lTmpVector[0],lTmpVector[1],lTmpVector[2]);
		lTmpVector =  pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		Matrix3 rotMatrix;
		rotMatrix.FromEulerAnglesXYZ(lTmpVector[0],lTmpVector[1],lTmpVector[2]);
		Quaternion rot;
		rot.FromRotationMatrix(rotMatrix);
		lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		Vector3 scale(lTmpVector[0],lTmpVector[1],lTmpVector[2]);
		std::string name(pNode->GetName());
		GameObjectPtr gameObject = boost::make_shared<GameObject>(name,position,rot,scale);
		return gameObject;
	}
}