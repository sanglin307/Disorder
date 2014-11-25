#ifndef _DISORDER_LOGGER_H_
#define _DISORDER_LOGGER_H_

#include <boost\thread.hpp>


namespace Disorder
{
	class LoggerRunner
	{
	public:
		void operator()();

	};



	class Logger 
	{
		friend class LoggerRunner;
	public:

		struct LogObj
		{
			static const unsigned int StringMaxLengh = 8096;
			char content[StringMaxLengh+1];
		};
		
		void Init();
		void Exit();


		void Error(std::string const& error);
		void Warning(std::string const& warning);
		void Info(std::string const& info);
 
	private:
		void Flush();

		boost::thread _thread;

		FileObject* _errorFile;
		FileObject* _warningFile;
		FileObject* _infoFile;

		boost::mutex _errorMutex;
		boost::mutex _warningMutex;
		boost::mutex _infoMutex;

		std::list<LogObj> _errorCache;
		std::list<LogObj> _warningCache;
		std::list<LogObj> _infoCache;


	};

	extern Logger* GLogger;
}



#endif