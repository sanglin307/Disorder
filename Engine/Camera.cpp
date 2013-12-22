#include "EngineInclude.h"
 

namespace Disorder
{
 
	CameraSphereTargetUpdatePtr CameraSphereTargetUpdate::Create(float fRadius,const Vector3& target)
	{
		CameraSphereTargetUpdate *pTarget = new CameraSphereTargetUpdate(fRadius,target);
		return CameraSphereTargetUpdatePtr(pTarget);
	}

	bool CameraSphereTargetUpdate::KeyboardEvent(Camera *pCamera,OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds)
	{
		return true;
	}

    bool CameraSphereTargetUpdate::MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
		if( inputManager->IsKeyDown(OIS::KC_LMENU) )
		{
			if( mouseEvent.buttonDown(OIS::MB_Right) )
			{
				if( mouseEvent.RelativeX != 0 )
				{
					Vector3 moveDelta = pCamera->_xAxis * pCamera->_moveSpeed * deltaSeconds * (float)mouseEvent.RelativeX;
					pCamera->_eyePos += moveDelta;
					_target += moveDelta;
					pCamera->_viewMatrixInvalid = true;
				}

				if( mouseEvent.RelativeY != 0 )
				{
					Vector3 moveDelta = pCamera->_upVec * pCamera->_moveSpeed * deltaSeconds * (float)mouseEvent.RelativeY;
					pCamera->_eyePos -= moveDelta;
					_target -= moveDelta;
					pCamera->_viewMatrixInvalid = true;
				}
			}
			else if( mouseEvent.RelativeX != 0 || mouseEvent.RelativeY != 0 || mouseEvent.RelativeZ != 0)
			{
				float yAngle = 0.f;
				float zAngle = 0.f;

				Math::ConvertToSphericalCoord(pCamera->_eyePos,_target,_radius,yAngle,zAngle);

				if( mouseEvent.RelativeX != 0 && mouseEvent.buttonDown(OIS::MB_Left) )
				{				
					zAngle -= deltaSeconds * pCamera->_rotateSpeed * mouseEvent.RelativeX;	  
				}

				if( mouseEvent.RelativeY != 0 && mouseEvent.buttonDown(OIS::MB_Left) )
				{
					yAngle -= deltaSeconds * pCamera->_rotateSpeed * mouseEvent.RelativeY;	  	    
				}

				if( mouseEvent.RelativeZ != 0 )
				{
					_radius -= mouseEvent.RelativeZ * deltaSeconds;
				}

				if( _radius < 5 )
					_radius = 5;
				if( _radius > 100 )
					_radius = 100;

				float limit = 3.0f;
				limit = Math::PI / 180 * limit;
				if( yAngle < limit )
					yAngle = limit;

				if( yAngle > Math::PI - limit )
					yAngle = Math::PI - limit;


				Vector3 finalPos;
				Math::ConvertFromSphericalCoord(_radius,yAngle,zAngle,_target,finalPos);
				Vector3 upVec = Vector3::UNIT_Y;
				Vector3 xVec = upVec.Cross(pCamera->_viewVec);
				xVec.Normalise();
				upVec = pCamera->_viewVec.Cross(xVec);
				upVec.Normalise();
				pCamera->LookAt_(finalPos,_target,upVec);
			}		
		}

