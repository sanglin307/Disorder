#include "EngineInclude.h"

namespace Disorder
{
	GameObject::~GameObject()
	{
		_vComponents.clear();
	}

	GameObject::GameObject(std::string const& name, Vector3 const& pos,Vector3 const& rot,Vector3 const& scale)
		:_name(name)
	{
		_transform = boost::make_shared<Transform>(pos,rot,scale);
	}
 
	void GameObject::AddComponent(ComponentPtr const& component)
	{
		_vComponents.push_back(component);
		component->SetBase(shared_from_this());
		if( component->Type == CT_Renderer )
		{
			RendererPtr renderer = boost::dynamic_pointer_cast<Renderer>(component);
			if( renderer != NULL )
				GSceneManager->AddRenderer(renderer);
			else
				BOOST_ASSERT(0);
		}
		else if( component->Type == CT_Light )
		{
			LightPtr light = boost::dynamic_pointer_cast<Light>(component);
			if( light != NULL )
				GSceneManager->AddLight(light);
			else
				BOOST_ASSERT(0);
		}
		else if( component->Type == CT_Camera )
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

	
}