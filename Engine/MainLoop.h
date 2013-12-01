#ifndef _DISORDER_MAINLOOP_H_
#define _DISORDER_MAINLOOP_H_


namespace Disorder
{
	class MainLoop : public Singleton<MainLoop>
	{ 
		friend class Singleton<MainLoop>;

	public:
		 
		void Init();
		void Exit();	 
		void Tick();
		void CalculateTimeDelta();
		
	private:
		static MainLoopPtr Create();
		MainLoop(){};
	};

#define GMainLoop MainLoop::GetSingleton()
}


#endif