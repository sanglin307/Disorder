#ifndef _DISORDER_SCENEMANAGER_H_
#define _DISORDER_SCENEMANAGER_H_



namespace Disorder
{
	class SceneManager  
	{	
		typedef boost::unordered_map<std::string, GeometryRenderer*>  RendererMap;
		typedef boost::unordered_map<std::string,Light*> LightMap;
		typedef boost::unordered_map<std::string,Camera*> CameraMap;

	public:
	 
		void Init();
		void Exit();
	    void Tick(float deltaSeconds);

		void AddRenderer(GeometryRenderer* renderer);
		void AddLight(Light* light);
		void AddCamera(Camera*  camera);

		GeometryRenderer* GetRenderer(std::string const& name);
		Light* GetLight(std::string const& name);
		Camera* GetCamera(std::string const& name);

		void CreateDefaultLight();

		void CreateDefaultCamera();
		Camera* GetDefaultCamera()
		{
			return _mDefaultCamera;
		}

		void SetDefaultCamera(Camera* camera);
 
		// get render list according camera
		void GetRendererList(Camera* camera, std::vector<GeometryRenderer*>& renderObjList) const;
		void GetRendererList(std::vector<GeometryRenderer*>& renderObjList) const;

		const std::vector<Light*>& GetLightsList() const
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

		Skybox* GetSkybox() const
		{
			return _skyBox;
		}

		bool EnableDebugDraw;

	protected:
 
		RendererMap _mRenderObjects;
        LightMap    _mLightObjects;	
		CameraMap   _mCameraObjects;
		Camera*   _mDefaultCamera;
		BoxBounds   _sceneBounds;
		Skybox*   _skyBox;

		std::vector<Light*> _vLightList;
		std::vector<GeometryRenderer*> _vRenderObjects;

		SceneImporter* _sceneImporter;

		glm::vec3 _vAmbientLowerColor;
		glm::vec3 _vAmbientUpperColor;

		ShaderProperty* _sAmbientLowerProperty;
		ShaderProperty* _sAmbientUpperProperty;

		ShaderProperty* _sScreenWidthProperty;
		ShaderProperty* _sScreenHeightProperty;
		ShaderProperty* _sShadowMapSizeProperty;
		ShaderProperty* _sLineRadius;

		ShaderPropertyManager* _propertyManager;

	};

	
}





#endif