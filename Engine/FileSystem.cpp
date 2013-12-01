#include "EngineInclude.h"

namespace Disorder
{
	FileObjectPtr FileObject::Create()
	{
		FileObject *pObject = new FileObject();
		return FileObjectPtr(pObject);
	}
}