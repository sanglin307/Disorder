#include "WinLauncher.h"


namespace Disorder
{
	FileObjectPtr WinFileSystem::OpenFile(std::string const& fileName,std::string const& fileFlag)
	{ 
		FileObjectPtr filePtr = FileObject::Create(fileName,fileFlag);
		return filePtr;
	}

	WinFileSystemPtr WinFileSystem::Create()
	{
		WinFileSystem *pSystem = new WinFileSystem;
		return WinFileSystemPtr(pSystem);
	}

	void WinFileSystem::WriteFile(FileObjectPtr const& fileHandler,std::string const& content)
	{
		BOOST_ASSERT(fileHandler != NULL && fileHandler->FileHandler != NULL );
		fputs(content.c_str(),fileHandler->FileHandler);
	}

	std::string WinFileSystem::ReadFile(FileObjectPtr const& fileHandler)
	{
		BOOST_ASSERT(fileHandler != NULL && fileHandler->FileHandler != NULL );

		char buff[65535];
		std::string result;
		while(fgets(buff,65535,fileHandler->FileHandler) != NULL )
		{
			result += buff;
		}
	 
		return result;
	}
}