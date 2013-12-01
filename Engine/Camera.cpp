#include "EngineInclude.h"


namespace Disorder
{
 

	Camera::Camera(std::string const& name)
		:Component(name,CT_Camera)
	{
		_eyePos = Vector3(0.0f,1.0f,-10.0f);
		_upVec =  Vector3(0.0f,1.0f,0.0f);

		_viewVec = Vector3(0.0f,1.0f,0.0f) - _eyePos;
		_viewVec.Normalise();

		_xAxis = _upVec.Cross(_viewVec);
		_xAxis.Normalise();

		_rotation.FromAxes(_xAxis,_upVec,_viewVec);

		_nearPlane = GConfig->pCameraConfig->NearClip;
		_farPlane = GConfig->pCameraConfig->FarClip;
		_FOV = GConfig->pCameraConfig->FOV * Math::fDeg2Rad;
		_aspectRatio = GConfig->pRenderConfig->SizeX*1.0f/GConfig->pRenderConfig->SizeY;

		_moveSpeed = GConfig->pCameraConfig->mFreeMode.MoveSpeed;
		_rotateSpeed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;

		_viewMatrixInvalid = true;
		_projectMatrixInvalid = true;

		_propertyManager = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerCamera);
		_viewMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraView,eSP_Matrix4);
		_projMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraProjection,eSP_Matrix4);
		_positionProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraPosition,eSP_Vector3);
	 
	}

	void Camera::UpdateShaderProperty()
	{
		_viewMatrixProperty->SetData(ViewMatrix);
		_projMatrixProperty->SetData(ProjectMatrix);
		_positionProperty->SetData(_eyePos);
	}

	void Camera::UpdateViewMatrix()
	{
		if( !_viewMatrixInvalid )
			return;

		_rotation.ToAxes(_xAxis,_upVec,_viewVec);
			 

		Vector3 yAxis = _viewVec.Cross(_xAxis);
		//yAxis.Normalise();
		 
	   /* ViewMatrix = Matrix4(xAxis.x,                   yAxis.x,	                 _viewVec.x,	               0,
							xAxis.y,                   yAxis.y,	                 _viewVec.y,	               0,
							xAxis.z,                   yAxis.z,	                 _viewVec.z,                   0,
							-xAxis.dotProduct(_eyePos),-yAxis.dotProduct(_eyePos), -_viewVec.dotProduct(_eyePos),1);*/
		 
		ViewMatrix = Matrix4(_xAxis.x,           _xAxis.y,	                  _xAxis.z,	       -_xAxis.Dot(_eyePos),
				            yAxis.x,            yAxis.y,	                  yAxis.z,	       -yAxis.Dot(_eyePos),
							_viewVec.x,         _viewVec.y,	              _viewVec.z,      -_viewVec.Dot(_eyePos),
							0,                  0,                          0,               1 );

 
		_viewMatrixInvalid = false;

	}

	void Camera::UpdateProjectMatrix()
	{
		if( !_projectMatrixInvalid )
			return;

		float h = 1.0f / Math::Tan(_FOV / 2);
		float w = h / _aspectRatio;
		float q = _farPlane / (_farPlane - _nearPlane);

		/*ProjectMatrix = Matrix4(
				w,		0,		0,				0,
				0,		h,		0,				0,
				0,		0,		q,				1,
				0,		0,		-nearPlane * q, 0 );*/

		ProjectMatrix = Matrix4(
				w,		0,		0,				0,
				0,		h,		0,				0,
				0,		0,		q,				-_nearPlane * q,
				0,		0,		1,              0 );

		GEngine->RenderEngine->AdjustProjMatrix(ProjectMatrix);
 

		_projectMatrixInvalid = false;

	}

    void Camera::Tick(float delta)
	{ 

		//Update Camera 
		Update(delta);

		UpdateViewMatrix();

		UpdateProjectMatrix();

	}

	CameraPtr Camera::Create(std::string const& name)
	{
		Camera *pCamera = new Camera(name);
		return CameraPtr(pCamera);
	}

	bool Camera::KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds)
	{
 
		return true;
	}

	bool Camera::MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		if( mouseEvent.buttonDown(OIS::MB_Left) )
		{
			if( mouseEvent.RelativeX != 0 )
			{
				Quaternion q(mouseEvent.RelativeX * _rotateSpeed * deltaSeconds,Vector3::UNIT_Y);
				_rotation = _rotation * q;
			    _viewMatrixInvalid = true;
			}

			if( mouseEvent.RelativeY != 0 )
			{
				Quaternion q(mouseEvent.RelativeY * _rotateSpeed * deltaSeconds,Vector3::UNIT_X);
				_rotation = _rotation * q;
			    _viewMatrixInvalid = true;
			}

		}
		else if( mouseEvent.buttonDown(OIS::MB_Right) )
		{
			if( mouseEvent.RelativeX != 0 )
			{
				Quaternion q(mouseEvent.RelativeX * _rotateSpeed * deltaSeconds,Vector3::UNIT_Z);
				_rotation = _rotation * q;
			    _viewMatrixInvalid = true;
			}

			if( mouseEvent.RelativeY != 0 )
			{
				Quaternion q(mouseEvent.RelativeY * _rotateSpeed * deltaSeconds,Vector3::UNIT_Z);
				_rotation = _rotation * q;
			    _viewMatrixInvalid = true;
			}
 
		}

		return true;
	}

	void Camera::DrawAxis()
	{

	}

	void Camera::Update(float delta)
	{
 
		if( GSceneManager->GetDefaultCamera().get() != this )
			return;
 
		const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
		if( inputManager->IsKeyDown(OIS::KC_W) )
		{
			_eyePos = _eyePos + _viewVec * _moveSpeed * delta;
			_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_S) )
		{
			_eyePos = _eyePos - _viewVec * _moveSpeed * delta;
			_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_A) )
		{
			_eyePos = _eyePos - _xAxis * _moveSpeed * delta;
			_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_D) )
		{
			_eyePos = _eyePos + _xAxis * _moveSpeed * delta;
			_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_Q) )
		{
			_eyePos = _eyePos + _upVec * _moveSpeed * delta;
			_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_E) )
		{
			_eyePos = _eyePos - _upVec * _moveSpeed * delta;
			_viewMatrixInvalid = true;
		}

		// debug draw
		std::stringstream strstream;
		strstream << "camera: [eyePos](" << _eyePos.x << "; " << _eyePos.y << "; " << _eyePos.z << ")     [Focus At](" << _viewVec.x << ";  "<< _viewVec.y << ";  " << _viewVec.z << ")";
		GEngine->GameCanvas->DrawString(Vector3(5.0f,GConfig->pRenderConfig->SizeY - 20.f,0.f),30,Vector4::ONE,strstream.str());
	}

	void Camera::LookAt(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec)
	{
		 _eyePos = eyePos;
		 _upVec  = upVec;

		 _viewVec = lookAt - eyePos;
		 _viewVec.Normalise();
 
		 _xAxis = _upVec.Cross(_viewVec);
		 _xAxis.Normalise();

		 _rotation.FromAxes(_xAxis,_upVec,_viewVec);

		 _viewMatrixInvalid = true;
	}

	void Camera::ProjCalculate(float FOV,  float nearPlane,float farPlane)
	{
		_FOV		 = FOV * Math::fDeg2Rad;
 
		_nearPlane	 = nearPlane;
		_farPlane	 = farPlane;

		_projectMatrixInvalid = true;
	}		 
}