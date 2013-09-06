#ifndef _DISORDER_GAMEOBJECT_H_
#define _DISORDER_GAMEOBJECT_H_


namespace Disorder
{
	class GameObject : public boost::enable_shared_from_this<GameObject>
	{
	public:
 
		GameObject(std::string const& name, Vector3 const& pos = Vector3::ZERO,Vector3 const& rot = Vector3::ZERO,Vector3 const& scale = Vector3::UNIT_SCALE);
		~GameObject();
 
		void AddComponent(ComponentPtr const& component);
 
		void SetName(std::string const& name)
		{
			_name = name;
		}

		const std::string & GetName() const
		{
			return _name;
		}

		const TransformPtr &GetTransform() const
		{
			return _transform;
		}

		void Tick(float deltaSeconds);

	private:

		std::string _name;
		std::vector<ComponentPtr> _vComponents;
		TransformPtr _transform;
 
	};
}




#endif