#ifndef _DISORDER_WORLD_H_
#define _DISORDER_WORLD_H_

namespace Disorder
{
	class World
	{
	public:
		void Init();
		void Exit();
		void Tick(float deltaSeconds);

		void AddLevel(Level *level);
		Level* GetLevel(); // the top level;
		Level* GetLevel(std::string const& name);

	private:
		std::vector<Level*> _levels;
	};

	extern World* GWorld;

}





#endif