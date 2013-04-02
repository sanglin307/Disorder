#ifndef _DISORDER_FILESYSTEM_H_
#define _DISORDER_FILESYSTEM_H_

namespace Disorder
{

	class FileObject
	{
	public:
		std::string fileName;
		std::fstream fileHandler;
		int fileFlag;
	};

	class FileSystem
	{
	public:
		virtual FileObjectPtr OpenFile(std::string const& fileName,int flag) = 0;
		virtual void WriteFile(FileObjectPtr const& fileHandler,std::string const& content) = 0;
		virtual std::string ReadFile(FileObjectPtr const& fileHandler) = 0;


	};


	
}

#endif