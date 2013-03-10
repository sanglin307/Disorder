#ifndef _DISORDER_SINGLETON_H_
#define _DISORDER_SINGLETON_H_

namespace Disorder
{
	#define InitialiseSingleton( type ) \
	template <> boost::shared_ptr<type> Singleton < type > :: _pSingleton

   /* #define InitialiseTemplateSingleton( temp, type ) \
    template <> temp< type > * Singleton < temp< type > > :: _pSingleton = 0*/

	template <class type> 
	class Singleton 
	{
 
	public:

		  static boost::shared_ptr<type> const& GetSingleton( ) 
		  {  
			  if( !_pSingleton )
				  _pSingleton = boost::make_shared<type>();
			  return _pSingleton; 
		  }
 
	protected:
		  static boost::shared_ptr<type> _pSingleton;
	};
}

#endif