		return true;
	}

	void CameraSphereTargetUpdate::SetTarget(Camera *pCamera,const Vector3& target)
	{
		Vector3 viewVec = target - pCamera->_eyePos;
		viewVec.Normalise();
		_target = pCamera->_eyePos + viewVec * _radius ;
	}

	bool CameraSphereTargetUpdate::Update(Camera *pCamera, float deltaSeconds)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	bool CameraFirstPersonUpdate::KeyboardEvent(Camera *pCamera,OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds)
	{
		return true;
	}

	bool CameraFirstPersonUpdate::MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		if( mouseEvent.buttonDown(OIS::MB_Left) )
		{
			if( mouseEvent.RelativeY != 0 )
			{
				float pitch = pCamera->_rotation.GetPitch();
				if( pitch > 90 || pitch < -90 )
					return true;
	 
				Quaternion q(mouseEvent.RelativeY * pCamera->_rotateSpeed * deltaSeconds,Vector3::UNIT_X);
                pCamera->_rotation = pCamera->_rotation * q;
			
                pCamera->_viewMatrixInvalid = true;
 
			}

			if( mouseEvent.RelativeX != 0 )
			{
				//float pitch = pCamera->_rotation.GetPitch();
				//float yaw = pCamera->_rotation.GetYaw();
				//;

				Quaternion q(mouseEvent.RelativeX *  pCamera->_rotateSpeed * deltaSeconds,Vector3::UNIT_Y);
				pCamera->_rotation = pCamera->_rotation * q ;
			
				// std::stringstream stream;
				// stream << " old:" << pitch << ":" << yaw <<":" << roll << "new:"<< pCamera->_rotation.GetPitch() <<":" <<pCamera->_rotation.GetYaw() <<":" << pCamera->_rotation.GetRoll();
				// GLogger->Info(stream.str());
                 pCamera->_viewMatrixInvalid = true;
			}
 

		}
		return true;
	}

	bool CameraFirstPersonUpdate::Update(Camera *pCamera, float deltaSeconds)
	{
		const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
		if( inputManager->IsKeyDown(OIS::KC_W) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->_viewVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_S) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->_viewVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_A) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->_xAxis * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_D) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->_xAxis * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_Q) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->_upVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_viewMatrixInvalid = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_E) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->_upVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_viewMatrixInvalid = true;
		}

		return true;
	}
		 
	CameraFirstPersonUpdatePtr CameraFirstPersonUpdate::Create()
	{
		CameraFirstPersonUpdate *pObject = new CameraFirstPersonUpdate;
		return CameraFirstPersonUpdatePtr(pObject);
	}

	Camera::Camera(std::string const& name)
		:Component(name,CT_Camera)
	{
		_eyePos = Vector3::ZERO;
		_upVec =  Vector3(0.0f,1.0f,0.0f);

		_viewVec = Vector3(0.0f,0.0f,-1.0f) - _eyePos;
		_viewVec.Normalise();

		_xAxis = _upVec.Cross(_viewVec);
		_xAxis.Normalise();

		_upVec = _viewVec.Cross(_xAxis);
		_upVec.Normalise();

		_rotation.FromAxes(_xAxis,_upVec,_viewVec);

		_nearPlane = GConfig->pCameraConfig->NearClip;
		_farPlane = GConfig->pCameraConfig->FarClip;
		_FOV = GConfig->pCameraConfig->FOV * Math::fDeg2Rad;
		_aspectRatio = GConfig->pRenderConfig->SizeX*1.0f/GConfig->pRenderConfig->SizeY;

		_moveSpeed = GConfig->pCameraConfig->mFreeMode.MoveSpeed;
		_rotateSpeed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;

		_viewMatrixInvalid = true;
		_projectMatrixInvalid = true;

		_updateStrategy = CameraSphereTargetUpdate::Create(10,Vector3::ZERO);
		_updateMode = eSphericalTargetMode;

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
		_propertyManager->UpdateShaderProperty();

	}

	void Camera::UpdateViewMatrix()
	{
		if( !_viewMatrixInvalid )
			return;

		_rotation.ToAxes(_xAxis,_upVec,_viewVec);
			 

		Vector3 yAxis = _viewVec.Cross(_xAxis);
		yAxis.Normalise();
		 
	   /* ViewMatrix = Matrix4(xAxis.x,                   yAxis.x,	                 _viewVec.x,	               0,
							xAxis.y,                   yAxis.y,	                 _viewVec.y,	               0,
							xAxis.z,                   yAxis.z,	                 _viewVec.z,                   0,
							-xAxis.dotProduct(_eyePos),-yAxis.dotProduct(_eyePos), -_viewVec.dotProduct(_eyePos),1);*/
		 
		ViewMatrix = Matrix4(_xAxis.x,           _xAxis.y,	                  _xAxis.z,	       -_xAxis.Dot(_eyePos),
				            yAxis.x,            yAxis.y,	                  yAxis.z,	       -yAxis.Dot(_eyePos),
							_viewVec.x,         _viewVec.y,	              _viewVec.z,      -_viewVec.Dot(_eyePos),
							0,                  0,                          0,               1 );

 
		CameraFrustrum.Construct(ViewMatrix,ProjectMatrix);
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
 
		CameraFrustrum.Construct(ViewMatrix,ProjectMatrix);
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
		if( state == IS_Release )
		{
		   if( key == OIS::KC_P )
			   SetUpdateStrategy(eFirstPersonMode);
		   else if( key == OIS::KC_O )
			   SetUpdateStrategy(eSphericalTargetMode);
		}

		if( _updateStrategy != NULL )
			return _updateStrategy->KeyboardEvent(this,key,text,state,deltaSeconds);

		return true;
	}

	bool Camera::MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		if( _updateStrategy != NULL )
			return _updateStrategy->MouseEvent(this,mouseEvent,deltaSeconds);
 
		return true;
	}

	void Camera::SetUpdateStrategy(ECameraUpdateStrategy mode)
	{
		if( _updateMode == mode )
			return;

		_updateMode = mode;
		if( _updateMode == eFirstPersonMode )
		{
			_updateStrategy = CameraFirstPersonUpdate::Create();
		}
		else if( _updateMode == eSphericalTargetMode )
		{
			_updateStrategy = CameraSphereTargetUpdate::Create(10,_eyePos + _viewVec * 10);
		}

	}

	void Camera::DrawAxis()
	{

	}

	void Camera::Update(float delta)
	{
 
		if( GSceneManager->GetDefaultCamera().get() != this )
			return;
 
		if(_updateStrategy != NULL )
		{
			_updateStrategy->Update(this,delta);
		}

		// debug draw
		std::stringstream strstream;
		Vector3 lookAt = _eyePos + _viewVec * 50;
		float roll = _rotation.GetRoll() * Math::fRad2Deg;
		strstream << "camera: [eyePos](" << _eyePos.x << "; " << _eyePos.y << "; " << _eyePos.z << ")     [Focus At](" << lookAt.x << ";  "<< lookAt.y << ";  " << lookAt.z << ")" << roll;
		if( _updateMode == eFirstPersonMode )
		{
			GEngine->GameCanvas->DrawString(Vector2(5.0f,GConfig->pRenderConfig->SizeY - 40.f),30,Vector4::ONE,"First Person Mode");
		}
		else if( _updateMode == eSphericalTargetMode )
		{
			GEngine->GameCanvas->DrawString(Vector2(5.0f,GConfig->pRenderConfig->SizeY - 40.f),30,Vector4::ONE,"Spherical Coordinate Mode");
		}
		GEngine->GameCanvas->DrawString(Vector2(5.0f,GConfig->pRenderConfig->SizeY - 20.f),30,Vector4::ONE,strstream.str());
	}

	void Camera::LookAt(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec)
	{
		 LookAt_(eyePos,lookAt,upVec);	
		 _updateStrategy->SetTarget(this,lookAt);
	}

	void Camera::LookAt_(Vector3 const& eyePos,Vector3 const& lookAt,Vector3 const& upVec)
	{
		 _eyePos = eyePos;
		 _upVec  = upVec;

		 _viewVec = lookAt - eyePos;
		 _viewVec.Normalise();
 
		 _xAxis = _upVec.Cross(_viewVec);
		 _xAxis.Normalise();

		 _upVec = _viewVec.Cross(_xAxis);
		 _upVec.Normalise();

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