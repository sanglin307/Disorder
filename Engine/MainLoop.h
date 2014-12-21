#ifndef _DISORDER_MAINLOOP_H_
#define _DISORDER_MAINLOOP_H_


namespace Disorder
{
	class MainLoop
	{ 
	public:
		MainLoop();
		~MainLoop();
		void Tick();
		void CalculateTimeDelta();
	};

	
}


#endif