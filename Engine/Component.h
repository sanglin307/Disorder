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
		virtual ~Component(){};
	   inline void SetBase(GameObjectPtr const& base)
	   {
		   _baseObject = base;
	   }

	   GameObjectPtr GetBase()
	   {
		   return _baseObject.lock();
	   }

	   std::string Name;
	   EComponentType ComponentType;

	protected:
		boost::weak_ptr<GameObject> _baseObject;
	};
}



#endif