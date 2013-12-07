#ifndef _DISORDER_FILESYSTEM_H_
#define _DISORDER_FILESYSTEM_H_

namespace Disorder
{

	class FileObject
	{
	public:
		std::string FileName;
		FILE *FileHandler;
		std::string FileFlag;

	    static FileObjectPtr Create(std::string const& fileName,std::string const& fileFlag);

		~FileObject();

	private:
		FileObject(){}
		
	};

	class FileSystem
	{
	public:
		virtual FileObjectPtr OpenFile(std::string const& fileName,std::string const& fileFlag) = 0;
		virtual void WriteFile(FileObjectPtr const& fileHandler,std::string const& content) = 0;
		virtual std::string ReadFile(FileObjectPtr const& fileHandler) = 0;


	};


	
}

#endif