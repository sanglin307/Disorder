#include "EngineInclude.h"
 

namespace Disorder
{
	Logger* GLogger = NULL;

	Logger::Logger()
	{
		std::string fileName = GConfig->LogPath + "Error.log";
		if (fopen_s(&_errorFile, fileName.c_str(), "at"))
		{
			BOOST_ASSERT(0);
			return;
		}
		
		fileName = GConfig->LogPath + "Warning.log";
		if (fopen_s(&_warningFile, fileName.c_str(), "at"))
		{
			BOOST_ASSERT(0);
			return;
		}

		fileName = GConfig->LogPath + "Info.log";
		if (fopen_s(&_infoFile, fileName.c_str(), "at"))
		{
			BOOST_ASSERT(0);
			return;
		}
		
		_thread =  boost::thread(LoggerRunner());
	}

	Logger::~Logger()
	{
		_thread.join();
		fclose(_errorFile);
		_errorFile = NULL;
		fclose(_warningFile);
		_warningFile = NULL;
		fclose(_infoFile);
		_infoFile = NULL;
	}

	void Logger::Info(std::string const& info)
	{
		boost::mutex::scoped_lock lock(_infoMutex);
		std::string log = info;
		log += "\n";
		_infoCache.push_back(log);

	}
 
	void Logger::Warning(std::string const& warning)
	{
		boost::mutex::scoped_lock lock(_warningMutex);
		std::string log = warning;
		log += "\n";
		_warningCache.push_back(log);
	}

	void Logger::Error(std::string const& error)
	{
		boost::mutex::scoped_lock lock(_errorMutex);
		std::string err = error;
		err += "\n";
		_errorCache.push_back(err);
	}

	void Logger::Flush()
	{
		boost::mutex::scoped_lock errorlock(_errorMutex);
		std::list<std::string>::const_iterator iter = _errorCache.begin();
		while(iter != _errorCache.end())
		{
			fputs(iter->c_str(), _errorFile);
			iter++;
		}
		_errorCache.clear();
		fflush(_errorFile);

		boost::mutex::scoped_lock warninglock(_warningMutex);
		iter = _warningCache.begin();
		while(iter != _warningCache.end())
		{
			fputs(iter->c_str(), _warningFile);
			iter++;
		}
		_warningCache.clear();
		fflush(_warningFile);

		boost::mutex::scoped_lock infolock(_infoMutex);
		iter = _infoCache.begin();
		while(iter != _infoCache.end())
		{
			fputs(iter->c_str(), _infoFile);
			iter++;
		}
		_infoCache.clear();
		fflush(_infoFile);

	}


	void LoggerRunner::operator()()
	{
		while(!GIsRequestingExit)
		{
			GLogger->Flush();
			sysSleep(0.1f);
		}
	}
}