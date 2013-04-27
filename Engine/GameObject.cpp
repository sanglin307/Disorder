#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
	}

	GameObject::GameObject(std::string const& name, Vector3 const& pos,Quaternion const& rot,Vector3 const& scale)
		:_name(name),_position(pos),_rotation(rot),_scale(scale)
	{
		_localMatrix.makeTransform(_position,_scale,_rotation);
		_worldMatrix = _localMatrix;
	}

	void GameObject::LocalTransform(Vector3 const& pos,Quaternion const& rot,Vector3 const& scale)
	{
		_position = pos;
		_scale = scale;
		_rotation = rot;
		_localMatrix.makeTransform(_position,_scale,_rotation);
		 
		GameObjectPtr parent = _parent.lock();
		if(parent != NULL )
		{
			_worldMatrix = parent->GetWorldMatrix() * _localMatrix;
		}
		 
	}

	void GameObject::AddComponent(ComponentPtr const& component)
	{
		_vComponents.push_back(component);
		component->SetBase(shared_from_this());
		RendererPtr renderer = boost::dynamic_pointer_cast<Renderer>(component);
		if( renderer != NULL )
			GSceneManager->AddRenderer(renderer);
	}

	void GameObject::Tick(float deltaSeconds)
	{
		static float radian = 0.0f;
		radian += 0.01f;
		// test code ,rotate it continuously~ 
	  //  _rotation.FromAngleAxis(radian,Vector3::UNIT_Y);
	//	_worldMatrix.makeTransform(_position,_scale,_rotation);
	}

	void GameObject::AddChild(GameObjectPtr const& child)
	{
		AddChild(child,child->GetPosition(),child->GetRotation(),child->GetScale());
	}

	void GameObject::AddChild(GameObjectPtr const& child,Vector3 const& pos,Quaternion const& rot,Vector3 const& scale)
	{
		child->SetParent(shared_from_this());
		child->LocalTransform(pos,rot,scale);
		_vChildren.push_back(child);
	}
}