#ifndef _DISORDER_ENGINE_H_
#define _DISORDER_ENGINE_H_


namespace Disorder
{
	class Client;

	class EngineStat
	{
	public:

		unsigned long FrameCounter;
		unsigned int  DrawTriNumber;

		void OnFrameBegin()
		{
			_lastFrameDrawTriNumber = DrawTriNumber;
			DrawTriNumber = 0;
		}

		void OnFrameEnd()
		{
		}

		EngineStat()
		{
			_bDrawEnable = true;	
			FrameCounter = 0;
			DrawTriNumber = 0;
			_lastFrameDrawTriNumber = 0;
		}

		void OnOff()
		{
			_bDrawEnable = !_bDrawEnable;
		}

		bool bDrawEnable()
		{
			return _bDrawEnable;
		}

		void DrawStat();

		void Tick(float deltaSeconds);

	private:
		bool _bDrawEnable;
		int _lastFrameDrawTriNumber;
		 
	};


	class Engine : public Singleton<Engine>
	{
		friend class Singleton<Engine>;

	public:
		void Init();
		void Exit();

		int GetMaxTickRate(float delta) { return 0;}
		void Tick(float deltaSeconds);

		~Engine();

	public :
 
		ClientPtr GameClient;
		CanvasPtr GameCanvas;
		ConsolePtr GameConsole;
		RenderEnginePtr RenderEngine;
		SceneImporterPtr SceneImporter;
		RenderResourceManagerPtr RenderResourceMgr;
		RenderSurfaceCachePtr RenderSurfaceCache;

	    EngineStat Stat;

		FileSystemPtr FileManager;

	private:
		static EnginePtr Create();
	    Engine();

	};

#define GEngine Engine::GetSingleton()

}



#endif