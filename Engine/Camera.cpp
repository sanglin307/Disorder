#include "EngineInclude.h"
 

namespace Disorder
{
 
	CameraSphereTargetUpdatePtr CameraSphereTargetUpdate::Create(float fRadius,const Eigen::Vector3f& target)
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
					Eigen::Vector3f moveDelta = pCamera->Right() * speed * deltaSeconds * (float)mouseEvent.RelativeX;
					pCamera->_eyePos -= moveDelta;
					_target -= moveDelta;
					pCamera->_InvalidViewMatrix = true;
				}

				if( mouseEvent.RelativeY != 0 )
				{
					Eigen::Vector3f moveDelta = pCamera->Up() * speed * deltaSeconds * (float)mouseEvent.RelativeY;
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

				float speed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;

				Math::ConvertToSphericalCoord(pCamera->_eyePos,_target,_radius,yAngle,zAngle);

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


				Eigen::Vector3f finalPos;
				Math::ConvertFromSphericalCoord(_radius,yAngle,zAngle,_target,finalPos);
				pCamera->LookAt_(finalPos,_target,Eigen::Vector3f::UnitY());
				return true;
			}		
	
			
		 
		}
		return false;
	}

	void CameraSphereTargetUpdate::SetTarget(Camera *pCamera,const Eigen::Vector3f& target)
	{
		Eigen::Vector3f viewVec = target - pCamera->_eyePos;
		viewVec.normalize();
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
			Eigen::Vector3f eulerAngle = pCamera->ViewMatrix.linear().eulerAngles(0,1,2); //.toRotationMatrixGetPitch(false);
			float speed = GConfig->pCameraConfig->mFreeMode.RotateSpeed;
			if( mouseEvent.RelativeX != 0 )
			{
				eulerAngle.y() += mouseEvent.RelativeX * speed * deltaSeconds;
			}

			if( mouseEvent.RelativeY != 0 )
			{
				eulerAngle.x() += mouseEvent.RelativeY * speed * deltaSeconds;    
			}

			if( mouseEvent.RelativeY != 0 || mouseEvent.RelativeX != 0 )
			{
 
				pCamera->_rotation = Eigen::AngleAxisf(eulerAngle.x(),Eigen::Vector3f::UnitX()) *   
					                 Eigen::AngleAxisf(eulerAngle.y(),Eigen::Vector3f::UnitY()) *  
									 Eigen::AngleAxisf(eulerAngle.z(),Eigen::Vector3f::UnitZ());    
				 
				pCamera->_rotation = pCamera->_rotation.conjugate();
 
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
			pCamera->_eyePos = pCamera->_eyePos + pCamera->Direction() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_S) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->Direction() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_A) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->Right() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_D) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->Right() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_Q) )
		{
			pCamera->_eyePos = pCamera->_eyePos + pCamera->Up() * speed * deltaSeconds;
			pCamera->_InvalidViewMatrix = true;
		}

		if( inputManager->IsKeyDown(OIS::KC_E) )
		{
			pCamera->_eyePos = pCamera->_eyePos - pCamera->Up() * speed * deltaSeconds;
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

		_nearPlane = GConfig->pCameraConfig->NearClip;
		_farPlane = GConfig->pCameraConfig->FarClip;
		_FOV = GConfig->pCameraConfig->FOV * Math::fDeg2Rad;
		_aspectRatio = GConfig->pRenderConfig->SizeX*1.0f/GConfig->pRenderConfig->SizeY;

		LookAt_(Eigen::Vector3f::Zero(),Eigen::Vector3f(0.0f,0.0f,-1.0f),Eigen::Vector3f(0.f,1.f,0.f));
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
		_viewMatrixProperty->SetData(ViewMatrix.data());
		_projMatrixProperty->SetData(ProjMatrix.data());
		_viewProjMatrixProperty->SetData(ViewProjMatrix.data());
		_viewInvProperty->SetData(ViewInvMatrix.data());
		_projInvProperty->SetData(ProjInvMatrix.data());
		_viewProjInvProperty->SetData(ViewProjInvMatrix.data());
		_positionProperty->SetData(_eyePos.data());
		_propertyManager->UpdateShaderProperty();

	}

	void Camera::UpdateViewMatrix()
	{
		if( !_InvalidViewMatrix )
			return;

		Eigen::Quaternionf q = _rotation.conjugate();
        ViewMatrix.linear() = q.toRotationMatrix();
		ViewMatrix.translation() = - (ViewMatrix.linear() * _eyePos);

		ViewInvMatrix = ViewMatrix.inverse().matrix();
		ViewProjMatrix.noalias() =  ProjMatrix * ViewMatrix.matrix();
		ViewProjInvMatrix.noalias() = ViewInvMatrix * ProjInvMatrix;
		CameraFrustrum.Construct(ViewProjInvMatrix);

		_InvalidViewMatrix = false;

	}
 
	void Camera::UpdateProjectMatrix()
	{
		if( !_InvalidProjMatrix )
			return;
 
		ProjMatrix = Math::ProjFovRH(_FOV,_aspectRatio,_nearPlane,_farPlane);

		GEngine->RenderEngine->AdjustProjMatrix(ProjMatrix);
		ProjInvMatrix = ProjMatrix.inverse();
		ViewProjMatrix.noalias() =  ProjMatrix * ViewMatrix.matrix();
		ViewProjInvMatrix.noalias() = ViewInvMatrix * ProjInvMatrix;

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
			_updateStrategy = CameraSphereTargetUpdate::Create(10,_eyePos + Direction() * 10);
		}

	}

	void Camera::DebugDraw()
	{
		std::stringstream strstream;
		Eigen::Vector3f lookAt = _eyePos + Direction() * 50;
		strstream << "camera: [eyePos](" << (int)_eyePos.x() << "; " <<  (int)_eyePos.y() << "; " <<  (int)_eyePos.z() << ")     [Focus At](" <<  (int)lookAt.x() << ";  "<<  (int)lookAt.y() << ";  " <<  (int)lookAt.z() << ")";
		if( _updateMode == eFirstPersonMode )
		{
			GEngine->GameCanvas->DrawString(0.005f,0.945f,0.04f,Vector4f::ONE,"First Person Mode");
		}
		else if( _updateMode == eSphericalTargetMode )
		{
			GEngine->GameCanvas->DrawString(0.005f,0.945f,0.04f,Vector4f::ONE,"Spherical Coordinate Mode");
		}
		GEngine->GameCanvas->DrawString(0.005f,0.965f,0.04f,Vector4f::ONE,strstream.str());
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

	void Camera::LookAt(Eigen::Vector3f const& eyePos,Eigen::Vector3f const& lookAt,Eigen::Vector3f const& upVec)
	{
		 LookAt_(eyePos,lookAt,upVec);	
		 _updateStrategy->SetTarget(this,lookAt);
	}

	void Camera::LookAt_(Eigen::Vector3f const& eyePos,Eigen::Vector3f const& lookAt,Eigen::Vector3f const& upVec)
	{
		 _eyePos = eyePos;
         Eigen::Matrix3f camAxes;
		 Eigen::Vector3f newDirection = lookAt - eyePos;
		 camAxes.col(2) = (-newDirection).normalized();
		 camAxes.col(0) = upVec.cross( camAxes.col(2) ).normalized();
		 camAxes.col(1) = camAxes.col(2).cross( camAxes.col(0) ).normalized();
		 _rotation = Eigen::Quaternionf(camAxes);

		 _InvalidViewMatrix = true;
	}

	void Camera::ProjCalculate(float FOV,  float nearPlane,float farPlane)
	{
		_FOV		 = FOV * Math::fDeg2Rad;
 
		_nearPlane	 = nearPlane;
		_farPlane	 = farPlane;

		_InvalidProjMatrix = true;
	}	

	Eigen::Vector3f Camera::Direction() const
	{
		return - (_rotation * Eigen::Vector3f::UnitZ());
	}

	Eigen::Vector3f Camera::Up() const
	{
		return _rotation * Eigen::Vector3f::UnitY();
	}

	Eigen::Vector3f Camera::Right() const
	{
		return _rotation * Eigen::Vector3f::UnitX();
	}

}