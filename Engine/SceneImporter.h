#ifndef _DISORDER_SCENEIMPORTER_H_
#define _DISORDER_SCENEIMPORTER_H_

#include <fbxsdk.h>

namespace Disorder
{
	class SceneImporter
	{
	public:
		virtual void Import(std::string const& fileName) = 0;
		virtual void Init() = 0;
		virtual void Exit() = 0;
	 
	};

	class FbxSceneImporter : public SceneImporter
	{
	public:
		virtual void Import(std::string const& fileName);	
		virtual void Init();
		virtual void Exit();

	private:
		bool LoadScene(std::string const& fileName);

		FbxManager* _sdkManager;
     
	};
}


#endif