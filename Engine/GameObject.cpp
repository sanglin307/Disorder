#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
	}

	GameObjectPtr GameObject::Create(std::string const& name, Vector3 const& pos,Quaternion const& rot,Vector3 const& scale)
	{
		GameObject *pGo = new GameObject(name,pos,rot,scale);
		return GameObjectPtr(pGo);
	}

	GameObject::GameObject(std::string const& name, Vector3 const& pos,Quaternion const& rot,Vector3 const& scale)
		:Name(name)
	{
		_locPos = pos;
		_locRot = rot;
		_locScale = scale;
		_wldPos = _locPos;
		_wldRot = _locRot;
		_wldScale = _locScale;

		_worldMatrix.MakeTransform(_wldPos,_wldScale,_wldRot);

		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerObject);
		_worldMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sObjectWorld,eSP_Float,16);	
		_worldNormalMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sObjectNormal,eSP_Float,16);
	}

	void GameObject::UpdateShaderProperty()
	{
		_propertyManager->ClearShaderPropertyValue();
		Matrix4 transMatrix = _worldMatrix.Transpose();
		_worldMatrixProperty->SetData((float*)&transMatrix);
		Matrix4 normalMatrix = transMatrix.GetNormalMatrix();
		_worldNormalMatrixProperty->SetData((float*)&normalMatrix);
		_propertyManager->UpdateShaderProperty();
	}

	 ComponentPtr GameObject::GetComponent(std::string const& name) const
	 {
		 for(size_t i=0;i<_vComponents.size();i++)
		 {
			 if( _vComponents[i]->Name == name )
				 return _vComponents[i];
		 }

		 return NULL;
	 }
 
	void GameObject::AddComponent(ComponentPtr const& component)
	{
		_vComponents.push_back(component);
		component->SetBase(shared_from_this());
		if( component->ComponentType == CT_Renderer )
		{
			RendererPtr renderer = boost::dynamic_pointer_cast<Renderer>(component);
			if( renderer != NULL )
				GSceneManager->AddRenderer(renderer);
			else
				BOOST_ASSERT(0);
		}
		else if( component->ComponentType == CT_Light )
		{
			LightPtr light = boost::dynamic_pointer_cast<Light>(component);
			if( light != NULL )
				GSceneManager->AddLight(light);
			else
				BOOST_ASSERT(0);
		}
		else if( component->ComponentType == CT_Camera )
		{
			CameraPtr camera = boost::dynamic_pointer_cast<Camera>(component);
			if( camera != NULL )
				GSceneManager->AddCamera(camera);
			else 
				BOOST_ASSERT(0);
		}
 
	}

	void GameObject::Tick(float deltaSeconds)
	{	 
	}

	GameObjectPtr GameObject::GetChild(std::string const& name)
	{
	    if( _mapChildren.find(name) != _mapChildren.end() )
			return _mapChildren[name];

		return NULL;
	}

 
	void GameObject::LocalPitch(float radian)
	{
		LocalRotate(Vector3::UNIT_X,radian);
	}

	void GameObject::LocalRoll(float radian)
	{
		LocalRotate(Vector3::UNIT_Z,radian);
	}

	void GameObject::LocalYaw(float radian)
	{
		LocalRotate(Vector3::UNIT_Y,radian);
	}

	void GameObject::LocalRotate(Vector3 const& axis, float radian)
	{
		Quaternion rot;
		rot.FromAngleAxis(radian,axis);
		_locRot = rot * _locRot;
		RefreshWorldTransform();
	}

	void GameObject::LocalTranslate(const Vector3& delta)
	{
		_locPos += _locRot * delta;
		RefreshWorldTransform();
	}

	void GameObject::LocalScale(const Vector3& delta)
	{
		_locScale = _locScale * delta;
		RefreshWorldTransform();
	}

	void GameObject::SetLocalRotation(Quaternion const& rot)
	{
		_locRot = rot;
		RefreshWorldTransform();
	}
 
	void GameObject::SetLocalPosition(Vector3 const& position)
	{
		_locPos = position;
		RefreshWorldTransform();
	}

	void GameObject::SetLocalScale(Vector3 const& scale)
	{
		_locScale = scale;
		RefreshWorldTransform();
	}
 
	void GameObject::RefreshWorldTransform()
	{
		GameObjectPtr parent = _parent.lock();
		if(parent != NULL )
		{
			const Quaternion& parentRot = parent->GetWorldRotation();
			const Vector3& parentScale = parent->GetWorldScale();

			_wldRot = parentRot * _locRot;
			_wldScale = parentScale * _locScale;
		 
			_wldPos = parentRot * ( parentScale * _locPos );
			_wldPos += parent->GetWorldPosition();
		}
		else
		{
			_wldPos = _locPos;
			_wldRot = _locRot;
			_wldScale = _locScale;
		}

		_worldMatrix.MakeTransform(_wldPos,_wldScale,_wldRot);

		for(std::map<std::string,GameObjectPtr>::const_iterator iter=_mapChildren.begin();iter != _mapChildren.end();iter++)
		{
			iter->second->RefreshWorldTransform();
		}
	}

	 

	 void GameObject::WorldPitch(float radian)
	 {
		 WorldRotate(Vector3::UNIT_X,radian);
	 }

	void GameObject::WorldRoll(float radian)
	{
		WorldRotate(Vector3::UNIT_Z,radian);
	}

	void GameObject::WorldYaw(float radian)
	{
		WorldRotate(Vector3::UNIT_Y,radian);
	}

	void GameObject::WorldRotate(const Vector3& axis,float radian)
	{
		Quaternion rot;
		rot.FromAngleAxis(radian,axis);
		_locRot = _locRot * _wldRot.Inverse() * rot * _wldRot;
		RefreshWorldTransform();
	}
	
	void GameObject::WorldTranslate(const Vector3& delta)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			_locPos += (parent->GetWorldRotation().Inverse() * delta) / parent->GetWorldScale();
		}
		else
			_locPos += delta;

		RefreshWorldTransform();
	}
 
 
	Vector3 GameObject::WorldToLocalPosition( const Vector3 &worldPos )
	{
		return _wldRot.Inverse() * (worldPos - _wldPos) / _wldScale;
	}
	 
	Vector3 GameObject::LocalToWorldPosition( const Vector3 &localPos )
	{
		return (_wldRot * (localPos * _wldScale)) + _wldPos;
	}
	 
	Quaternion GameObject::WorldToLocalRotation( const Quaternion &worldRot )
	{
		return _wldRot.Inverse() * worldRot;
	}
 
	Quaternion GameObject::LocalToWorldRotation( const Quaternion &localRot )
	{
		return _wldRot * localRot;
	}

	void GameObject::SetWorldPosition(Vector3 const& position)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			SetLocalPosition(parent->WorldToLocalPosition(position));
		}
		else
			SetLocalPosition(position);
	}

	void GameObject::SetWorldRotation(Quaternion const& rot)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			SetLocalRotation(parent->WorldToLocalRotation(rot));
		}
		else
			SetLocalRotation(rot);
	}

	void GameObject::AddChild(GameObjectPtr const& child)
	{
		AddChild(child,child->GetLocalPosition(),child->GetLocalRotation(),child->GetLocalScale());
	}

	void GameObject::AddChild(GameObjectPtr const& child,Vector3 const& pos,Quaternion const& rot,Vector3 const& scale)
	{
		child->SetParent(shared_from_this());
		child->SetLocalPosition(pos);
		child->SetLocalRotation(rot);
		child->SetLocalScale(scale);
		_mapChildren[child->Name] = child;
	}
	
}