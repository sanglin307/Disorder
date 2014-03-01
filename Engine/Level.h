#ifndef _DISORDER_LEVEL_H_
#define _DISORDER_LEVEL_H_

namespace Disorder
{
	class Level
	{
		typedef std::list<GameObjectPtr> GameObjectContainer;
	public:
        
		void AddGameObject(GameObjectPtr const& gameObject);

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
 
		static LevelPtr Create(std::string const& levelName);
		
		~Level();

		std::string Name;

	private:
		Level(std::string const& levelName);

		GameObjectContainer _vGameObjects;
		glm::vec4     _vAmbientColor;
 
	};
}



#endif