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
		if( key == OIS::KC_LMENU )
		{
			const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
			if( state == IS_Press )
			{
				_zoomPos = inputManager->GetMousePosWheel();
			}
			else if( state == IS_Release )
			{
				_zoomPos = -1;
			}

			return true;
		}

		return false;
	}

    bool CameraSphereTargetUpdate::MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
		if( inputManager->IsKeyDown( OIS::KC_LMENU ) )
		{
			if( mouseEvent.buttonDown(OIS::MB_Right) )
			{
				if( mouseEvent.RelativeX != 0 )
				{
					Vector3 moveDelta = pCamera->_xAxis * pCamera->_moveSpeed * deltaSeconds * (float)mouseEvent.RelativeX;
					pCamera->_eyePos += moveDelta;
					_target += moveDelta;
					pCamera->_InvalidViewMatrix = true;
				}

				if( mouseEvent.RelativeY != 0 )
				{
					Vector3 moveDelta = pCamera->_upVec * pCamera->_moveSpeed * deltaSeconds * (float)mouseEvent.RelativeY;
					pCamera->_eyePos += moveDelta;
					_target += moveDelta;
					pCamera->_InvalidViewMatrix = true;
				}

				return true;
			}
			else if(mouseEvent.RelativeX != 0 || mouseEvent.RelativeY != 0 || _zoomPos != -1)
			{
				float yAngle = 0.f;
				float zAngle = 0.f;

				Math::ConvertToSphericalCoord(pCamera->_eyePos,_target,_radius,yAngle,zAngle);

				if( mouseEvent.RelativeX != 0 && mouseEvent.buttonDown(OIS::MB_Left) )
				{				
					zAngle += deltaSeconds * pCamera->_rotateSpeed * mouseEvent.RelativeX;	  
				}

				if( mouseEvent.RelativeY != 0 && mouseEvent.buttonDown(OIS::MB_Left) )
				{
					yAngle += deltaSeconds * pCamera->_rotateSpeed * mouseEvent.RelativeY;	  	    
				}

				if( _zoomPos != -1 )
				{
					int deltaWheel = mouseEvent.AbsoluteZ - _zoomPos;

					if( deltaWheel != 0 )
					{
						_radius -= deltaWheel * deltaSeconds;
					}

					_zoomPos = mouseEvent.AbsoluteZ;
				}

				if( _radius < 5 )
					_radius = 5;
				if( _radius > 100 )
					_radius = 100;

				float limit = 3.0f;
				limit = limit * Math::fDeg2Rad;

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
				return true;
			}		
	
			
		 
		}
		return false;
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
		return false;
	}

	bool CameraFirstPersonUpdate::MouseEvent(Camera *pCamera,MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		if( mouseEvent.buttonDown(OIS::MB_Left) )
		{
			//float pitch = pCamera->_rotation.GetPitch(false);
		  //  float yaw = pCamera->_rotation.GetYaw(false);
			//float roll = pCamera->_rotation.GetRoll(false);

			if( mouseEvent.RelativeX != 0 )
			{
		//		yaw += mouseEvent.RelativeX * pCamera->_rotateSpeed * deltaSeconds;
			}

			if( mouseEvent.RelativeY != 0 )
			{
		//		pitch += mouseEvent.RelativeY *  pCamera->_rotateSpeed * deltaSeconds;    
				/*if( pitch > Math::HALF_PI )
					pitch = Math::HALF_PI;
				if( pitch < -Math::HALF_PI )
					pitch = -Math::HALF_PI;*/

			}

			if( mouseEvent.RelativeY != 0 || mouseEvent.RelativeX != 0 )
			{
			/*	Matrix3 testMat;
				pCamera->_rotation.ToRotationMatrix(testMat);
				testMat = testMat.Transpose();
				Matrix3 rotMat;
				rotMat.FromEulerAnglesXYZ(pitch,yaw,roll);
				pCamera->_rotation.FromRotationMatrix(rotMat);

				pitch = pCamera->_rotation.GetPitch();
				yaw = pCamera->_rotation.GetYaw();
				roll = pCamera->_rotation.GetRoll();
				pCamera->_InvalidViewMatrix = true;*/
			}

			return true;
		}

		return false;
	}

	bool CameraFirstPersonUpdate::Update(Camera *pCamera, float deltaSeconds)
	{
		const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
		if( inputManager->IsKeyDown(OIS::KC_W) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->_viewVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_S) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->_viewVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_A) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->_xAxis * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_D) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->_xAxis * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_Q) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->_upVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_E) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->_upVec * pCamera->_moveSpeed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
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

		Eigen::Matrix3f rotMat;
		rotMat.col(0) = Eigen::Vector3f(_xAxis.x,_xAxis.y,_xAxis.z);
		rotMat.col(1) = Eigen::Vector3f(_upVec.x,_upVec.y,_upVec.z);
		rotMat.col(2) = Eigen::Vector3f(_viewVec.x,_viewVec.y,_viewVec.z);

		_rotation = Eigen::Quaternionf(rotMat);

		_nearPlane = GConfig->pCameraConfig->NearClip;
		_farPlane = GConfig->pCameraConfig->FarClip;
		_FOV = GConfig->pCameraConfig->FOV * Math::fDeg2Rad;
		_aspectRatio = GConfig->pRenderConfig->SizeX*1.0f/GConfig->pRenderConfig->SizeY;

		_moveSpeed = GConfig->pCameraConfig->mFreeMode.MoveSpeed;
		_rotateSpeed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;

		_InvalidViewMatrix = true;
		_InvalidProjMatrix = true;

		SetUpdateStrategy(eSphericalTargetMode);

		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerCamera);
		_viewMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraView,eSP_Float,16);
		_projMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraProjection,eSP_Float,16);
		_viewProjMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraViewProj,eSP_Float,16);
		_viewInvProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraViewInv,eSP_Float,16);
		_projInvProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraProjInv,eSP_Float,16);
		_viewProjInvProperty =  _propertyManager->CreateProperty(ShaderPropertyManager::sCameraViewProjInv,eSP_Float,16);
		_positionProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sCameraPosition,eSP_Float,3);
	 
	}

	void Camera::UpdateShaderProperty()
	{
		_viewMatrixProperty->SetData(ViewMatrix.data());
		_projMatrixProperty->SetData(ProjMatrix.data());
		_viewProjMatrixProperty->SetData(ViewProjMatrix.data());
		_viewInvProperty->SetData(ViewInvMatrix.data());
		_projInvProperty->SetData(ProjInvMatrix.data());
		_viewProjInvProperty->SetData(ViewProjInvMatrix.data());
		_positionProperty->SetData(_eyePos.Ptr());
		_propertyManager->UpdateShaderProperty();

	}

	void Camera::UpdateViewMatrix()
	{
		if( !_InvalidViewMatrix )
			return;

		_rotation.ToAxes(_xAxis,_upVec,_viewVec);

		ViewMatrix = Math::ViewMatrixRH(_eyePos,_eyePos + _viewVec,_upVec);
		ViewInvMatrix = ViewMatrix.inverse().matrix();
		ViewProjMatrix =  ProjMatrix * ViewMatrix.matrix();
		ViewProjInvMatrix = ViewInvMatrix * ProjInvMatrix;
		CameraFrustrum.Construct(ViewMatrix.matrix(),ProjMatrix);
		_InvalidViewMatrix = false;

	}

	void Camera::UpdateProjectMatrix()
	{
		if( !_InvalidProjMatrix )
			return;
 
		ProjMatrix = Math::ProjFovRH(_FOV,_aspectRatio,_nearPlane,_farPlane);

		//GEngine->RenderEngine->AdjustProjMatrix(ProjMatrix);
		ProjInvMatrix = ProjMatrix.inverse();
		ViewProjMatrix =  ProjMatrix * ViewMatrix.matrix();
		ViewProjInvMatrix = ViewInvMatrix * ProjInvMatrix;

		CameraFrustrum.Construct(ViewMatrix.matrix(),ProjMatrix);
		_InvalidProjMatrix = false;

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
		if( _updateStrategy != NULL )
			return _updateStrategy->KeyboardEvent(this,key,text,state,deltaSeconds);

		return false;
	}

	bool Camera::MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		if( _updateStrategy != NULL )
			return _updateStrategy->MouseEvent(this,mouseEvent,deltaSeconds);
 
		return false;
	}

	void Camera::ToggleUpdateStratety()
	{
		if( _updateMode == eFirstPersonMode )
		{
			SetUpdateStrategy(eSphericalTargetMode);
		}
		else if( _updateMode == eSphericalTargetMode )
		{
			SetUpdateStrategy(eFirstPersonMode);
		}
	}

	void Camera::SetUpdateStrategy(ECameraUpdateStrategy mode)
	{
 
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

	void Camera::DebugDraw()
	{
		std::stringstream strstream;
		Vector3 lookAt = _eyePos + _viewVec * 50;
		float roll = _rotation.GetRoll() * Math::fRad2Deg;
		strstream << "camera: [eyePos](" << (int)_eyePos.x << "; " <<  (int)_eyePos.y << "; " <<  (int)_eyePos.z << ")     [Focus At](" <<  (int)lookAt.x << ";  "<<  (int)lookAt.y << ";  " <<  (int)lookAt.z << ")" <<  (int)roll;
		if( _updateMode == eFirstPersonMode )
		{
			GEngine->GameCanvas->DrawString(0.005f,0.945f,0.04f,Vector4::ONE,"First Person Mode");
		}
		else if( _updateMode == eSphericalTargetMode )
		{
			GEngine->GameCanvas->DrawString(0.005f,0.945f,0.04f,Vector4::ONE,"Spherical Coordinate Mode");
		}
		GEngine->GameCanvas->DrawString(0.005f,0.965f,0.04f,Vector4::ONE,strstream.str());
 
	}

	void Camera::Update(float delta)
	{
 
		if( GSceneManager->GetDefaultCamera().get() != this )
			return;
 
		if(_updateStrategy != NULL )
		{
			_updateStrategy->Update(this,delta);
		}
		
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

		 _InvalidViewMatrix = true;
	}

	void Camera::ProjCalculate(float FOV,  float nearPlane,float farPlane)
	{
		_FOV		 = FOV * Math::fDeg2Rad;
 
		_nearPlane	 = nearPlane;
		_farPlane	 = farPlane;

		_InvalidProjMatrix = true;
	}		 
}