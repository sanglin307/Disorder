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

		~Level();

	private:
		GameObjectContainer _vGameObjects;
	};
}



#endif