#ifndef _DISORDER_FILESYSTEM_H_
#define _DISORDER_FILESYSTEM_H_

namespace Disorder
{
	enum FileAccessFlag
	{
		eF_Read,
		eF_Write,
		eF_ReadWrite
	};

	class FileObject
	{
	public:
		std::string FileName;
	
		FileAccessFlag AccessFlag;
		bool BinaryFile;

		static FileObjectPtr Create(std::string const& fileName, FileAccessFlag access, bool binaryFile = false,bool append = false);

		void Write(std::string const& content);
		std::string ReadText();
		void Flush();
		void Close();

		~FileObject();

	private:

		FILE *_handler;
		FileObject(){}
		
	};

	class FileSystem
	{
	public:
		FileObjectPtr OpenTextFile(std::string const& fileName, FileAccessFlag access, bool append = false);
		FileObjectPtr OpenBinaryFile(std::string const& fileName, FileAccessFlag access, bool append = false);

	
		static FileSystemPtr Create();

	protected:
		FileSystem(){}
	};


	
}

#endif