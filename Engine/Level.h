#ifndef _DISORDER_LEVEL_H_
#define _DISORDER_LEVEL_H_

namespace Disorder
{
	class Level
	{
		typedef std::list<GameObject*> GameObjectContainer;
	public:
        
		void AddGameObject(GameObject* gameObject);

		void Init();
		void Exit();

		void Tick(float deltaSeconds);
		void SetAmbientColor(glm::vec4 const& color)
		{
			_vAmbientColor = color;
		}

		const glm::vec4& GetAmbientColor() const
		{
			return _vAmbientColor;
		}
 
		Level(std::string const& levelName);
		~Level();

		std::string Name;

	private:
		GameObjectContainer _vGameObjects;
		glm::vec4     _vAmbientColor;
 
	};
}



#endif