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

		virtual void SetTarget(Camera *pCamera,const Eigen::Vector3f& target)
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
		 
		static CameraSphereTargetUpdatePtr Create(float fRadius,const Eigen::Vector3f& target);
		
		virtual void SetTarget(Camera *pCamera,const Eigen::Vector3f& target);

	protected:
		CameraSphereTargetUpdate(float radius,const Eigen::Vector3f& target)
			:_radius(radius),_target(target),_zoomPos(-1)
		{};

		float _radius;
		Eigen::Vector3f _target;
		int _zoomPos;
	};

	class Camera : public InputListener,public Component
	{
		friend class CameraUpdateStrategy;
		friend class CameraSphereTargetUpdate;
		friend class CameraFirstPersonUpdate;
	public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		mutable Eigen::Affine3f ViewMatrix;
		mutable Eigen::Matrix4f ProjMatrix;
		mutable Eigen::Matrix4f ViewProjMatrix;

		mutable Eigen::Matrix4f ViewInvMatrix;
		mutable Eigen::Matrix4f ProjInvMatrix;
		mutable Eigen::Matrix4f ViewProjInvMatrix;

		void LookAt(Eigen::Vector3f const& eyePos,Eigen::Vector3f const& lookAt,Eigen::Vector3f const& upVec);
	    void ProjCalculate(float FOV, float nearPlane,float farPlane);

		bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);

		Eigen::Vector3f Direction() const;
		Eigen::Vector3f Up() const;
		Eigen::Vector3f Right() const;

		virtual void Tick(float delta);
 
		Frustrum CameraFrustrum;

		void SetUpdateStrategy(ECameraUpdateStrategy mode);
		void ToggleUpdateStratety();

		void DebugDraw();

		void UpdateShaderProperty();

		static CameraPtr Create(std::string const& name);
	private:

		Camera(std::string const& name);

		void LookAt_(Eigen::Vector3f const& eyePos,Eigen::Vector3f const& lookAt,Eigen::Vector3f const& upVec);

		void Update(float delta);
		void UpdateViewMatrix();
		void UpdateProjectMatrix();
 
		Eigen::Vector3f _eyePos;
		Eigen::Quaternionf _rotation;
 
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