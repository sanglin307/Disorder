#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
	}

	GameObjectPtr GameObject::Create(std::string const& name, glm::vec3 const& pos, glm::quat const& rot, glm::vec3 const& scale)
	{
		GameObject *pGo = new GameObject(name,pos,rot,scale);
		return GameObjectPtr(pGo);
	}

	GameObject::GameObject(std::string const& name, glm::vec3 const& pos, glm::quat const& rot, glm::vec3 const& scale)
		:Name(name)
	{
		_locPos = pos;
		_locRot = rot;
		_locScale = scale;
		_wldPos = _locPos;
		_wldRot = _locRot;
		_wldScale = _locScale;

		_worldMatrix = glm::translate(_wldPos) * glm::scale(_wldScale) * glm::toMat4(_wldRot);

		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerObject);
		_worldMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sObjectWorld,eSP_Float,16);	
		_worldNormalMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sObjectNormal,eSP_Float,16);
	}

	void GameObject::UpdateShaderProperty()
	{
		_propertyManager->ClearShaderPropertyValue();
		_worldMatrixProperty->SetData(glm::value_ptr(_worldMatrix));
		glm::mat4 normalMatrix = glm::inverseTranspose(_worldMatrix);
		_worldNormalMatrixProperty->SetData(glm::value_ptr(normalMatrix));
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
		LocalRotate(glm::vec3(1.f,0,0), radian);
	}

	void GameObject::LocalRoll(float radian)
	{
		LocalRotate(glm::vec3(0,0,1), radian);
	}

	void GameObject::LocalYaw(float radian)
	{
		LocalRotate(glm::vec3(0,1,0), radian);
	}

	void GameObject::LocalRotate(glm::vec3 const& axis, float radian)
	{
		_locRot = glm::angleAxis(radian,axis) * _locRot;
		RefreshWorldTransform();
	}

	void GameObject::LocalTranslate(const glm::vec3& delta)
	{
		_locPos += _locRot * delta;
		RefreshWorldTransform();
	}

	void GameObject::LocalScale(const glm::vec3& delta)
	{
		_locScale = _locScale * delta;
		RefreshWorldTransform();
	}

	void GameObject::SetLocalRotation(glm::quat const& rot)
	{
		_locRot = rot;
		RefreshWorldTransform();
	}
 
	void GameObject::SetLocalPosition(glm::vec3 const& position)
	{
		_locPos = position;
		RefreshWorldTransform();
	}

	void GameObject::SetLocalScale(glm::vec3 const& scale)
	{
		_locScale = scale;
		RefreshWorldTransform();
	}
 
	void GameObject::RefreshWorldTransform()
	{
		GameObjectPtr parent = _parent.lock();
		if(parent != NULL )
		{
			const glm::quat& parentRot = parent->GetWorldRotation();
			const glm::vec3& parentScale = parent->GetWorldScale();

			_wldRot = parentRot * _locRot;
			_wldScale = parentScale / _locScale;
		 
			_wldPos = parentRot * ( parentScale / _locPos);
			_wldPos += parent->GetWorldPosition();
		}
		else
		{
			_wldPos = _locPos;
			_wldRot = _locRot;
			_wldScale = _locScale;
		}

		_worldMatrix = glm::translate(_wldPos) * glm::scale(_wldScale) * glm::toMat4(_wldRot);

		for(std::map<std::string,GameObjectPtr>::const_iterator iter=_mapChildren.begin();iter != _mapChildren.end();iter++)
		{
			iter->second->RefreshWorldTransform();
		}
	}

	 

	 void GameObject::WorldPitch(float radian)
	 {
		 WorldRotate(glm::vec3(1,0,0), radian);
	 }

	void GameObject::WorldRoll(float radian)
	{
		WorldRotate(glm::vec3(0,0,1), radian);
	}

	void GameObject::WorldYaw(float radian)
	{
		WorldRotate(glm::vec3(0,1,0), radian);
	}

	void GameObject::WorldRotate(const glm::vec3& axis, float radian)
	{
		_locRot = _locRot * glm::conjugate(_wldRot) * glm::angleAxis(radian,axis) * _wldRot;
		RefreshWorldTransform();
	}
	
	void GameObject::WorldTranslate(const glm::vec3& delta)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			_locPos += (glm::conjugate(parent->GetWorldRotation()) * delta) / parent->GetWorldScale();
		}
		else
			_locPos += delta;

		RefreshWorldTransform();
	}
 
 
	glm::vec3 GameObject::WorldToLocalPosition(const glm::vec3 &worldPos)
	{
		return glm::conjugate(_wldRot) * ((worldPos - _wldPos) /_wldScale);
	}
	 
	glm::vec3 GameObject::LocalToWorldPosition(const glm::vec3 &localPos)
	{
		return (_wldRot * (localPos * _wldScale)) + _wldPos;
	}
	 
	glm::quat GameObject::WorldToLocalRotation(const glm::quat &worldRot)
	{
		return glm::conjugate(_wldRot) * worldRot;
	}
 
	glm::quat GameObject::LocalToWorldRotation( const glm::quat &localRot )
	{
		return _wldRot * localRot;
	}

	void GameObject::SetWorldPosition(glm::vec3 const& position)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			SetLocalPosition(parent->WorldToLocalPosition(position));
		}
		else
			SetLocalPosition(position);
	}

	void GameObject::SetWorldRotation(glm::quat const& rot)
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

	void GameObject::AddChild(GameObjectPtr const& child, glm::vec3 const& pos, glm::quat const& rot, glm::vec3 const& scale)
	{
		child->SetParent(shared_from_this());
		child->SetLocalPosition(pos);
		child->SetLocalRotation(rot);
		child->SetLocalScale(scale);
		_mapChildren[child->Name] = child;
	}
	
}