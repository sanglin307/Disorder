#ifndef _DISORDER_CAMERA_H_
#define _DISORDER_CAMERA_H_


namespace Disorder
{
	class Camera : public InputListener,public Component
	{
	public:
 
		void LookAt(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec);
	    void ProjCalculate(float FOV,  float nearPlane,float farPlane);

		bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);

		virtual void Tick(float delta);

		Matrix4 ViewMatrix;
		Matrix4 ProjectMatrix;

		void DrawAxis();

		void UpdateShaderProperty();

		static CameraPtr Create(std::string const& name);
	private:

		Camera(std::string const& name);

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

		ShaderPropertyManagerPtr _propertyManager;
		ShaderPropertyPtr _viewMatrixProperty;
		ShaderPropertyPtr _projMatrixProperty;
		ShaderPropertyPtr _positionProperty;


	};
}

#endif