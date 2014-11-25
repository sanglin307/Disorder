#ifndef _DISORDER_CONFIG_H_
#define _DISORDER_CONFIG_H_

namespace Disorder
{
	struct CameraConfig
	{
		float FOV;
		float NearClip;
		float FarClip;

		struct FreeMode
		{
			float MoveSpeed;
			float RotateSpeed;
		};

		FreeMode mFreeMode;
	};

	struct RenderConfig
	{
		RenderEngineType RenderEngine;
		bool	FullScreen;
		int		X;
		int		Y;
		int		SizeX;
		int		SizeY;
		PixelFormat ColorFormat;
		PixelFormat DepthStencilFormat;
		unsigned int MultiSampleCount;
		unsigned int MultiSampleQuality;
		unsigned int SyncInterval;
		unsigned int MotionFrames;
		bool HDR;
		bool Gamma;
		int ShadowMapSize;
		
		bool FXAA;

		RenderConfig::RenderConfig()
		{
			RenderEngine = RET_DirectX;
			FullScreen = false;
			X = 0;
			Y = 0;
			SizeX = 640;
			SizeY = 480;
			ColorFormat = PF_R8G8B8A8_UNORM;
			DepthStencilFormat = PF_D24_UNORM_S8_UINT;
			MultiSampleCount = 1;
			MultiSampleQuality = 0;
			SyncInterval = 0;
			MotionFrames = 0;
			HDR = false;
			Gamma = true;
			FXAA = true;
			ShadowMapSize = 1024;
			
		};
	};

	struct SceneConfig
	{
		std::string LevelName;
		float LineRadius;
		SceneConfig::SceneConfig()
		{
			LevelName = "";
			LineRadius = 0.5f;
		}
	};
 
	class Config 
	{
	public:
		Config(const std::string& rRootPath);
		~Config();

		bool Load();
		bool Save();

		std::string RootPath; 
		std::string LogPath;
		std::string ConfigPath;
		std::string ResourceFBXPath;
		std::string ResourceFXPath;
		std::string ResourceTexPath;
		std::string ResourceFontPath;


		RenderConfig* pRenderConfig; 
		CameraConfig* pCameraConfig;
		SceneConfig* pSceneConfig;
		

	private:

		bool LoadRenderConfig();
		bool LoadCameraConfig();
		bool LoadSceneConfig();
		bool SaveSceneConfig();
		bool SaveRenderConfig();
		bool SaveCameraConfig();


	};

	extern Config* GConfig;
}

#endif