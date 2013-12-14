#ifndef _DISORDER_WINFILESYSTEM_H_
#define _DISORDER_WINFILESYSTEM_H_



namespace Disorder
{
	class WinFileSystem : public FileSystem
	{
	public:
 
		virtual FileObjectPtr OpenFile(std::string const& fileName,std::string const& fileFlag);
		virtual void WriteFile(FileObjectPtr const& fileHandler,std::string const& content);
		virtual std::string ReadFile(FileObjectPtr const& fileHandler);
		virtual void Flush(FileObjectPtr const& fileHandler);

		static WinFileSystemPtr Create();

	private:
		WinFileSystem(){}
	};
}

#endif