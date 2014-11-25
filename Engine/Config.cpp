#include "EngineInclude.h"

#include "boost\property_tree\ptree.hpp"
#include "boost\property_tree\xml_parser.hpp"

namespace Disorder
{
	Config *GConfig = NULL;

	Config::Config(const std::string& rRootPath)
	{
		RootPath = rRootPath;
		ConfigPath = RootPath + "/Config/";
		LogPath = RootPath + "/Log/";
		ResourceFBXPath = RootPath + "/Resource/Fbx/";
		ResourceFXPath = RootPath + "/Resource/FX/";
		ResourceTexPath = RootPath + "/Resource/Texture/";
		ResourceFontPath = RootPath + "/Resource/Font/";
		pRenderConfig = new RenderConfig;
		pCameraConfig = new CameraConfig;
		pSceneConfig = new SceneConfig;
		Load();
	}

	Config::~Config()
	{
		if(pRenderConfig)
		{
			delete pRenderConfig;
			pRenderConfig = NULL;
		}

		if( pCameraConfig )
		{
			delete pCameraConfig;
			pCameraConfig = NULL;
		}

		if (pSceneConfig)
		{
			delete pSceneConfig;
			pSceneConfig = NULL;
		}
	}

	
	bool Config::Load()
	{
		bool result = LoadRenderConfig();
		result &= LoadCameraConfig();
		result &= LoadSceneConfig();

		return result;

	}

	bool Config::Save()
	{
		bool result = SaveRenderConfig();
		result &= SaveCameraConfig();
		result &= SaveSceneConfig();

		return result;
	}

	bool Config::LoadRenderConfig()
	{
		BOOST_ASSERT(pRenderConfig);

		std::string renderConfigFile = GConfig->ConfigPath + "RenderConfig.xml";
		boost::property_tree::ptree tree;
		boost::property_tree::read_xml(renderConfigFile,tree);
		
		std::string renderType = tree.get("RenderConfig.RenderEngine","DirectX");
		boost::to_lower(renderType);
		if( renderType == "directx" )
			pRenderConfig->RenderEngine = RET_DirectX;
		else if( renderType == "opengl" )
			pRenderConfig->RenderEngine = RET_OpenGL;
		else
		{
			GLogger->Error("Unknown Render engine type:" + renderType );
			return false;
		}

		pRenderConfig->FullScreen = tree.get("RenderConfig.FullScreen",0) > 0;
		pRenderConfig->SizeX = tree.get("RenderConfig.ResX",640);
		pRenderConfig->SizeY = tree.get("RenderConfig.ResY",480);
		pRenderConfig->SyncInterval = tree.get("RenderConfig.SyncInterval", 0);
		pRenderConfig->MultiSampleCount = tree.get("RenderConfig.MultiSampleCount", 0);

		return true;
	}

	bool Config::LoadCameraConfig()
	{
		BOOST_ASSERT(pCameraConfig);

		std::string cameraConfigFile = GConfig->ConfigPath + "CameraConfig.xml";
		boost::property_tree::ptree tree;
		boost::property_tree::read_xml(cameraConfigFile,tree);

		pCameraConfig->NearClip = tree.get("CameraConfig.NearClip",1.0f);
		pCameraConfig->FarClip = tree.get("CameraConfig.FarClip",1000.0f);
		pCameraConfig->FOV = tree.get("CameraConfig.FOV",60.0f);

		pCameraConfig->mFreeMode.MoveSpeed = tree.get("CameraConfig.FreeMode.MoveSpeed",5.0f);
		pCameraConfig->mFreeMode.RotateSpeed = tree.get("CameraConfig.FreeMode.RotateSpeed",5.0f);

		return true;
	}

	bool Config::LoadSceneConfig()
	{
		BOOST_ASSERT(pSceneConfig);

		std::string sceneConfigFile = GConfig->ConfigPath + "SceneConfig.xml";
		boost::property_tree::ptree tree;
		boost::property_tree::read_xml(sceneConfigFile, tree);

		pSceneConfig->LevelName = tree.get("SceneConfig.LevelName", "");
		pSceneConfig->LineRadius = tree.get("SceneConfig.LineRadius", 0.5f);
		return true;
	}


	bool Config::SaveRenderConfig()
	{
		BOOST_ASSERT(pRenderConfig);
		boost::property_tree::ptree tree;
		
		if( pRenderConfig->FullScreen )
			tree.put("RenderConfig.FullScreen",1);
		else
			tree.put("RenderConfig.FullScreen",0);

		if( pRenderConfig->RenderEngine == RET_DirectX )
		    tree.put("RenderConfig.RenderEngine","DirectX");
		else if( pRenderConfig->RenderEngine == RET_OpenGL )
			tree.put("RenderConfig.RenderEngine","OpenGL");

		tree.put("RenderConfig.ResX",pRenderConfig->SizeX);
		tree.put("RenderConfig.ResY",pRenderConfig->SizeY);
		tree.put("RenderConfig.SyncInterval", pRenderConfig->SyncInterval);
		tree.put("RenderConfig.MultiSampleCount", pRenderConfig->MultiSampleCount);

		std::string renderConfigFile = GConfig->ConfigPath + "RenderConfig.xml";
		boost::property_tree::write_xml(renderConfigFile,tree);

		return true;
	}

	bool Config::SaveCameraConfig()
	{
		BOOST_ASSERT(pCameraConfig);

		boost::property_tree::ptree tree;
		tree.put("CameraConfig.NearClip",pCameraConfig->NearClip);
		tree.put("CameraConfig.FarClip",pCameraConfig->FarClip);
		tree.put("CameraConfig.FOV",pCameraConfig->FOV);

		tree.put("CameraConfig.FreeMode.MoveSpeed",pCameraConfig->mFreeMode.MoveSpeed);
		tree.put("CameraConfig.FreeMode.RotateSpeed",pCameraConfig->mFreeMode.RotateSpeed);

		std::string cameraConfigFile = GConfig->ConfigPath + "CameraConfig.xml";
		boost::property_tree::write_xml(cameraConfigFile,tree);

		return true;
	}

	bool Config::SaveSceneConfig()
	{
		BOOST_ASSERT(pSceneConfig);

		boost::property_tree::ptree tree;
		tree.put("SceneConfig.LevelName", pSceneConfig->LevelName);
	 
		std::string sceneConfigFile = GConfig->ConfigPath + "SceneConfig.xml";
		boost::property_tree::write_xml(sceneConfigFile, tree);

		return true;
	}

}