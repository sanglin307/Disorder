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
		SamplerStatePtr pointState = GEngine->RenderResourceMgr->CreateSamplerState(SF_Point,TAM_Clamp,0);
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
}
