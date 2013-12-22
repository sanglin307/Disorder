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
			return true;
		}

		virtual bool MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds)
		{
			return true;
		}

		virtual bool Update(Camera *pCamera, float deltaSeconds)
		{
			return true;
		}

		virtual void SetTarget(Camera *pCamera,const Vector3& target)
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
		 
		static CameraSphereTargetUpdatePtr Create(float fRadius,const Vector3& target);
		
		virtual void SetTarget(Camera *pCamera,const Vector3& target);

	protected:
		CameraSphereTargetUpdate(float radius,const Vector3& target)
			:_radius(radius),_target(target)
		{};

		float _radius;
		Vector3 _target;
	};

	class Camera : public InputListener,public Component
	{
		friend class CameraUpdateStrategy;
		friend class CameraSphereTargetUpdate;
		friend class CameraFirstPersonUpdate;
	public:
 
		void LookAt(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec);
	    void ProjCalculate(float FOV,  float nearPlane,float farPlane);

		bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);

		virtual void Tick(float delta);

		Matrix4 ViewMatrix;
		Matrix4 ProjectMatrix;
		Frustrum CameraFrustrum;

		void SetUpdateStrategy(ECameraUpdateStrategy mode);
		void DrawAxis();

		void UpdateShaderProperty();

		static CameraPtr Create(std::string const& name);
	private:

		Camera(std::string const& name);

		void LookAt_(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec);

		void Update(float delta);
		void UpdateViewMatrix();
		void UpdateProjectMatrix();

		

		float _moveSpeed;
		float _rotateSpeed;

		Vector3 _eyePos;

		Vector3 _upVec;
        Vector3 _viewVec;
		Vector3 _xAxis;

		Quaternion _rotation;

		float _nearPlane;
		float _farPlane;
		float _FOV;
		float _aspectRatio;

		bool _viewMatrixInvalid;
		bool _projectMatrixInvalid;

		ECameraUpdateStrategy _updateMode;
		CameraUpdateStrategyPtr _updateStrategy;

		ShaderPropertyManagerPtr _propertyManager;
		ShaderPropertyPtr _viewMatrixProperty;
		ShaderPropertyPtr _projMatrixProperty;
		ShaderPropertyPtr _positionProperty;


	};
}

#endif