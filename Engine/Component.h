#ifndef _DISORDER_COMPONENT_H_
#define _DISORDER_COMPONENT_H_


namespace Disorder
{
 
	enum EComponentType
	{
		CT_Camera = 0,
		CT_Light = 1,
		CT_Renderer = 2
	};

	class GameObject;

	class Component
	{
	public:
 
		Component(std::string const& name,EComponentType componentType);

		virtual ~Component(){};
	   inline void SetBase(GameObject* base)
	   {
		   _baseObject = base;
	   }

	   GameObject* GetBase()
	   {
		   return _baseObject;
	   }

	   std::string Name;
	   EComponentType ComponentType;

	protected:
		glm::vec3 _vPos;
		glm::vec3 _vScale;
		glm::quat _vRot;
		GameObject* _baseObject;

		// offset transform to gameobject , when using instance, set it to identity
	
	};
}



#endif