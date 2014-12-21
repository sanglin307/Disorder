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

		Logger();
		~Logger();
		
		void Error(std::string const& error);
		void Warning(std::string const& warning);
		void Info(std::string const& info);
 
	private:
		void Flush();

		boost::thread _thread;

		FILE* _errorFile;
		FILE* _warningFile;
		FILE* _infoFile;

		boost::mutex _errorMutex;
		boost::mutex _warningMutex;
		boost::mutex _infoMutex;

		std::list<std::string> _errorCache;
		std::list<std::string> _warningCache;
		std::list<std::string> _infoCache;
	};

}



#endif