#ifndef _DISORDER_WORLD_H_
#define _DISORDER_WORLD_H_




namespace Disorder
{
	class World : public Singleton<World>
	{
		typedef std::list<LevelPtr> LevelContainer;
	public:
		void Init();
		void Exit();

		void Tick(float deltaSeconds);

		LevelContainer Levels;
	};

    #define GWorld World::GetSingleton()

}





#endif