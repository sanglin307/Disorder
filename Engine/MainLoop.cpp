#include "EngineInclude.h"

namespace Disorder
{
	InitialiseSingleton(MainLoop);
 
	void MainLoop::Init()
	{
		GameTime::Init();
		GEngine->Init();
	}

	void MainLoop::CalculateTimeDelta()
	{
		static double lastTime = GameTime::Seconds() -0.001; 
		double currentTime	= GameTime::Seconds();
		float  delta		= (float)(currentTime - lastTime);

		BOOST_ASSERT(delta >= 0);
 
		float maxTickRate	= GEngine->GetMaxTickRate( delta );
		float waitTime		= 0.f;
		 
		if( maxTickRate > 0 )
		{
			waitTime = Max(1.f/maxTickRate-delta, 0.f );
		}
 
		if( waitTime > 0 )
		{
			sysSleep(waitTime);
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