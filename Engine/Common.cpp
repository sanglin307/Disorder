#include "EngineInclude.h"
 

namespace Disorder
{
	bool GIsRequestingExit = false;
	unsigned long GFrameCounter = 0;
	unsigned int  GDrawTriNumber = 0;
 
	void sysSleep(float deltaseconds)
	{
#ifdef WIN32
			::Sleep((unsigned long)(deltaseconds * 1000.0));
#endif
	}

	void DebugOutput(const char* expr,const char* file,int line,const char* message)
	{
		std::stringstream stream;
		stream << "Assert "<< expr << "hit! at file " << file << " and line " << line<< " message:"<< message; 
		GLogger->Error(stream.str());
	}
}