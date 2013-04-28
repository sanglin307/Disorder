#ifndef _DISORDER_CAMERA_H_
#define _DISORDER_CAMERA_H_


namespace Disorder
{
	class Camera : public InputListener,public Component
	{
	public:
		Camera();
		
		void LookAt(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec);
	    void ProjCalculate(float FOV, float aspect, float nearPlane,float farPlane);

		bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);

		virtual void Tick(float delta);

		Matrix4 ViewMatrix;
		Matrix4 ProjectMatrix;


	private:

		void Update(float delta);
		
		int _moveSpeed;
		int _rotateSpeed;

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


	};
}

#endif