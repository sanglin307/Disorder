#ifndef _DISORDER_TRANSFORM_H_
#define _DISORDER_TRANSFORM_H_


namespace Disorder
{
	class Transform : public boost::enable_shared_from_this<Transform>
	{
	public:

		Transform();
		Transform(Vector3 const& pos,Vector3 const& rot,Vector3 const& scale);

		GameObjectPtr& GetTarget() const
		{
			return _target.lock();
		}

		void SetParent(TransformPtr const& parent)
		{
			_parent = parent;
		}

		void AddChild(TransformPtr const& child,Vector3 const& pos,Vector3 const& rot,Vector3 const& scale);
		void AddChild(TransformPtr const& child);

		unsigned int GetChildCount()
		{
			return _vChildren.size();
		}
		
		TransformPtr GetChild(unsigned int index)
		{
			if( index < _vChildren.size())
				return _vChildren[index];

			return NULL;
		}

		void SetLocalRotation(float pitch,float yaw,float roll);
		void SetLocalPosition(Vector3 const& position);
		void SetLocalScale(Vector3 const& scale);
		
		const Vector3& GetLocalPosition() const {  return _locPos; }
		const Vector3& GetLocalRotation() const {  return _locRot; }
		const Vector3& GetLocalScale() const    { return _locScale;}

		const Matrix4& GetWorldMatrix() const   { return _worldMatrix;}

		const Vector3& GetWorldPosition() const { return _wldPos;}
		const Vector3& GetWorldRotation() const { return _wldRot;} 
		const Vector3& GetWorldScale() const    { return _wldScale;}

		void SetWorldRotation(float pitch,float yaw,float roll);
		void SetWorldPosition(Vector3 const& position);
		void SetWorldScale(Vector3 const& scale);
 
		void RefreshWorldTransform();
		void RefreshLocalTransform();

	private:

		Vector3 _locPos;
		Vector3 _locScale;
		Vector3 _locRot;
	 
		Vector3 _wldPos;
		Vector3 _wldScale;
		Vector3 _wldRot;
		Matrix4 _worldMatrix;

		boost::weak_ptr<GameObject> _target;
		boost::weak_ptr<Transform> _parent;
		std::vector<TransformPtr> _vChildren;
	};
}


#endif