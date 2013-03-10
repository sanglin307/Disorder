#ifndef _DISORDER_TIME_H_
#define _DISORDER_TIME_H_


namespace Disorder
{
	class GameTime
	{
	public:
	    static double SecondsPerCycle;
		static double StartTime;
	 
		static double FrameDelta;
 
	    static void Init();
		 
		static inline double Seconds()
		{
#ifdef WIN32
			LARGE_INTEGER Cycles;
			QueryPerformanceCounter(&Cycles);
			// Add big number to make bugs apparent where return value is being passed to FLOAT
			return Cycles.QuadPart * SecondsPerCycle + 16777216.0;   
#endif
		}

		static inline LONGLONG Cycles()
		{
#ifdef WIN32
			LARGE_INTEGER Cycles;
			QueryPerformanceCounter(&Cycles);
			return Cycles.QuadPart;
#endif
		}


	};
}



#endif