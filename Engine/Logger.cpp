#include "EngineInclude.h"
#include "Logger.h"

namespace Disorder
{
	InitialiseSingleton(Logger);

	void Logger::Init()
	{
		_thread =  boost::thread(LoggerRunner());
		_errorFile = GEngine->FileManager->OpenFile("..\\Log\\Error.log",std::ios::out|std::ios::trunc);
		_warningFile = GEngine->FileManager->OpenFile("..\\Log\\Warning.log",std::ios::out|std::ios::trunc);
		_infoFile = GEngine->FileManager->OpenFile("..\\Log\\Info.log",std::ios::out|std::ios::trunc);
	}

	void Logger::Exit()
	{
		_thread.join();
		_errorFile->fileHandler.close();
		_warningFile->fileHandler.close();
		_infoFile->fileHandler.close();
	}

	void Logger::Info(std::string const& info)
	{
		boost::mutex::scoped_lock lock(_infoMutex);
		LogObj log;
		strncpy_s(log.content,info.c_str(),Min<unsigned int>(info.size(), LogObj::StringMaxLengh));
		_infoCache.push_back(log);

	}

	void Logger::Warning(std::string const& warning)
	{
		boost::mutex::scoped_lock lock(_warningMutex);
		LogObj log;
		strncpy_s(log.content,warning.c_str(),Min<unsigned int>(warning.size() , LogObj::StringMaxLengh));
		_warningCache.push_back(log);
	}

	void Logger::Error(std::string const& error)
	{
		BOOST_ASSERT(0);
		boost::mutex::scoped_lock lock(_errorMutex);
		LogObj log;
		strncpy_s(log.content,error.c_str(),Min<unsigned int>(error.size() , LogObj::StringMaxLengh));
		_errorCache.push_back(log);
	}

	void Logger::Flush()
	{
		boost::mutex::scoped_lock errorlock(_errorMutex);
		std::list<LogObj>::const_iterator iter = _errorCache.begin();
		while(iter != _errorCache.end())
		{
			GEngine->FileManager->WriteFile(_errorFile,iter->content);
			iter++;
		}
		_errorCache.clear();

		boost::mutex::scoped_lock warninglock(_warningMutex);
		iter = _warningCache.begin();
		while(iter != _warningCache.end())
		{
			GEngine->FileManager->WriteFile(_warningFile,iter->content);
			iter++;
		}
		_warningCache.clear();

		boost::mutex::scoped_lock infolock(_infoMutex);
		iter = _infoCache.begin();
		while(iter != _infoCache.end())
		{
			GEngine->FileManager->WriteFile(_infoFile,iter->content);
			iter++;
		}
		_infoCache.clear();

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