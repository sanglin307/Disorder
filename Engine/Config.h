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

		SceneConfig::SceneConfig()
		{
			LevelName = "";
		}
	};
 
	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;

	public:
		
		~Config();

		bool Load();
		bool Save();

		static std::string sRootPath; 
		static std::string sLogPath;
		static std::string sConfigPath;
		static std::string sResourceFBXPath;
		static std::string sResourceFXPath;
		static std::string sResourceTexPath;
		static std::string sResourceFontPath;


		RenderConfig* pRenderConfig; 
		CameraConfig* pCameraConfig;
		SceneConfig* pSceneConfig;
		

	private:
		static ConfigPtr Create();
		Config();

		bool LoadRenderConfig();
		bool LoadCameraConfig();
		bool LoadSceneConfig();
		bool SaveSceneConfig();
		bool SaveRenderConfig();
		bool SaveCameraConfig();


	};

	#define GConfig Config::GetSingleton()
}

#endif