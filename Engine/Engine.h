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
	};


	class Engine : public Singleton<Engine>
	{
	public:
		void Init();
		void Exit();

		float GetMaxTickRate(float delta) { return 200.f;}
		void Tick(float deltaSeconds);


		Engine();
		~Engine();

	public :
 
		ClientPtr GameClient;
		CanvasPtr GameCanvas;

		RenderEnginePtr RenderEngine;

	    EngineStat Stat;

		FileSystemPtr FileManager;

	};

#define GEngine Engine::GetSingleton()

}



#endif