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
		bool	FullScreen;
		int		X;
		int		Y;
		int		SizeX;
		int		SizeY;
		PixelFormat ColorFormat;
		PixelFormat DepthStencilFormat;
		unsigned int SampleCount;
		unsigned int SampleQuality;
		unsigned int SyncInterval;
		unsigned int MotionFrames;
		bool HDR;
		bool Gamma;

		RenderConfig::RenderConfig()
		{
			FullScreen = false;
			X = 0;
			Y = 0;
			SizeX = 640;
			SizeY = 480;
			ColorFormat = PF_R10G10B10A2;//PF_R8G8B8A8;
			DepthStencilFormat = PF_D24S8;
			SampleCount = 0;
			SampleQuality = 0;
			SyncInterval = 0;
			MotionFrames = 0;
			HDR = false;
			Gamma = true;
		};
	};

 
	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;

	public:
		
		~Config();

		bool Load();
		bool Save();

		static std::string sRootPath; 
		static std::string sConfigPath;
		static std::string sResourceFBXPath;
		static std::string sResourceFXPath;
		static std::string sResourceTexPath;
		static std::string sResourceFontPath;


		RenderConfig* pRenderConfig; 
		CameraConfig* pCameraConfig;

		

	private:
		static ConfigPtr Create();
		Config();

		bool LoadRenderConfig();
		bool LoadCameraConfig();
		bool SaveRenderConfig();
		bool SaveCameraConfig();


	};

	#define GConfig Config::GetSingleton()
}

#endif