#ifndef _DISORDER_ENGINE_H_
#define _DISORDER_ENGINE_H_


namespace Disorder
{
	class Client;

	class EngineStat
	{
	public:

		EngineStat()
		{
			_bEnable = true;
			_iDrawSize = 32;
		}

		void OnOff()
		{
			_bEnable = !_bEnable;
		}

		bool bEnable()
		{
			return _bEnable;
		}

		void DrawStat();

		void Tick(float deltaSeconds);

	private:
		bool _bEnable;
		int _iDrawSize;
	};


	class Engine : public Singleton<Engine>
	{
		friend class Singleton<Engine>;

	public:
		void Init();
		void Exit();

		float GetMaxTickRate(float delta) { return 200.f;}
		void Tick(float deltaSeconds);

		
	
		~Engine();

	public :
 
		ClientPtr GameClient;
		CanvasPtr GameCanvas;
		ConsolePtr GameConsole;
		RenderEnginePtr RenderEngine;
		SceneImporterPtr SceneImporter;
		RenderResourceManagerPtr RenderResManager;

	    EngineStat Stat;

		FileSystemPtr FileManager;

	private:
		static EnginePtr Create();
	    Engine();

	};

#define GEngine Engine::GetSingleton()

}



#endif