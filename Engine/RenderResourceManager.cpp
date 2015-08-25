#include "EngineInclude.h"

namespace Disorder
{
	RenderResourceManager* GRenderResourceMgr = NULL;

	RenderResourceManager::~RenderResourceManager()
	{
		DefaultRasterizeState = NULL;
		DefaultBlentState = NULL;
		DefaultDepthStencilState = NULL;
		DefaultWhiteTexture2D = NULL;
		DefaultSamplerState = NULL;

		ShaderMap::const_iterator shaderIter = _shaderMap.begin();
		while (shaderIter != _shaderMap.end())
		{
			delete shaderIter->second;
			++shaderIter;
		}
		_shaderMap.empty();

		ShaderPropertyManagerMap::const_iterator pIter = _propertyManagerMap.begin();
		while (pIter != _propertyManagerMap.end())
		{
			delete pIter->second;
			++pIter;
		}
		_propertyManagerMap.empty();

		DepthStencilMap::const_iterator dIter = _depthStencilMap.begin();
		while (dIter != _depthStencilMap.end())
		{
			delete dIter->second;
			++dIter;
		}
		_depthStencilMap.empty();

		SamplerStateMap::const_iterator sIter = _samplerStateMap.begin();
		while (sIter != _samplerStateMap.end())
		{
			delete sIter->second;
			++sIter;
		}
		_samplerStateMap.empty();

		RaterizeStateMap::const_iterator rIter = _rasterizeStateMap.begin();
		while (rIter != _rasterizeStateMap.end())
		{
			delete rIter->second;
			++rIter;
		}
		_rasterizeStateMap.empty();
		
		BlendStateMap::const_iterator bIter = _blendStateMap.begin();
		while (bIter != _blendStateMap.end())
		{
			delete bIter->second;
			++bIter;
		}
		_blendStateMap.empty();

		RenderLayoutMap::const_iterator layoutIter = _renderLayoutMap.begin();
		while (layoutIter != _renderLayoutMap.end())
		{
			delete layoutIter->second;
			++layoutIter;
		}
		_renderLayoutMap.empty();

		RenderBufferMap::const_iterator bufferIter = _renderBufferMap.begin();
		while (bufferIter != _renderBufferMap.end())
		{
			delete bufferIter->second;
			++bufferIter;
		}
		_renderBufferMap.empty();

		RenderTexture2DMap::const_iterator texIter = _renderTexture2DMap.begin();
		while (texIter != _renderTexture2DMap.end())
		{
			delete texIter->second;
			++texIter;
		}
		_renderTexture2DMap.empty();	
	}
 
	ShaderPropertyManager* RenderResourceManager::GetPropertyManager(std::string const& name)
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