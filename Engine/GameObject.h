#ifndef _DISORDER_GAMEOBJECT_H_
#define _DISORDER_GAMEOBJECT_H_


namespace Disorder
{
	class GameObject : public boost::enable_shared_from_this<GameObject>
	{
	public:
 
	

		GameObject(Vector3 const& pos = Vector3::ZERO,Quaternion const& rot = Quaternion::ZERO,Vector3 const& scale = Vector3::UNIT_SCALE);
		~GameObject();

		

		void AddComponent(ComponentPtr const& component);

		
		void SetParent(GameObjectPtr const& parent)
		{
			_parent = parent;
		}

		void AddChild(GameObjectPtr const& child,Vector3 const& pos = Vector3::ZERO,Quaternion const& rot = Quaternion::ZERO,Vector3 const& scale = Vector3::UNIT_SCALE);

		void LocalTransform(Vector3 const& pos = Vector3::ZERO,Quaternion const& rot = Quaternion::ZERO,Vector3 const& scale = Vector3::UNIT_SCALE)
		{
			_position = pos;
			_scale = scale;
			_rotation = rot;
		}

		const Matrix4& GetWorldMatrix() const
		{
			return _worldMatrix;
		}

		void Tick(float deltaSeconds);

	private:

	    Vector3 _position;
		Vector3 _scale;
		Quaternion _rotation;
		Matrix4 _localMatrix;
		Matrix4 _worldMatrix;

	private:
		std::vector<ComponentPtr> _vComponents;

		boost::weak_ptr<GameObject> _parent;
		std::vector<GameObjectPtr> _vChildren;

	


	};
}




#endif