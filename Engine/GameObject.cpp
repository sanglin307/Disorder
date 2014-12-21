#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
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

		_propertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerObject);
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

	 const Component* GameObject::GetComponent(std::string const& name) const
	 {
		 for(size_t i=0;i<_vComponents.size();i++)
		 {
			 if( _vComponents[i]->Name == name )
				 return _vComponents[i];
		 }

		 return NULL;
	 }
 
	void GameObject::AddComponent(Component* component)
	{
		_vComponents.push_back(component);
		component->SetBase(this);
		if( component->ComponentType == CT_Renderer )
		{
			GeometryRenderer* renderer = (GeometryRenderer*)component;
			if( renderer != NULL )
				GSceneManager->AddRenderer(renderer);
			else
				BOOST_ASSERT(0);
		}
		else if( component->ComponentType == CT_Light )
		{
			Light* light = (Light*)component;
			if( light != NULL )
				GSceneManager->AddLight(light);
			else
				BOOST_ASSERT(0);
		}
		else if( component->ComponentType == CT_Camera )
		{
			Camera* camera = (Camera*)component;
			if( camera != NULL )
				GSceneManager->AddCamera(camera);
			else 
				BOOST_ASSERT(0);
		}
 
	}

	void GameObject::Tick(float deltaSeconds)
	{	
		//just a test
		for (size_t i = 0; i < _vComponents.size(); i++)
		{
			if (_vComponents[i]->ComponentType == CT_Light)
			{
				Light* light = (Light*)_vComponents[i];
				if (light->LightType == LT_Point)
				{
					static glm::vec3 vDir(1.0, 0.0, 1.0);
					glm::vec3 value(Math::UnitRandom(), Math::UnitRandom(), Math::UnitRandom());
				
					if (_wldPos.x < -20)
						vDir.x = 1;
					if (_wldPos.y < -3)
						vDir.y = 0;
					if (_wldPos.z < -20)
						vDir.z = 1;

					if (_wldPos.x > 20)
						vDir.x = -1;
					if (_wldPos.y > 20)
						vDir.y = 0;
					if (_wldPos.z > 20)
						vDir.z = -1;

					WorldTranslate(vDir * value * 0.2f);
				}
			}
		}
	}

	const GameObject* GameObject::GetChild(std::string const& name)
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
		if(_parent != NULL )
		{
			const glm::quat& parentRot = _parent->GetWorldRotation();
			const glm::vec3& parentScale = _parent->GetWorldScale();

			_wldRot = parentRot * _locRot;
			_wldScale = parentScale / _locScale;
		 
			_wldPos = parentRot * ( parentScale * _locPos);
			_wldPos += _parent->GetWorldPosition();
		}
		else
		{
			_wldPos = _locPos;
			_wldRot = _locRot;
			_wldScale = _locScale;
		}

		_worldMatrix = glm::translate(_wldPos) * glm::scale(_wldScale) * glm::toMat4(_wldRot);
		for (size_t i = 0; i < _vComponents.size(); i++)
		{
			if (_vComponents[i]->ComponentType == CT_Renderer)
			{
				GeometryRenderer* renderer = (GeometryRenderer*)_vComponents[i];
				renderer->UpdateBoundingBox();
			}
		}

		for(std::map<std::string,GameObject*>::const_iterator iter=_mapChildren.begin();iter != _mapChildren.end();iter++)
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
		if( _parent )
		{
			_locPos += (glm::conjugate(_parent->GetWorldRotation()) * delta) / _parent->GetWorldScale();
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
		if( _parent )
		{
			SetLocalPosition(_parent->WorldToLocalPosition(position));
		}
		else
			SetLocalPosition(position);
	}

	void GameObject::SetWorldRotation(glm::quat const& rot)
	{
		if( _parent )
		{
			SetLocalRotation(_parent->WorldToLocalRotation(rot));
		}
		else
			SetLocalRotation(rot);
	}

	void GameObject::AddChild(GameObject* child)
	{
		AddChild(child,child->GetLocalPosition(),child->GetLocalRotation(),child->GetLocalScale());
	}

	void GameObject::AddChild(GameObject* child, glm::vec3 const& pos, glm::quat const& rot, glm::vec3 const& scale)
	{
		child->SetParent(this);
		child->SetLocalPosition(pos);
		child->SetLocalRotation(rot);
		child->SetLocalScale(scale);
		_mapChildren[child->Name] = child;
	}
	
}