#ifndef _DISORDER_CAMERA_H_
#define _DISORDER_CAMERA_H_


namespace Disorder
{
	enum ECameraUpdateStrategy
	{
		eFirstPersonMode,
		eSphericalTargetMode,
	};

	enum ECameraProjectMode
	{
		ePerspectiveCamera,
		eOrthoCamera,
	};

	class CameraUpdateStrategy
	{
	public:

		virtual bool KeyboardEvent(Camera *pCamera ,OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds)
		{
			return false;
		}

		virtual bool MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds)
		{
			return false;
		}

		virtual bool Update(Camera *pCamera, float deltaSeconds)
		{
			return true;
		}

		virtual void SetTarget(Camera *pCamera,const glm::vec3& target)
		{
			return;
		}

		
	protected:
		CameraUpdateStrategy(){};
 

	};

	class CameraFirstPersonUpdate : public CameraUpdateStrategy
	{
	public:
		virtual bool KeyboardEvent(Camera *pCamera,OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		virtual bool MouseEvent(Camera *pCamera, MouseInputEvent const& mouseEvent, float deltaSeconds);
		virtual bool Update(Camera *pCamera, float deltaSeconds);

	};

	class CameraSphereTargetUpdate : public CameraUpdateStrategy
	{
	public:
		virtual bool KeyboardEvent(Camera *pCamera, OIS::KeyCode key, unsigned int text, InputState state, float deltaSeconds);
		virtual bool MouseEvent(Camera *pCamera, MouseInputEvent const& mouseEvent, float deltaSeconds);
		virtual bool Update(Camera *pCamera, float deltaSeconds);
		 
		CameraSphereTargetUpdate(float radius, const glm::vec3& target)
			:_radius(radius), _target(target), _zoomPos(-1)
		{};
		
		virtual void SetTarget(Camera *pCamera, const glm::vec3& target);
		
		const glm::vec3& GetTarget() const
		{
			return _target;
		}

	protected:
		float _radius;
		glm::vec3 _target;
		int _zoomPos;
	};

	class Camera : public InputListener,public Component
	{
		friend class CameraUpdateStrategy;
		friend class CameraSphereTargetUpdate;
		friend class CameraFirstPersonUpdate;
	public:
    
		glm::mat4 ViewMatrix;
		glm::mat4 ProjMatrix;
		glm::mat4 ViewProjMatrix;

		glm::mat4 ViewInvMatrix;
		glm::mat4 ProjInvMatrix;
		glm::mat4 ViewProjInvMatrix;
 
		ECameraProjectMode ProjectMode;

		void LookAt(const glm::vec3& eyePos,const glm::vec3& lookAt,const glm::vec3& upVec);
	    void ProjCalculate(float FOV, float nearPlane,float farPlane);

		bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);

		const glm::vec3& Direction() const
		{
			return _Direction;
		}

		const glm::vec3& Up() const
		{
			return _Up;
		}

		const glm::vec3& Right() const
		{
			return _Right;
		}

		virtual void Tick(float delta);
 
		Frustrum CameraFrustrum;

		void SetUpdateStrategy(ECameraUpdateStrategy mode);
		void ToggleUpdateStratety();

		void DebugDraw();

		void UpdateShaderProperty();
		Camera(std::string const& name);
		~Camera();

		void UpdateViewProjMatrix(const glm::mat4& viewMat, const glm::mat4& projMat);
		
	private:
		void LookAt_(const glm::vec3& eyePos,const glm::vec3& lookAt,const glm::vec3& upVec);

		void Update(float delta);
		void UpdateViewMatrix();
		void UpdateProjectMatrix();
 
		glm::vec3 EyePos;
		glm::quat Rotation;

		glm::vec3 _Direction; // right-hand 
		glm::vec3 _Up;
		glm::vec3 _Right;
 
		float _nearPlane;
		float _farPlane;
		float _FOV;
		float _aspectRatio;

		bool _InvalidViewMatrix;
		bool _InvalidProjMatrix;

		ECameraUpdateStrategy _updateMode;
		CameraUpdateStrategy* _updateStrategy;

		ShaderPropertyManager* _propertyManager;
		ShaderProperty* _viewMatrixProperty;
		ShaderProperty* _projMatrixProperty;
		ShaderProperty* _viewProjMatrixProperty;
		ShaderProperty* _viewInvProperty;
		ShaderProperty* _projInvProperty;
		ShaderProperty* _viewProjInvProperty;
		ShaderProperty* _positionProperty;

	};
}

#endif