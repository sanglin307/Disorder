#ifndef _DISORDER_SCENEIMPORTER_H_
#define _DISORDER_SCENEIMPORTER_H_

#include <fbxsdk.h>

namespace Disorder
{
	class SceneImporter
	{
	public:
		virtual LevelPtr Import(std::string const& fileName) = 0;
		virtual void Init() = 0;
		virtual void Exit() = 0;
	 
	};

	class FbxSceneImporter : public SceneImporter
	{
	public:
		virtual LevelPtr Import(std::string const& fileName);	
		virtual void Init();
		virtual void Exit();

	private:

		void ProcessGlobalSetting(FbxScene* lscene,LevelPtr const& level);
	 
		void ProcessHierarchy(FbxScene* lscene,LevelPtr const& level);
		void ProcessHierarchy(FbxNode* node,GameObjectPtr const& gameObject);
		GameObjectPtr ProcessTranform(FbxNode* node);


		FbxManager* _sdkManager;
     
	};
}


#endif