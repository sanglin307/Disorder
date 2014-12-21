#ifndef _DISORDER_WORLD_H_
#define _DISORDER_WORLD_H_

namespace Disorder
{
	class World
	{
	public:
		World();
		~World();
		void Tick(float deltaSeconds);

		void AddLevel(Level *level);
		Level* GetLevel(); // the top level;
		Level* GetLevel(std::string const& name);

	private:
		std::vector<Level*> _levels;
	};



}





#endif