#ifndef _DISORDER_COMPTR_H_
#define _DISORDER_COMPTR_H_


#ifdef WIN32
	#ifndef BOOST_MEM_FN_ENABLE_STDCALL
		#define BOOST_MEM_FN_ENABLE_STDCALL
	#endif
#endif
#include <boost/mem_fn.hpp>

namespace Disorder
{
	template <typename T> inline boost::shared_ptr<T> MakeComPtr(T* p)
	{
		return boost::shared_ptr<T>(p, boost::mem_fn(&T::Release));
	}

}

#endif