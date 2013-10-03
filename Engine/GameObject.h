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

		const std::string & GetName() const
		{
			return _name;
		}
 
		void Tick(float deltaSeconds);

		void SetParent(GameObjectPtr const& parent)
		{
			_parent = parent;
		}

		void AddChild(GameObjectPtr const& child,Vector3 const& pos,Quaternion const& rot,Vector3 const& scale);
		void AddChild(GameObjectPtr const& child);

		unsigned int GetChildCount()
		{
			return _mapChildren.size();
		}
		
		GameObjectPtr GetChild(std::string const& name);
		
		void LocalPitch(float radian);
		void LocalRoll(float radian);
		void LocalYaw(float radian);
		void LocalRotate(Vector3 const& axis, float radian);
		void LocalTranslate(const Vector3& delta);
		void LocalScale(const Vector3& delta);

		void SetLocalRotation(Quaternion const& rot);
		void SetLocalPosition(Vector3 const& position);
		void SetLocalScale(Vector3 const& scale);
		
		const Vector3& GetLocalPosition() const {  return _locPos; }
		const Quaternion& GetLocalRotation() const {  return _locRot; }
		const Vector3& GetLocalScale() const    { return _locScale;}

		const Matrix4& GetWorldMatrix() const   { return _worldMatrix;}

		const Vector3& GetWorldPosition() const { return _wldPos;}
		const Quaternion& GetWorldRotation() const { return _wldRot;} 
		const Vector3& GetWorldScale() const    { return _wldScale;}

	    void WorldPitch(float radian);
		void WorldRoll(float radian);
		void WorldYaw(float radian);
		void WorldRotate(const Vector3& axis,float radian);
		void WorldTranslate(const Vector3& delta);
		void WorldScale(const Vector3& delta);
		void SetWorldRotation(Quaternion const& rot);
		void SetWorldPosition(Vector3 const& position);
 
		Vector3 WorldToLocalPosition( const Vector3 &worldPos );
	    Vector3 LocalToWorldPosition( const Vector3 &localPos );
	    Quaternion WorldToLocalRotation( const Quaternion &worldRot );
		Quaternion LocalToWorldRotation( const Quaternion &localRot );
	 
		void RefreshWorldTransform();

	private:

		std::string _name;
		std::vector<ComponentPtr> _vComponents;

		boost::weak_ptr<GameObject> _parent;
		std::map<std::string,GameObjectPtr> _mapChildren;
		
	    Vector3 _locPos;
		Quaternion _locRot;
		Vector3 _locScale;

		Vector3 _wldPos;
		Quaternion _wldRot;
		Vector3 _wldScale;
		Matrix4 _worldMatrix;

 
	};
}




#endif