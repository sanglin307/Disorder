#include "EngineInclude.h"
 

namespace Disorder
{
	InitialiseSingleton(Logger);

	void Logger::Init()
	{
		_errorFile = GEngine->FileManager->OpenFile("..\\Log\\Error.log","wt");
		_warningFile = GEngine->FileManager->OpenFile("..\\Log\\Warning.log","wt");
		_infoFile = GEngine->FileManager->OpenFile("..\\Log\\Info.log","wt");
		_thread =  boost::thread(LoggerRunner());
	}

	void Logger::Exit()
	{
		_thread.join();
	}

	void Logger::Info(std::string const& info)
	{
		boost::mutex::scoped_lock lock(_infoMutex);
		LogObj log;
		strncpy_s(log.content,info.c_str(),Min<unsigned int>(info.size(), LogObj::StringMaxLengh));
		_infoCache.push_back(log);

	}

	LoggerPtr Logger::Create()
	{
		Logger *logger = new Logger;
		return LoggerPtr(logger);
	}

	void Logger::Warning(std::string const& warning)
	{
		boost::mutex::scoped_lock lock(_warningMutex);
		LogObj log;
		unsigned int length = Min<unsigned int>(warning.size() , LogObj::StringMaxLengh);
		strncpy_s(log.content,warning.c_str(),length);
		_warningCache.push_back(log);
	}

	void Logger::Error(std::string const& error)
	{
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
			GEngine->FileManager->WriteFile(_errorFile,"\n");
			iter++;
		}
		_errorCache.clear();
		GEngine->FileManager->Flush(_errorFile);

		boost::mutex::scoped_lock warninglock(_warningMutex);
		iter = _warningCache.begin();
		while(iter != _warningCache.end())
		{
			GEngine->FileManager->WriteFile(_warningFile,iter->content);
			GEngine->FileManager->WriteFile(_warningFile,"\n");
			iter++;
		}
		_warningCache.clear();
		GEngine->FileManager->Flush(_warningFile);

		boost::mutex::scoped_lock infolock(_infoMutex);
		iter = _infoCache.begin();
		while(iter != _infoCache.end())
		{
			GEngine->FileManager->WriteFile(_infoFile,iter->content);
			GEngine->FileManager->WriteFile(_infoFile,"\n");
			iter++;
		}
		_infoCache.clear();
		GEngine->FileManager->Flush(_infoFile);

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