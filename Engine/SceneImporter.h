#ifndef _DISORDER_SCENEIMPORTER_H_
#define _DISORDER_SCENEIMPORTER_H_

#include <fbxsdk.h>
#include "Level.h"

namespace Disorder
{
	class SceneImporter
	{
	public:
		virtual Level* Import(std::string const& fileName) = 0;
		virtual void Init() = 0;
		virtual void Exit() = 0;
	 
	};

	class FbxSceneImporter : public SceneImporter
	{
	public:
		virtual Level* Import(std::string const& fileName);	
		virtual void Init();
		virtual void Exit();
 
	private:
		 
		void PreProcessGlobalSetting(FbxScene* lscene,Level* level);
		void PostProcessGlobalSetting(FbxScene* lscene, Level* level);
	 
		void ProcessHierarchy(FbxScene* lscene,Level* level);
		void ProcessHierarchy(FbxNode* pNode,GameObject* gameObject);
		void ProcessContent(FbxNode* pNode,GameObject* gameObject);
		void ProcessMesh(FbxNode* pNode,GameObject* gameObject);
		void ProcessTextures(FbxSurfaceMaterial *pMaterial, SurfaceMaterial* materials);
		void ProcessFileTexture(const char* propertyName,FbxFileTexture* pTexture,SurfaceMaterial* materials);
		void ProcessLayerTexture(const char* propertyName, FbxLayeredTexture* pTexture, SurfaceMaterial* materials);
		void ProcessProceduralTexture(const char* propertyName, FbxProceduralTexture* pTexture, SurfaceMaterial* materials);
		void ProcessGeometry(FbxMesh *pMesh,Geometry* geometry);
		void ProcessMaterials(FbxMesh *pMesh,std::vector<SurfaceMaterial*> & materials,int &usedMaterial);
		void ProcessLight(FbxNode* pNode,GameObject* gameObject);
		void ProcessCamera(FbxNode* pNode,GameObject* gameObject);

		GameObject* ProcessTranform(FbxNode* pNode);


		FbxManager* _sdkManager;
		std::string _cachedCurrentScene;
     
	};
}


#endif