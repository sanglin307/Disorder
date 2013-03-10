#ifndef _DISORDER_CONFIG_H_
#define _DISORDER_CONFIG_H_

namespace Disorder
{
 
	class Config : public Singleton<Config>
	{
	public:
		static std::string sRunningDictioary; 

		static RenderConfig sRenderConfig; 

	};

	#define GConfig Config::GetSingleton()
}

#endif