#ifndef _DISORDER_LEVEL_H_
#define _DISORDER_LEVEL_H_

namespace Disorder
{
	class Level
	{
		typedef std::list<GameObjectPtr> GameObjectContainer;
	public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		void AddGameObject(GameObjectPtr const& gameObject);

		void Init();
		void Exit();

		void Tick(float deltaSeconds);
		void SetAmbientColor(Eigen::Vector4f const& color)
		{
			_vAmbientColor = color;
		}

		const Eigen::Vector4f& GetAmbientColor() const
		{
			return _vAmbientColor;
		}
 
		static LevelPtr Create(std::string const& levelName);
		
		~Level();

		std::string Name;

	private:
		Level(std::string const& levelName);

		GameObjectContainer _vGameObjects;
		Eigen::Vector4f     _vAmbientColor;
 
	};
}



#endif