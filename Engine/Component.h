#ifndef _DISORDER_COMPONENT_H_
#define _DISORDER_COMPONENT_H_


namespace Disorder
{
 
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

	protected:
		boost::weak_ptr<GameObject> _baseObject;
	};
}



#endif