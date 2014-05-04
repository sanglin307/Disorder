#include "EngineInclude.h"
 

namespace Disorder
{
	InitialiseSingleton(Logger);

	void Logger::Init()
	{
		_errorFile = GEngine->FileManager->OpenTextFile(GConfig->sLogPath + "Error.log",eF_Write);
		_warningFile = GEngine->FileManager->OpenTextFile(GConfig->sLogPath + "Warning.log", eF_Write);
		_infoFile = GEngine->FileManager->OpenTextFile(GConfig->sLogPath + "Info.log", eF_Write);

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
			_errorFile->Write(iter->content);
			_errorFile->Write("\n");
			iter++;
		}
		_errorCache.clear();
		_errorFile->Flush();

		boost::mutex::scoped_lock warninglock(_warningMutex);
		iter = _warningCache.begin();
		while(iter != _warningCache.end())
		{
			_warningFile->Write(iter->content);
			_warningFile->Write("\n");
			iter++;
		}
		_warningCache.clear();
		_warningFile->Flush();

		boost::mutex::scoped_lock infolock(_infoMutex);
		iter = _infoCache.begin();
		while(iter != _infoCache.end())
		{
			_infoFile->Write(iter->content);
			_infoFile->Write("\n");
			iter++;
		}
		_infoCache.clear();
		_infoFile->Flush();

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