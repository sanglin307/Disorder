#ifndef _DISORDER_SCENEMANAGER_H_
#define _DISORDER_SCENEMANAGER_H_



namespace Disorder
{
	class SceneManager : public Singleton<SceneManager>
	{	
		friend class Singleton<SceneManager>;

		typedef boost::unordered_map<std::string,RendererPtr>  RendererMap;
		typedef boost::unordered_map<std::string,LightPtr> LightMap;
		typedef boost::unordered_map<std::string,CameraPtr> CameraMap;

	public:
		void Init();
		void Exit();
	    void Tick(float deltaSeconds);

		void AddRenderer(RendererPtr const& renderer);
		void AddLight(LightPtr const& light);
		void AddCamera(CameraPtr const& camera);

		RendererPtr GetRenderer(std::string const& name);
		LightPtr GetLight(std::string const& name);
		CameraPtr GetCamera(std::string const& name);

		void CreateDefaultCamera();
		const CameraPtr& GetDefaultCamera() const
		{
			return _mDefaultCamera;
		}

		void SetDefaultCamera(CameraPtr const& camera);

		void UpdateLight();
		const std::vector<RendererPtr>& GetRendererList(CameraPtr const camera) const
		{
			return _vRenderObjects;
		}

		void SetAmbientColor(Vector3 const& lowerColor,Vector3 const& upperColor);
		void UpdateShaderProperty();
		

	protected:
		static SceneManagerPtr Create();
		SceneManager(){};

		RendererMap _mRenderObjects;
        LightMap    _mLightObjects;	
		CameraMap   _mCameraObjects;
		CameraPtr   _mDefaultCamera;

		std::vector<LightPtr> _vDirectionalLights;
		std::vector<LightPtr> _vNonDirectionalLights;
		std::vector<RendererPtr> _vRenderObjects;

		SceneImporterPtr _sceneImporter;

		Vector3 _vAmbientLowerColor;
		Vector3 _vAmbientUpperColor;

		ShaderPropertyPtr _sAmbientLowerProperty;
		ShaderPropertyPtr _sAmbientUpperProperty;
		ShaderPropertyManagerPtr _propertyManager;

	};

	#define GSceneManager SceneManager::GetSingleton()
}





#endif