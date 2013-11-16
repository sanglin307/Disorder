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

		void AddLevel(LevelPtr const& level);
		LevelPtr GetLevel(); // the top level;
		LevelPtr GetLevel(std::string const& name);

	private:

		LevelContainer _Levels;
	};

    #define GWorld World::GetSingleton()

}





#endif