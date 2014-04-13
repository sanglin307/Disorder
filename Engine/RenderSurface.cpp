#include "EngineInclude.h"

namespace Disorder
{ 
	RenderGBuffer::RenderGBuffer()
	{
		ShaderPropertyManagerPtr propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_GBufferDepthTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferDepthTexture,eSP_ShaderResource);
		_GBufferColorSpecIntTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferColorSpecIntTexture,eSP_ShaderResource);
		_GBufferNormalTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferNormalTexture,eSP_ShaderResource);
		_GBufferSpecPowTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferSpecPowTexture,eSP_ShaderResource);
		_SurfaceVisTex = propertyManager->CreateProperty(ShaderPropertyManager::sSurfaceVisTex,eSP_ShaderResource);
		_SurfaceSampler = propertyManager->CreateProperty(ShaderPropertyManager::sSurfaceSampler,eSP_SampleState);
		_GBufferPointSampler = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferPointSampler,eSP_SampleState);
		SamplerDesc sDesc;
		SamplerStatePtr pointState = GEngine->RenderResourceMgr->CreateSamplerState(&sDesc);
		_GBufferPointSampler->SetData(pointState);
		_SurfaceSampler->SetData(pointState);
	}

	void RenderGBuffer::UpdateShaderProperty()
	{
		_GBufferDepthTexture->SetData(DepthStencilBuffer);
		_GBufferColorSpecIntTexture->SetData(BasicColorBuffer);
		_GBufferNormalTexture->SetData(NormalDataBuffer);
		_GBufferSpecPowTexture->SetData(SpecularDataBuffer);
	}

	//void* RenderSurface::GetHandle(ESurfaceLocation location)
	//{
	//	for (size_t i = 0; i < SurfaceDes.size(); i++)
	//	{
	//		if (SurfaceDes[i].Location == location)
	//			return SurfaceDes[i].Handler;
	//	}

	//	return NULL;
	//}

	//RenderTexturePtr RenderSurface::GetResource(ESurfaceLocation location)
	//{
	//	for (size_t i = 0; i < SurfaceDes.size(); i++)
	//	{
	//		if (SurfaceDes[i].Location == location)
	//			return SurfaceDes[i].Resource;
	//	}

	//	return NULL;
	//}

	void RenderSurface::AddSurface(const RenderTexturePtr& tex, const std::vector<sRenderSurfaceDes>& des)
	{
		if (_mapSurfaces.find(tex) != _mapSurfaces.end())
			return;

		sRenderSurfaceObject obj;
		obj.Resource = tex;
		obj.vDes = des;
		_mapSurfaces.insert(std::pair<RenderTexturePtr, sRenderSurfaceObject>(tex, obj));
	}

	const std::vector<sRenderSurfaceDes>& RenderSurface::GetSurface(const RenderTexturePtr& tex) const
	{
		return _mapSurfaces.at(tex).vDes;
	}
}
