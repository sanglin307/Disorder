#ifndef _DISORDER_SCENEMANAGER_H_
#define _DISORDER_SCENEMANAGER_H_



namespace Disorder
{
	class SceneManager : public Singleton<SceneManager>
	{	
		typedef boost::unordered_map<std::string,RendererPtr>  RendererMap;
		typedef boost::unordered_map<std::string,LightPtr> LightMap;
		typedef boost::unordered_map<std::string,CameraPtr> CameraMap;

	public:
		void Init();
		void Exit();
		void Render();
	    void Tick(float deltaSeconds);
		void AddRenderer(RendererPtr const& renderer);
		void AddLight(LightPtr const& light);
		void AddCamera(CameraPtr const& camera);
		RendererPtr GetRenderer(std::string const& name);
		LightPtr GetLight(std::string const& name);
		CameraPtr GetCamera(std::string const& name);


	private:
		RendererMap _mRenderObjects;
        LightMap    _mLightObjects;
		std::vector<LightPtr> _mDirectLights;
		std::vector<LightPtr> _mNonDirectLights;

		CameraMap   _mCameraObjects;

		SceneImporterPtr _sceneImporter;

	};

	#define GSceneManager SceneManager::GetSingleton()
}





#endif