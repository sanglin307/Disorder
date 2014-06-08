#ifndef _DISORDER_SCENEMANAGER_H_
#define _DISORDER_SCENEMANAGER_H_



namespace Disorder
{
	class SceneManager : public Singleton<SceneManager>
	{	
		friend class Singleton<SceneManager>;

		typedef boost::unordered_map<std::string, GeometryRendererPtr>  RendererMap;
		typedef boost::unordered_map<std::string,LightPtr> LightMap;
		typedef boost::unordered_map<std::string,CameraPtr> CameraMap;

	public:
	 
		void Init();
		void Exit();
	    void Tick(float deltaSeconds);

		void AddRenderer(GeometryRendererPtr const& renderer);
		void AddLight(LightPtr const& light);
		void AddCamera(CameraPtr const& camera);

		GeometryRendererPtr GetRenderer(std::string const& name);
		LightPtr GetLight(std::string const& name);
		CameraPtr GetCamera(std::string const& name);

		void CreateDefaultLight();

		void CreateDefaultCamera();
		const CameraPtr& GetDefaultCamera() const
		{
			return _mDefaultCamera;
		}

		void SetDefaultCamera(CameraPtr const& camera);
 
		// get render list according camera
		void GetRendererList(CameraPtr const camera, std::vector<GeometryRendererPtr>& renderObjList) const;
		void GetRendererList(std::vector<GeometryRendererPtr>& renderObjList) const;

		const std::vector<LightPtr>& GetLightsList() const
		{
			return _vLightList;
		}

		void SetAmbientColor(glm::vec3 const& lowerColor, glm::vec3 const& upperColor)
		{
			_vAmbientLowerColor = lowerColor;
			_vAmbientUpperColor = upperColor;
		}

		void UpdateShaderProperty();
		void UpdateBoundingBox();
		void DebugDraw();

		const BoxBounds& GetSceneBoundingBox() const
		{
			return _sceneBounds;
		}

		const SkyboxPtr& GetSkybox() const
		{
			return _skyBox;
		}

		bool EnableDebugDraw;

	protected:
		static SceneManagerPtr Create();
		SceneManager(){};

		RendererMap _mRenderObjects;
        LightMap    _mLightObjects;	
		CameraMap   _mCameraObjects;
		CameraPtr   _mDefaultCamera;
		BoxBounds   _sceneBounds;
		SkyboxPtr   _skyBox;

		std::vector<LightPtr> _vLightList;
		std::vector<GeometryRendererPtr> _vRenderObjects;

		SceneImporterPtr _sceneImporter;

		glm::vec3 _vAmbientLowerColor;
		glm::vec3 _vAmbientUpperColor;

		ShaderPropertyPtr _sAmbientLowerProperty;
		ShaderPropertyPtr _sAmbientUpperProperty;

		ShaderPropertyPtr _sScreenWidthProperty;
		ShaderPropertyPtr _sScreenHeightProperty;
		ShaderPropertyPtr _sShadowMapSizeProperty;
	 

		ShaderPropertyManagerPtr _propertyManager;

	};

	#define GSceneManager SceneManager::GetSingleton()
}





#endif