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
		//EPixelFormat DepthStencilFormat;
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
			ColorFormat = PF_R8G8B8A8;
			//DepthStencilFormat = PF_DepthStencil;
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
	public:
		Config();
		~Config();

		bool Load();
		bool Save();

		static std::string sRootPath; 
		static std::string sConfigPath;
		static std::string sResourceFBXPath;
		static std::string sResourceFXPath;
		static std::string sResourceTexPath;


		RenderConfig* pRenderConfig; 
		CameraConfig* pCameraConfig;

	private:

		bool LoadRenderConfig();
		bool LoadCameraConfig();
		bool SaveRenderConfig();
		bool SaveCameraConfig();


	};

	#define GConfig Config::GetSingleton()
}

#endif