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

		Vector4 GetAmbientColor()
		{
			return _vAmbientColor;
		}

		 Level();
		~Level();

	private:
		GameObjectContainer _vGameObjects;
		Vector4     _vAmbientColor;
	};
}



#endif