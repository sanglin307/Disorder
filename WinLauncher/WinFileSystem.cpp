#include "WinLauncher.h"


namespace Disorder
{
	FileObjectPtr WinFileSystem::OpenFile(std::string const& fileName,int flag)
	{
		FileObjectPtr filePtr = FileObject::Create();
		filePtr->fileName = fileName;
		filePtr->fileHandler.open(fileName.c_str(),flag);
		return filePtr;
	}

	WinFileSystemPtr WinFileSystem::Create()
	{
		WinFileSystem *pSystem = new WinFileSystem;
		return WinFileSystemPtr(pSystem);
	}

	void WinFileSystem::WriteFile(FileObjectPtr const& fileHandler,std::string const& content)
	{
		fileHandler->fileHandler << content << std::endl;
	}

	std::string WinFileSystem::ReadFile(FileObjectPtr const& fileHandler)
	{
		std::ostringstream tmp; 
        tmp << fileHandler->fileHandler.rdbuf(); 
        std::string content = tmp.str();
		return content;
	}
}