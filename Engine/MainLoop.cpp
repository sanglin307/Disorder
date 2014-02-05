#include "EngineInclude.h"

namespace Disorder
{
	InitialiseSingleton(MainLoop);
 
	void MainLoop::Init()
	{
		GameTime::Init();
		GEngine->Init();
	}

	MainLoopPtr MainLoop::Create()
	{
		MainLoop *pLoop = new MainLoop;
		return MainLoopPtr(pLoop);
	}

	void MainLoop::CalculateTimeDelta()
	{
		static double lastTime = GameTime::Seconds() -0.001; 
		double currentTime	= GameTime::Seconds();
		float  delta		= (float)(currentTime - lastTime);

		BOOST_ASSERT(delta >= 0);
 
		int maxTickRate	= GEngine->GetMaxTickRate( delta );
		double waitTime		= 0;
		 
		if( maxTickRate > 0 )
		{
			waitTime = Max(1.0/maxTickRate-delta, 0.0 );
		}
 
		if( waitTime > 0 )
		{
			sysSleep((float)waitTime);
		}
 
		GameTime::FrameDelta = currentTime - lastTime;
		lastTime = currentTime;
	}

	void MainLoop::Tick()
	{	 
		CalculateTimeDelta();
		GEngine->Tick((float)GameTime::FrameDelta);
		GFrameCounter++;
	}

	void MainLoop::Exit()
	{
		GEngine->Exit();
	}

}