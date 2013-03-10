#ifndef _DISORDER_GAMEOBJECT_H_
#define _DISORDER_GAMEOBJECT_H_


namespace Disorder
{
	class GameObject
	{
	public:
 
		void AddComponent(ComponentPtr const& component);
		~GameObject();

		GameObject(Vector3 const& position);
		
		void Tick(float deltaSeconds);

	    Vector3 Position;
		Vector3 Scale;
		Quaternion Rotation;
		Matrix4 WorldMatrix;

	private:
		std::list<ComponentPtr> _vComponents;

	


	};
}




#endif