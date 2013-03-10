#include "WinLauncher.h"


namespace Disorder
{
	FileObjectPtr WinFileSystem::OpenFile(std::string const& fileName,int flag)
	{
		FileObjectPtr filePtr = boost::make_shared<FileObject>();
		filePtr->fileName = fileName;
		filePtr->fileHandler.open(fileName.c_str(),flag);
		return filePtr;
	}

	void WinFileSystem::WriteFile(FileObjectPtr const& fileHandler,std::string const& content)
	{
		fileHandler->fileHandler << content << std::endl;
	}
}