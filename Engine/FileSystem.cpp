#include "EngineInclude.h"

namespace Disorder
{
	FileObjectPtr FileObject::Create(std::string const& fileName,std::string const& fileFlag)
	{
		FILE *fp = fopen(fileName.c_str(),fileFlag.c_str());
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
}