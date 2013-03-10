#include "EngineInclude.h"

namespace Disorder
{

	double GameTime::SecondsPerCycle = 0.0;
	double GameTime::StartTime = 0.0;
	double GameTime::FrameDelta = 0.0;

 
	void GameTime::Init()
	{
#ifdef WIN32
		LARGE_INTEGER Frequency;
	    QueryPerformanceFrequency(&Frequency);
	    SecondsPerCycle = 1.0 / Frequency.QuadPart;
	    StartTime = GameTime::Seconds();
#endif
	}
}