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



	class Logger : public Singleton<Logger>
	{
		friend class LoggerRunner;
		friend class Singleton<Logger>;
	public:

		struct LogObj
		{
			static const unsigned int StringMaxLengh = 1024;
			char content[StringMaxLengh];
		};
		
		void Init();
		void Exit();


		void Error(std::string const& error);
		void Warning(std::string const& warning);
		void Info(std::string const& info);
 
	

	private:
		static LoggerPtr Create();
		Logger(){};

		void Flush();

		boost::thread _thread;

		FileObjectPtr _errorFile;
		FileObjectPtr _warningFile;
		FileObjectPtr _infoFile;

		boost::mutex _errorMutex;
		boost::mutex _warningMutex;
		boost::mutex _infoMutex;

		std::list<LogObj> _errorCache;
		std::list<LogObj> _warningCache;
		std::list<LogObj> _infoCache;


	};

	#define GLogger Logger::GetSingleton()
}



#endif