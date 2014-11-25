#include "EngineInclude.h"

namespace Disorder
{
	RenderResourceManager* GRenderResourceMgr = NULL;

	RasterizeState* RenderResourceManager::DefaultRasterizeState = NULL;
	BlendState* RenderResourceManager::DefaultBlentState = NULL;
	DepthStencilState* RenderResourceManager::DefaultDepthStencilState = NULL;

	SurfaceView* RenderResourceManager::DefaultWhiteTexture2D = NULL;
	SamplerState* RenderResourceManager::DefaultSamplerState = NULL;

	void RenderResourceManager::Exit()
	{
		RenderResourceManager::DefaultRasterizeState = NULL;
		RenderResourceManager::DefaultBlentState = NULL;
		RenderResourceManager::DefaultDepthStencilState = NULL;

		RenderResourceManager::DefaultWhiteTexture2D = NULL;

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

		SamplerDesc samplerDesc;
		DefaultSamplerState = CreateSamplerState(&samplerDesc);

		unsigned int pixelData[16];
		memset(pixelData, 0xFF, 16 * sizeof(unsigned int));
		BufferInitData data;
		data.Data = pixelData;
		data.RowPitch = 4 * 4;
		data.SlicePitch = 0;
		RenderTexture2D* tex = CreateTexture2D(NULL, PF_R8G8B8A8_UNORM, 4, 4, false, false, SV_ShaderResource, 1,&data,0);
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