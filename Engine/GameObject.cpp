#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
	}

	GameObject::GameObject(Vector3 const& position)
	{
		Position = position;
		Scale = Vector3(1.0f);
		Rotation = Quaternion::IDENTITY;
		WorldMatrix.makeTransform(Position,Scale,Rotation);
	}

	void GameObject::AddComponent(ComponentPtr const& component)
	{
		_vComponents.push_back(component);

		RendererPtr renderer = boost::dynamic_pointer_cast<Renderer>(component);
		if( renderer != NULL )
			GSceneManager->AddRenderer(renderer);
	}

	void GameObject::Tick(float deltaSeconds)
	{
		static float radian = 0.0f;
		radian += 0.01f;
		// test code ,rotate it continuously~ 
	    Rotation.FromAngleAxis(radian,Vector3::UNIT_Y);
		WorldMatrix.makeTransform(Position,Scale,Rotation);
	}

}