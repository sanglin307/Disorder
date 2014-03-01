#ifndef _DISORDER_GAMEOBJECT_H_
#define _DISORDER_GAMEOBJECT_H_


namespace Disorder
{
	class GameObject : public boost::enable_shared_from_this<GameObject>
	{
	public:
     
		std::string Name;

		~GameObject();
 
		void AddComponent(ComponentPtr const& component);
        ComponentPtr GetComponent(std::string const& name) const;
 
		void Tick(float deltaSeconds);

		void SetParent(GameObjectPtr const& parent)
		{
			_parent = parent;
		}

		void AddChild(GameObjectPtr const& child, glm::vec3 const& pos, glm::quat const& rot, glm::vec3 const& scale);
		void AddChild(GameObjectPtr const& child);

		unsigned int GetChildCount()
		{
			return _mapChildren.size();
		}
		
		GameObjectPtr GetChild(std::string const& name);
		
		void LocalPitch(float radian);
		void LocalRoll(float radian);
		void LocalYaw(float radian);
		void LocalRotate(glm::vec3 const& axis, float radian);
		void LocalTranslate(const glm::vec3& delta);
		void LocalScale(const glm::vec3& delta);

		void SetLocalRotation(glm::quat const& rot);
		void SetLocalPosition(glm::vec3 const& position);
		void SetLocalScale(glm::vec3 const& scale);
		
		const glm::vec3& GetLocalPosition() const { return _locPos; }
		const glm::quat& GetLocalRotation() const {  return _locRot; }
		const glm::vec3& GetLocalScale() const    { return _locScale; }

		const glm::mat4& GetWorldMatrix() const   { return _worldMatrix;}

		const glm::vec3& GetWorldPosition() const { return _wldPos; }
		const glm::quat& GetWorldRotation() const { return _wldRot; }
		const glm::vec3& GetWorldScale() const    { return _wldScale; }

	    void WorldPitch(float radian);
		void WorldRoll(float radian);
		void WorldYaw(float radian);
		void WorldRotate(const glm::vec3& axis, float radian);
		void WorldTranslate(const glm::vec3& delta);
		void WorldScale(const glm::vec3& delta);
		void SetWorldRotation(glm::quat const& rot);
		void SetWorldPosition(glm::vec3 const& position);
 
		glm::vec3 WorldToLocalPosition(const glm::vec3 &worldPos);
		glm::vec3 LocalToWorldPosition(const glm::vec3 &localPos);
		glm::quat WorldToLocalRotation(const glm::quat &worldRot);
		glm::quat LocalToWorldRotation(const glm::quat &localRot);
	 
		void RefreshWorldTransform();

		void UpdateShaderProperty();

		static GameObjectPtr Create(std::string const& name, glm::vec3 const& pos = glm::vec3(0), glm::quat const& rot = glm::quat(1.f,0,0,0), glm::vec3 const& scale = glm::vec3(1.f));
	private:

		GameObject(std::string const& name, glm::vec3 const& pos = glm::vec3(0), glm::quat const& rot = glm::quat(1.f, 0, 0, 0), glm::vec3 const& scale = glm::vec3(1.f));

		std::vector<ComponentPtr> _vComponents;
		boost::weak_ptr<GameObject> _parent;
		std::map<std::string,GameObjectPtr> _mapChildren;
		
		glm::vec3 _locPos;
		glm::quat _locRot;
		glm::vec3 _locScale;

		glm::vec3 _wldPos;
		glm::quat _wldRot;
		glm::vec3 _wldScale;
		glm::mat4 _worldMatrix;

		ShaderPropertyManagerPtr _propertyManager;
		ShaderPropertyPtr _worldMatrixProperty;
		ShaderPropertyPtr _worldNormalMatrixProperty;
 
	};
}




#endif