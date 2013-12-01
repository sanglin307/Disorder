#ifndef _DISORDER_WORLD_H_
#define _DISORDER_WORLD_H_




namespace Disorder
{
	class World : public Singleton<World>
	{
		friend class Singleton<World>;

		typedef std::list<LevelPtr> LevelContainer;
	public:
		void Init();
		void Exit();
		void Tick(float deltaSeconds);

		void AddLevel(LevelPtr const& level);
		LevelPtr GetLevel(); // the top level;
		LevelPtr GetLevel(std::string const& name);
		

	protected:
		static WorldPtr Create();

	private:

		World(){};
		LevelContainer _Levels;
	};

    #define GWorld World::GetSingleton()

}





#endif