#include "EngineInclude.h"

namespace Disorder
{
	RasterizeStatePtr RenderResourceManager::DefaultRasterizeState = 0;
	BlendStatePtr RenderResourceManager::DefaultBlentState = 0;
	DepthStencilStatePtr RenderResourceManager::DefaultDepthStencilState = 0;

	void RenderResourceManager::Exit()
	{
		RenderResourceManager::DefaultRasterizeState = NULL;
		RenderResourceManager::DefaultBlentState = NULL;
		RenderResourceManager::DefaultDepthStencilState = NULL;

		_shaderMap.empty();
		_propertyManagerMap.empty();
	}

	void RenderResourceManager::Init()
	{
		RasterizeDesc rasterDesc;
		DefaultRasterizeState = CreateRasterizeState(&rasterDesc);

	    BlendDesc blendDesc;
		DefaultBlentState = CreateBlendState(&blendDesc,1);

		DepthStencilDesc dsDesc;
		DefaultDepthStencilState = CreateDepthStencilState(&dsDesc,0);

		RegisterPropertyManager(ShaderPropertyManager::sManagerCamera);
		RegisterPropertyManager(ShaderPropertyManager::sManagerObject);
		RegisterPropertyManager(ShaderPropertyManager::sManagerMaterial);
		RegisterPropertyManager(ShaderPropertyManager::sManagerDirectionLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerForwardFourLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerScene);
		RegisterPropertyManager(ShaderPropertyManager::sManagerGlobal);
	}
 
	ShaderPropertyManagerPtr RenderResourceManager::GetPropertyManager(std::string const& name)
	{
		if( _propertyManagerMap.find(name) != _propertyManagerMap.end() )
			return _propertyManagerMap.at(name);

		return _propertyManagerMap.at(ShaderPropertyManager::sManagerGlobal);
	}

	void RenderResourceManager::DumpContent()
	{
		ShaderMap::const_iterator shaderIter = _shaderMap.begin();
		while (shaderIter != _shaderMap.end() )
		{
			shaderIter->second->DumpContent();
			++shaderIter;
		}

		ShaderPropertyManagerMap::const_iterator iter = _propertyManagerMap.begin();
		while (iter != _propertyManagerMap.end() )
		{
			iter->second->DumpContent();
			++iter;
		}
	}
}