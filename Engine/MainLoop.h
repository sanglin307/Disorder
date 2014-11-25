#ifndef _DISORDER_MAINLOOP_H_
#define _DISORDER_MAINLOOP_H_


namespace Disorder
{
	class MainLoop
	{ 
	public:
		 
		void Init();
		void Exit();	 
		void Tick();
		void CalculateTimeDelta();
	};

	extern MainLoop* GMainLoop;
}


#endif