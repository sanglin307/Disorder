#include "EngineInclude.h"

namespace Disorder
{
	FileObject::FileObject(std::string const& fileName, FileAccessFlag access, bool binaryFile, bool append)
	{
		std::string flag;
		if (append)
		{
			if (binaryFile)
			{
				if (access == eF_Write)
					flag = "ab";
				else
					flag = "ab+";
			}
			else
			{
				if (access == eF_Write)
					flag = "at";
				else
					flag = "at+";
			}
		}
		else  
		{
			if (binaryFile)
			{
				if (access == eF_Read)
					flag = "rb";
				else if (access == eF_Write)
					flag = "wb";
				else
					flag = "wb+";
			}
			else
			{
				if (access == eF_Read)
					flag = "rt";
				else if (access == eF_Write)
					flag = "wt";
				else
					flag = "wt+";
			}
		}
		 
		FILE *fp = NULL;
		if (fopen_s(&fp, fileName.c_str(), flag.c_str()))
		{
			BOOST_ASSERT(0);
		}

		FileName = fileName;
		_handler = fp;
		AccessFlag = access;
		BinaryFile = binaryFile;
	}

	FileObject::~FileObject()
	{
		Close();
	}

	void FileObject::Close()
	{
		if (_handler != NULL)
		{
			fclose(_handler);
			_handler = NULL;
		}
	}

	void FileObject::Write(std::string const& content)
	{
		BOOST_ASSERT(_handler);
		if (!_handler)
			return;

		fputs(content.c_str(), _handler);
	}

	std::string FileObject::ReadText()
	{
		BOOST_ASSERT(_handler);
		if (!_handler)
			return "";

		char buff[65535];
		std::string result;
		while (fgets(buff, 65535, _handler) != NULL)
		{
			result += buff;
		}

		return result;
	}

	void FileObject::Flush()
	{
		if (!_handler)
			return;

		BOOST_ASSERT(_handler);
		fflush(_handler);
	}

	FileObject* FileSystem::OpenTextFile(std::string const& fileName, FileAccessFlag access, bool append)
	{
		return new FileObject(fileName,access,false,append);
	 
	}

	FileObject* FileSystem::OpenBinaryFile(std::string const& fileName, FileAccessFlag access, bool append)
	{
		return new FileObject(fileName, access, true, append);
		 
	}

	 
	
}