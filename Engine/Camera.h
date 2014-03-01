#ifndef _DISORDER_CAMERA_H_
#define _DISORDER_CAMERA_H_


namespace Disorder
{
	enum ECameraUpdateStrategy
	{
		eFirstPersonMode,
		eSphericalTargetMode,
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
		virtual bool MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds);
		virtual bool Update(Camera *pCamera, float deltaSeconds);
		 
		static CameraFirstPersonUpdatePtr Create();
	protected:
		CameraFirstPersonUpdate(){};

	};

	class CameraSphereTargetUpdate : public CameraUpdateStrategy
	{
	public:
		virtual bool KeyboardEvent(Camera *pCamera,OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		virtual bool MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds);
		virtual bool Update(Camera *pCamera, float deltaSeconds);
		 
		static CameraSphereTargetUpdatePtr Create(float fRadius,const glm::vec3& target);
		
		virtual void SetTarget(Camera *pCamera,const glm::vec3& target);

	protected:
		CameraSphereTargetUpdate(float radius,const glm::vec3& target)
			:_radius(radius),_target(target),_zoomPos(-1)
		{};

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

		static CameraPtr Create(std::string const& name);
	private:

		Camera(std::string const& name);

		void LookAt_(const glm::vec3& eyePos,const glm::vec3& lookAt,const glm::vec3& upVec);

		void Update(float delta);
		void UpdateViewMatrix();
		void UpdateProjectMatrix();
 
		glm::vec3 EyePos;
		glm::quat Rotation;

		glm::vec3 _Direction;
		glm::vec3 _Up;
		glm::vec3 _Right;
 
		float _nearPlane;
		float _farPlane;
		float _FOV;
		float _aspectRatio;

		bool _InvalidViewMatrix;
		bool _InvalidProjMatrix;

		ECameraUpdateStrategy _updateMode;
		CameraUpdateStrategyPtr _updateStrategy;

		ShaderPropertyManagerPtr _propertyManager;
		ShaderPropertyPtr _viewMatrixProperty;
		ShaderPropertyPtr _projMatrixProperty;
		ShaderPropertyPtr _viewProjMatrixProperty;
		ShaderPropertyPtr _viewInvProperty;
		ShaderPropertyPtr _projInvProperty;
		ShaderPropertyPtr _viewProjInvProperty;
		ShaderPropertyPtr _positionProperty;

	};
}

#endif