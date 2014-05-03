#include "EngineInclude.h"

namespace Disorder
{
	FileObjectPtr FileObject::Create(std::string const& fileName,std::string const& fileFlag)
	{
		FILE *fp = NULL;
		fopen_s(&fp,fileName.c_str(), fileFlag.c_str());
		if( !fp )
		{
			BOOST_ASSERT(0);
			return NULL;
		}

		FileObject *pObject = new FileObject();
		pObject->FileName = fileName;
		pObject->FileHandler = fp;
		pObject->FileFlag = fileFlag;
		return FileObjectPtr(pObject);
	}

	FileObject::~FileObject()
	{
		if( FileHandler != NULL )
		{
			 fclose(FileHandler);
			 FileHandler = NULL;
		}
	}

	FileObjectPtr FileSystem::OpenFile(std::string const& fileName, std::string const& fileFlag)
	{
		FileObjectPtr filePtr = FileObject::Create(fileName, fileFlag);
		return filePtr;
	}

	FileSystemPtr FileSystem::Create()
	{
		FileSystem *pSystem = new FileSystem;
		return FileSystemPtr(pSystem);
	}

	void FileSystem::WriteFile(FileObjectPtr const& fileHandler, std::string const& content)
	{
		BOOST_ASSERT(fileHandler != NULL && fileHandler->FileHandler != NULL);
		fputs(content.c_str(), fileHandler->FileHandler);
	}

	std::string FileSystem::ReadFile(FileObjectPtr const& fileHandler)
	{
		BOOST_ASSERT(fileHandler != NULL && fileHandler->FileHandler != NULL);

		char buff[65535];
		std::string result;
		while (fgets(buff, 65535, fileHandler->FileHandler) != NULL)
		{
			result += buff;
		}

		return result;
	}

	void FileSystem::Flush(FileObjectPtr const& fileHandler)
	{
		fflush(fileHandler->FileHandler);
	}
}