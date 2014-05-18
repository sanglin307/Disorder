#include "EngineInclude.h"
 

namespace Disorder
{
 
	CameraSphereTargetUpdatePtr CameraSphereTargetUpdate::Create(float fRadius,const glm::vec3& target)
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
				float speed = GConfig->pCameraConfig->mFreeMode.MoveSpeed;
				if( mouseEvent.RelativeX != 0 )
				{
					glm::vec3 moveDelta = pCamera->Right() * speed * deltaSeconds * (float)mouseEvent.RelativeX;
					pCamera->EyePos -= moveDelta;
				
					_target -= moveDelta;
					pCamera->_InvalidViewMatrix = true;
				}

				if( mouseEvent.RelativeY != 0 )
				{
					glm::vec3 moveDelta = pCamera->Up() * speed * deltaSeconds * (float)mouseEvent.RelativeY;
					pCamera->EyePos += moveDelta;
					
					_target += moveDelta;
					pCamera->_InvalidViewMatrix = true;
				}

				return true;
			}
			else if(mouseEvent.RelativeX != 0 || mouseEvent.RelativeY != 0 || _zoomPos != -1)
			{
				float yAngle = 0.f;
				float zAngle = 0.f;

				float speed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;

			

				Math::ConvertToSphericalCoord(pCamera->EyePos,_target,_radius,yAngle,zAngle);
 
				if( mouseEvent.RelativeX != 0 && mouseEvent.buttonDown(OIS::MB_Left) )
				{
					zAngle += deltaSeconds * speed * mouseEvent.RelativeX;	 
				}

				if( mouseEvent.RelativeY != 0 && mouseEvent.buttonDown(OIS::MB_Left) )
				{
					yAngle += deltaSeconds * speed * mouseEvent.RelativeY;	  	    
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
 
				glm::vec3 finalPos;
				Math::ConvertFromSphericalCoord(_radius,yAngle,zAngle,_target,finalPos);
 
				pCamera->LookAt_(finalPos,_target,glm::vec3(0,1,0));
				return true;
			}		
	
			
		 
		}
		return false;
	}

	void CameraSphereTargetUpdate::SetTarget(Camera *pCamera,const glm::vec3& target)
	{
		glm::vec3 viewVec = target - pCamera->EyePos;
		viewVec = glm::normalize(viewVec);
		_target = pCamera->EyePos + viewVec * _radius ;
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
			glm::vec3 euler = Math::EulerAngles(pCamera->ViewMatrix, 0, 1, 2);
 
			float speed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;
			if( mouseEvent.RelativeX != 0 )
			{
				euler.y += mouseEvent.RelativeX * speed * deltaSeconds;
			}

			if( mouseEvent.RelativeY != 0 )
			{
				euler.x += mouseEvent.RelativeY * speed * deltaSeconds;    
			}

			if( mouseEvent.RelativeY != 0 || mouseEvent.RelativeX != 0 )
			{
  
				glm::quat rot = glm::angleAxis(euler.x, glm::vec3(1, 0, 0)) *
					            glm::angleAxis(euler.y, glm::vec3(0, 1, 0)) *
					            glm::angleAxis(euler.z, glm::vec3(0, 0, 1));

				pCamera->Rotation = glm::conjugate(rot);
				pCamera->_InvalidViewMatrix = true;
		        
			}

			return true;
		}

		return false;
	}

	bool CameraFirstPersonUpdate::Update(Camera *pCamera, float deltaSeconds)
	{
		const InputManagerPtr inputManager = GEngine->GameClient->GetInputManager();
		float speed = GConfig->pCameraConfig->mFreeMode.MoveSpeed ;
		if( inputManager->IsKeyDown(OIS::KC_W) )
		{
			pCamera->EyePos = pCamera->EyePos - pCamera->Direction() * speed * deltaSeconds;		 
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_S) )
		{
			pCamera->EyePos = pCamera->EyePos + pCamera->Direction() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_A) )
		{
			pCamera->EyePos = pCamera->EyePos - pCamera->Right() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_D) )
		{
			pCamera->EyePos = pCamera->EyePos + pCamera->Right() * speed * deltaSeconds;			 
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_Q) )
		{
			pCamera->EyePos = pCamera->EyePos + pCamera->Up() * speed * deltaSeconds;			 
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_E) )
		{
			pCamera->EyePos = pCamera->EyePos - pCamera->Up() * speed * deltaSeconds;		 
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

		ProjectMode = eOrthoCamera;
		_nearPlane = GConfig->pCameraConfig->NearClip;
		_farPlane = GConfig->pCameraConfig->FarClip;
		_FOV = GConfig->pCameraConfig->FOV * Math::fDeg2Rad;
		_aspectRatio = GConfig->pRenderConfig->SizeX*1.0f/GConfig->pRenderConfig->SizeY;

		LookAt_(glm::vec3(0,0,0),glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.f,1.f,0.f));
		ProjCalculate(_FOV,_nearPlane,_farPlane);

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
		_viewMatrixProperty->SetData(glm::value_ptr(ViewMatrix));
		_projMatrixProperty->SetData(glm::value_ptr(ProjMatrix));
		_viewProjMatrixProperty->SetData(glm::value_ptr(ViewProjMatrix));
		_viewInvProperty->SetData(glm::value_ptr(ViewInvMatrix));
		_projInvProperty->SetData(glm::value_ptr(ProjInvMatrix));
		_viewProjInvProperty->SetData(glm::value_ptr(ViewProjInvMatrix));
		_positionProperty->SetData(glm::value_ptr(EyePos));
		_propertyManager->UpdateShaderProperty();

	}

	void Camera::UpdateViewMatrix()
	{
		if( !_InvalidViewMatrix )
			return;

		glm::mat3 mat = glm::mat3_cast(Rotation);
		_Right = glm::column(mat,0);
		_Up = glm::column(mat,1);
		_Direction = glm::column(mat,2);
	 
		ViewMatrix = Math::ViewMatrixRH(EyePos,_Right,_Up,_Direction);
		ViewInvMatrix = glm::inverse(ViewMatrix);
		ViewProjMatrix =   ProjMatrix * ViewMatrix;
		ViewProjInvMatrix = ViewInvMatrix * ProjInvMatrix ;

		CameraFrustrum.Construct(ViewProjInvMatrix);

		_InvalidViewMatrix = false;

	}
 
	void Camera::UpdateProjectMatrix()
	{
		if( !_InvalidProjMatrix )
			return;
		
		glm::mat4 test;
		if (ProjectMode == ePerspectiveCamera)
			ProjMatrix = Math::ProjFovRH(_FOV, _aspectRatio, _nearPlane, _farPlane);
		else if (ProjectMode == eOrthoCamera)
		{
			float left = - GConfig->pRenderConfig->SizeX / 2.f;
			float right = GConfig->pRenderConfig->SizeX / 2.f;
			float top = GConfig->pRenderConfig->SizeY / 2.f;
			float bottom = -GConfig->pRenderConfig->SizeY / 2.f;
			ProjMatrix = glm::ortho(left,right,bottom,top,_nearPlane,_farPlane);//Math::OrthoRH(, (float)GConfig->pRenderConfig->SizeY, _nearPlane, _farPlane);
		    test = Math::OrthoRH((float)GConfig->pRenderConfig->SizeX, (float)GConfig->pRenderConfig->SizeY, _nearPlane, _farPlane);;
		}
	
		ProjInvMatrix = glm::inverse(ProjMatrix);
		ViewProjMatrix =  ProjMatrix * ViewMatrix;
		ViewProjInvMatrix = ViewInvMatrix * ProjInvMatrix  ;

		CameraFrustrum.Construct(ViewProjInvMatrix);
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
			_updateStrategy = CameraSphereTargetUpdate::Create(10,EyePos + Direction() * glm::vec3(10));
		}

	}

	void Camera::DebugDraw()
	{
		if( _updateMode == eFirstPersonMode )
		{
			GEngine->GameCanvas->DrawString(5, GConfig->pRenderConfig->SizeY - 25, "First Person Mode");
		}
		else if( _updateMode == eSphericalTargetMode )
		{
			GEngine->GameCanvas->DrawString(5, GConfig->pRenderConfig->SizeY - 25, "Spherical Coordinate Mode");
		} 
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

	void Camera::LookAt(const glm::vec3& eyePos,const glm::vec3& lookAt,const glm::vec3& upVec)
	{
		 LookAt_(eyePos,lookAt,upVec);	
		 _updateStrategy->SetTarget(this,lookAt);
	}

	void Camera::LookAt_(const glm::vec3& eyePos,const glm::vec3& lookAt,const glm::vec3& upVec)
	{
		EyePos = eyePos;
		_Direction = eyePos - lookAt;
		_Direction = glm::normalize(_Direction);
		_Right = glm::cross(upVec,_Direction);
		_Right = glm::normalize(_Right);
	    _Up = glm::cross(_Direction,_Right);
 
		glm::mat3 mat(_Right,_Up,_Direction);
		Rotation = glm::quat_cast(mat);

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