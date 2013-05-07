#ifndef _DISORDER_GAMEOBJECT_H_
#define _DISORDER_GAMEOBJECT_H_


namespace Disorder
{
	class GameObject : public boost::enable_shared_from_this<GameObject>
	{
	public:
 
		GameObject(std::string const& name, Vector3 const& pos = Vector3::ZERO,Quaternion const& rot = Quaternion::IDENTITY,Vector3 const& scale = Vector3::UNIT_SCALE);
		~GameObject();

		

		void AddComponent(ComponentPtr const& component);

		

		void SetName(std::string const& name)
		{
			_name = name;
		}

		
		void SetParent(GameObjectPtr const& parent)
		{
			_parent = parent;
		}
		void AddChild(GameObjectPtr const& child,Vector3 const& pos,Quaternion const& rot,Vector3 const& scale);
		void AddChild(GameObjectPtr const& child);

		unsigned int GetChildCount()
		{
			return _vChildren.size();
		}
		
		GameObjectPtr GetChild(unsigned int index)
		{
			if( index < _vChildren.size())
				return _vChildren[index];

			return NULL;
		}

		void LocalRotation(float pitch,float yaw,float roll);
		void LocalPosition(Vector3 const& position);
		void LocalScale(Vector3 const& scale);

		void LocalTransform(Vector3 const& pos = Vector3::ZERO,Quaternion const& rot = Quaternion::IDENTITY,Vector3 const& scale = Vector3::UNIT_SCALE);

		const Matrix4& GetWorldMatrix() const
		{
			return _worldMatrix;
		}

		const Vector3& GetPosition()
		{
			return _position;
		}

		const Quaternion& GetRotation()
		{
			return _rotation;
		}

		const Vector3& GetScale()
		{
			return _scale;
		}

		void Tick(float deltaSeconds);

	private:

	    Vector3 _position;
		Vector3 _scale;
		Quaternion _rotation;
		Matrix4 _localMatrix;
		Matrix4 _worldMatrix;

		std::string _name;

	private:
		std::vector<ComponentPtr> _vComponents;

		boost::weak_ptr<GameObject> _parent;
		std::vector<GameObjectPtr> _vChildren;

	


	};
}




#endif