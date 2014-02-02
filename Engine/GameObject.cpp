#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
	}

	GameObjectPtr GameObject::Create(std::string const& name, Eigen::Vector3f const& pos,Eigen::Quaternionf const& rot,Eigen::Vector3f const& scale)
	{
		GameObject *pGo = new GameObject(name,pos,rot,scale);
		return GameObjectPtr(pGo);
	}

	GameObject::GameObject(std::string const& name, Eigen::Vector3f const& pos,Eigen::Quaternionf const& rot,Eigen::Vector3f const& scale)
		:Name(name)
	{
		_locPos = pos;
		_locRot = rot;
		_locScale = scale;
		_wldPos = _locPos;
		_wldRot = _locRot;
		_wldScale = _locScale;

		_worldMatrix = Eigen::Translation3f(_wldPos) * Eigen::Scaling(_wldScale) * _wldRot;

		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerObject);
		_worldMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sObjectWorld,eSP_Float,16);	
		_worldNormalMatrixProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sObjectNormal,eSP_Float,16);
	}

	void GameObject::UpdateShaderProperty()
	{
		_propertyManager->ClearShaderPropertyValue();
		_worldMatrixProperty->SetData(_worldMatrix.data());
		Eigen::Matrix4f normalMatrix = _worldMatrix.matrix().inverse().transpose();
		_worldNormalMatrixProperty->SetData(normalMatrix.data());
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
		LocalRotate(Eigen::Vector3f::UnitX(),radian);
	}

	void GameObject::LocalRoll(float radian)
	{
		LocalRotate(Eigen::Vector3f::UnitZ(),radian);
	}

	void GameObject::LocalYaw(float radian)
	{
		LocalRotate(Eigen::Vector3f::UnitY(),radian);
	}

	void GameObject::LocalRotate(Eigen::Vector3f const& axis, float radian)
	{
		_locRot = Eigen::AngleAxisf(radian,axis) * _locRot;
		RefreshWorldTransform();
	}

	void GameObject::LocalTranslate(const Eigen::Vector3f& delta)
	{
		_locPos += _locRot * delta;
		RefreshWorldTransform();
	}

	void GameObject::LocalScale(const Eigen::Vector3f& delta)
	{
		_locScale = _locScale.eval().cwiseProduct(delta);
		RefreshWorldTransform();
	}

	void GameObject::SetLocalRotation(Eigen::Quaternionf const& rot)
	{
		_locRot = rot;
		RefreshWorldTransform();
	}
 
	void GameObject::SetLocalPosition(Eigen::Vector3f const& position)
	{
		_locPos = position;
		RefreshWorldTransform();
	}

	void GameObject::SetLocalScale(Eigen::Vector3f const& scale)
	{
		_locScale = scale;
		RefreshWorldTransform();
	}
 
	void GameObject::RefreshWorldTransform()
	{
		GameObjectPtr parent = _parent.lock();
		if(parent != NULL )
		{
			const Eigen::Quaternionf& parentRot = parent->GetWorldRotation();
			const Eigen::Vector3f& parentScale = parent->GetWorldScale();

			_wldRot = parentRot * _locRot;
			_wldScale = parentScale.cwiseProduct(_locScale);
		 
			_wldPos = parentRot * ( parentScale.cwiseProduct(_locPos) );
			_wldPos += parent->GetWorldPosition();
		}
		else
		{
			_wldPos = _locPos;
			_wldRot = _locRot;
			_wldScale = _locScale;
		}

		_worldMatrix = Eigen::Translation3f(_wldPos) * Eigen::Scaling(_wldScale) * _wldRot;

		for(std::map<std::string,GameObjectPtr>::const_iterator iter=_mapChildren.begin();iter != _mapChildren.end();iter++)
		{
			iter->second->RefreshWorldTransform();
		}
	}

	 

	 void GameObject::WorldPitch(float radian)
	 {
		 WorldRotate(Eigen::Vector3f::UnitX(),radian);
	 }

	void GameObject::WorldRoll(float radian)
	{
		WorldRotate(Eigen::Vector3f::UnitZ(),radian);
	}

	void GameObject::WorldYaw(float radian)
	{
		WorldRotate(Eigen::Vector3f::UnitY(),radian);
	}

	void GameObject::WorldRotate(const Eigen::Vector3f& axis,float radian)
	{
		_locRot = _locRot * _wldRot.conjugate() * Eigen::AngleAxisf(radian,axis) * _wldRot;
		RefreshWorldTransform();
	}
	
	void GameObject::WorldTranslate(const Eigen::Vector3f& delta)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			_locPos += (parent->GetWorldRotation().conjugate() * delta).cwiseQuotient(parent->GetWorldScale());
		}
		else
			_locPos += delta;

		RefreshWorldTransform();
	}
 
 
	Eigen::Vector3f GameObject::WorldToLocalPosition( const Eigen::Vector3f &worldPos )
	{
		return _wldRot.conjugate() * (worldPos - _wldPos).cwiseQuotient(_wldScale);
	}
	 
	Eigen::Vector3f GameObject::LocalToWorldPosition( const Eigen::Vector3f &localPos )
	{
		return (_wldRot * (localPos.cwiseProduct(_wldScale))) + _wldPos;
	}
	 
	Eigen::Quaternionf GameObject::WorldToLocalRotation( const Eigen::Quaternionf &worldRot )
	{
		return _wldRot.conjugate() * worldRot;
	}
 
	Eigen::Quaternionf GameObject::LocalToWorldRotation( const Eigen::Quaternionf &localRot )
	{
		return _wldRot * localRot;
	}

	void GameObject::SetWorldPosition(Eigen::Vector3f const& position)
	{
		GameObjectPtr parent = _parent.lock();
		if( parent )
		{
			SetLocalPosition(parent->WorldToLocalPosition(position));
		}
		else
			SetLocalPosition(position);
	}

	void GameObject::SetWorldRotation(Eigen::Quaternionf const& rot)
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

	void GameObject::AddChild(GameObjectPtr const& child,Eigen::Vector3f const& pos,Eigen::Quaternionf const& rot,Eigen::Vector3f const& scale)
	{
		child->SetParent(shared_from_this());
		child->SetLocalPosition(pos);
		child->SetLocalRotation(rot);
		child->SetLocalScale(scale);
		_mapChildren[child->Name] = child;
	}
	
}