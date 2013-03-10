#ifndef _DISORDER_SCENEIMPORTER_H_
#define _DISORDER_SCENEIMPORTER_H_

#include <fbxsdk.h>

namespace Disorder
{
	class SceneImporter
	{
	public:
		virtual void Import(std::string const& fileName) = 0;
	 
	};

	class FbxSceneImporter : public SceneImporter
	{
	public:
		virtual void Import(std::string const& fileName);	

	private:

		bool LoadScene(std::string const& fileName);

		void Init();
		void Exit();

		FbxManager* _sdkManager;
        FbxScene* _scene;

	};
}


#endif