#include "EngineInclude.h"

#include "boost\property_tree\ptree.hpp"
#include "boost\property_tree\xml_parser.hpp"

namespace Disorder
{
	InitialiseSingleton(Config);

	std::string Config::sRootPath = "";
	std::string Config::sConfigPath = "";
	std::string Config::sLogPath = "";
	std::string Config::sResourceFBXPath = "";
    std::string Config::sResourceFXPath = "";
    std::string Config::sResourceTexPath = "";
	std::string Config::sResourceFontPath = "";


	ConfigPtr Config::Create()
	{
		Config *pConfig = new Config;
		return ConfigPtr(pConfig);
	}

	Config::Config()
	{
		pRenderConfig = new RenderConfig;
		pCameraConfig = new CameraConfig;
	}

	Config::~Config()
	{
		if(pRenderConfig)
		{
			delete pRenderConfig;
			pRenderConfig = 0;
		}

		if( pCameraConfig )
		{
			delete pCameraConfig;
			pCameraConfig = 0;
		}
	}

	
	bool Config::Load()
	{
		bool result = LoadRenderConfig();
		result &= LoadCameraConfig();

		return result;

	}

	bool Config::Save()
	{
		bool result = SaveRenderConfig();
		result &= SaveCameraConfig();

		return result;
	}

	bool Config::LoadRenderConfig()
	{
		BOOST_ASSERT(pRenderConfig);

		std::string renderConfigFile = GConfig->sConfigPath + "RenderConfig.xml";
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

		std::string cameraConfigFile = GConfig->sConfigPath + "CameraConfig.xml";
		boost::property_tree::ptree tree;
		boost::property_tree::read_xml(cameraConfigFile,tree);

		pCameraConfig->NearClip = tree.get("CameraConfig.NearClip",1.0f);
		pCameraConfig->FarClip = tree.get("CameraConfig.FarClip",1000.0f);
		pCameraConfig->FOV = tree.get("CameraConfig.FOV",60.0f);

		pCameraConfig->mFreeMode.MoveSpeed = tree.get("CameraConfig.FreeMode.MoveSpeed",5.0f);
		pCameraConfig->mFreeMode.RotateSpeed = tree.get("CameraConfig.FreeMode.RotateSpeed",5.0f);

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

		std::string renderConfigFile = GConfig->sConfigPath + "RenderConfig.xml";
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

		std::string cameraConfigFile = GConfig->sConfigPath + "CameraConfig.xml";
		boost::property_tree::write_xml(cameraConfigFile,tree);

		return true;
	}

}