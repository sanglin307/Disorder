#ifndef _DISORDER_WINFILESYSTEM_H_
#define _DISORDER_WINFILESYSTEM_H_



namespace Disorder
{
	class WinFileSystem : public FileSystem
	{
	public:
 
		virtual FileObjectPtr OpenFile(std::string const& fileName,int flag);
		virtual void WriteFile(FileObjectPtr const& fileHandler,std::string const& content);

	};
}

#endif