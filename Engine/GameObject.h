#ifndef _DISORDER_GAMEOBJECT_H_
#define _DISORDER_GAMEOBJECT_H_


namespace Disorder
{
	class GameObject : public boost::enable_shared_from_this<GameObject>
	{
	public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		std::string Name;

		
		~GameObject();
 
		void AddComponent(ComponentPtr const& component);
        ComponentPtr GetComponent(std::string const& name) const;
 
		void Tick(float deltaSeconds);

		void SetParent(GameObjectPtr const& parent)
		{
			_parent = parent;
		}

		void AddChild(GameObjectPtr const& child,Eigen::Vector3f const& pos,Eigen::Quaternionf const& rot,Eigen::Vector3f const& scale);
		void AddChild(GameObjectPtr const& child);

		unsigned int GetChildCount()
		{
			return _mapChildren.size();
		}
		
		GameObjectPtr GetChild(std::string const& name);
		
		void LocalPitch(float radian);
		void LocalRoll(float radian);
		void LocalYaw(float radian);
		void LocalRotate(Eigen::Vector3f const& axis, float radian);
		void LocalTranslate(const Eigen::Vector3f& delta);
		void LocalScale(const Eigen::Vector3f& delta);

		void SetLocalRotation(Eigen::Quaternionf const& rot);
		void SetLocalPosition(Eigen::Vector3f const& position);
		void SetLocalScale(Eigen::Vector3f const& scale);
		
		const Eigen::Vector3f& GetLocalPosition() const {  return _locPos; }
		const Eigen::Quaternionf& GetLocalRotation() const {  return _locRot; }
		const Eigen::Vector3f& GetLocalScale() const    { return _locScale;}

		const Eigen::Affine3f& GetWorldMatrix() const   { return _worldMatrix;}

		const Eigen::Vector3f& GetWorldPosition() const { return _wldPos;}
		const Eigen::Quaternionf& GetWorldRotation() const { return _wldRot;} 
		const Eigen::Vector3f& GetWorldScale() const    { return _wldScale;}

	    void WorldPitch(float radian);
		void WorldRoll(float radian);
		void WorldYaw(float radian);
		void WorldRotate(const Eigen::Vector3f& axis,float radian);
		void WorldTranslate(const Eigen::Vector3f& delta);
		void WorldScale(const Eigen::Vector3f& delta);
		void SetWorldRotation(Eigen::Quaternionf const& rot);
		void SetWorldPosition(Eigen::Vector3f const& position);
 
		Eigen::Vector3f WorldToLocalPosition( const Eigen::Vector3f &worldPos );
	    Eigen::Vector3f LocalToWorldPosition( const Eigen::Vector3f &localPos );
	    Eigen::Quaternionf WorldToLocalRotation( const Eigen::Quaternionf &worldRot );
		Eigen::Quaternionf LocalToWorldRotation( const Eigen::Quaternionf &localRot );
	 
		void RefreshWorldTransform();

		void UpdateShaderProperty();

		static GameObjectPtr Create(std::string const& name, Eigen::Vector3f const& pos = Eigen::Vector3f::Zero(),Eigen::Quaternionf const& rot = Eigen::Quaternionf::Identity(),Eigen::Vector3f const& scale = Eigen::Vector3f::Constant(1.f));
	private:

		GameObject(std::string const& name, Eigen::Vector3f const& pos = Eigen::Vector3f::Zero(),Eigen::Quaternionf const& rot = Eigen::Quaternionf::Identity(),Eigen::Vector3f const& scale = Eigen::Vector3f::Constant(1.f));

		std::vector<ComponentPtr> _vComponents;
		boost::weak_ptr<GameObject> _parent;
		std::map<std::string,GameObjectPtr> _mapChildren;
		
	    Eigen::Vector3f _locPos;
		Eigen::Quaternionf _locRot;
		Eigen::Vector3f _locScale;

		Eigen::Vector3f _wldPos;
		Eigen::Quaternionf _wldRot;
		Eigen::Vector3f _wldScale;
		Eigen::Affine3f _worldMatrix;

		ShaderPropertyManagerPtr _propertyManager;
		ShaderPropertyPtr _worldMatrixProperty;
		ShaderPropertyPtr _worldNormalMatrixProperty;
 
	};
}




#endif