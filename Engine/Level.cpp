#include "EngineInclude.h"


namespace Disorder
{
	void Level::AddGameObject(GameObject* gameObject)
	{
		_vGameObjects.push_back(gameObject);
	}

	 
	Level::Level(std::string const& levelName)
		:Name(levelName)
	{
		_vAmbientColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Level::~Level()
	{
		_vGameObjects.clear();
	}

    void Level::Init()
	{
	}
		
	void Level::Exit()
	{
	}
	
	void Level::Tick(float deltaSeconds)
	{
		GameObjectContainer::iterator iter = _vGameObjects.begin();
		for(;iter!=_vGameObjects.end();iter++)
		{
			// now we tick all object first,later we will add flag to it,and select tickable ones.
			(*iter)->Tick(deltaSeconds);
		}
	}
}