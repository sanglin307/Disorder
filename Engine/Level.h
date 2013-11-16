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
		void SetAmbientColor(Vector4 const& color)
		{
			_vAmbientColor = color;
		}

		const Vector4& GetAmbientColor() const
		{
			return _vAmbientColor;
		}
 
		 Level(std::string const& levelName);
		~Level();

		std::string Name;

	private:
		GameObjectContainer _vGameObjects;
		Vector4     _vAmbientColor;
 
	};
}



#endif