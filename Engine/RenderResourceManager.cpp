#include "EngineInclude.h"

namespace Disorder
{
	RenderResourceManager* GRenderResourceMgr = NULL;

	RasterizeState* RenderResourceManager::DefaultRasterizeState = NULL;
	BlendState* RenderResourceManager::DefaultBlentState = NULL;
	DepthStencilState* RenderResourceManager::DefaultDepthStencilState = NULL;

	SurfaceView* RenderResourceManager::DefaultWhiteTexture2D = NULL;
	SamplerState* RenderResourceManager::DefaultSamplerState = NULL;

	RenderResourceManager::~RenderResourceManager()
	{
		RenderResourceManager::DefaultRasterizeState = NULL;
		RenderResourceManager::DefaultBlentState = NULL;
		RenderResourceManager::DefaultDepthStencilState = NULL;

		RenderResourceManager::DefaultWhiteTexture2D = NULL;

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

	RenderResourceManager::RenderResourceManager()
	{
		RasterizeDesc rasterDesc;
		DefaultRasterizeState = CreateRasterizeState(&rasterDesc);

	    BlendDesc blendDesc;
		DefaultBlentState = CreateBlendState(&blendDesc,1);

		DepthStencilDesc dsDesc;
		DefaultDepthStencilState = CreateDepthStencilState(&dsDesc,0);

		SamplerDesc samplerDesc;
		DefaultSamplerState = CreateSamplerState(&samplerDesc);

		unsigned int pixelData[16];
		memset(pixelData, 0xFF, 16 * sizeof(unsigned int));
		BufferInitData data;
		data.Data = pixelData;
		data.RowPitch = 4 * 4;
		data.SlicePitch = 0;
		RenderTexture2D* tex = CreateTexture2D("DefaultTex",NULL, PF_R8G8B8A8_UNORM, 4, 4, false, false, SV_ShaderResource, 1,&data,0);
		DefaultWhiteTexture2D = CreateSurfaceView(SV_ShaderResource, tex, PF_R8G8B8A8_UNORM);

		RegisterPropertyManager(ShaderPropertyManager::sManagerCamera);
		RegisterPropertyManager(ShaderPropertyManager::sManagerObject);
		RegisterPropertyManager(ShaderPropertyManager::sManagerMaterial);
		RegisterPropertyManager(ShaderPropertyManager::sManagerDirectionLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerPointLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerSpotLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerScene);
		RegisterPropertyManager(ShaderPropertyManager::sManagerShadowMapGen);
		RegisterPropertyManager(ShaderPropertyManager::sManagerGlobal);
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