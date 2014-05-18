#include "EngineInclude.h"

namespace Disorder
{
	SurfaceViewPtr ShadowMap::sDepthBufferView = NULL;
	RenderEffectPtr ShadowMap::sDepthGenEffect = NULL;

	ShadowMapPtr ShadowMap::Create(unsigned int width, unsigned int height)
	{
		ShadowMap* pMap = new ShadowMap(width, height);
		return ShadowMapPtr(pMap);
	}

	ShadowMap::ShadowMap(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;

		_propertyMgr = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerShadowMapGen);
		_wldMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapWorld, eSP_Float,16);
		_viewMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapView, eSP_Float,16);
		_projMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapProj, eSP_Float, 16);
 
		if (sDepthGenEffect == NULL)
		{
			sDepthGenEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
			ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "ShadowMap", SM_4_0, "DepthVertexShader");
			ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "ShadowMap", SM_4_0, "DepthPixelShader");
			sDepthGenEffect->BindShader(vertexShader);
			sDepthGenEffect->BindShader(pixelShader);
			sDepthGenEffect->LinkShaders();
		}

		_shadowDataTex = GEngine->RenderResourceMgr->CreateTexture2D(NULL, PF_R32_TYPELESS, width, height, false, false, SV_RenderTarget | SV_ShaderResource, NULL);
		_targetView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_RenderTarget, _shadowDataTex, PF_R32_FLOAT);
		_shaderView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _shadowDataTex, PF_R32_FLOAT);

		if (sDepthBufferView == NULL)
		{
			RenderTexture2DPtr depthStencilTex = GEngine->RenderResourceMgr->CreateTexture2D(NULL, PF_R24G8_TYPELESS, width, height, false, false, SV_DepthStencil, NULL);
			sDepthBufferView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_DepthStencil, depthStencilTex, PF_D24_UNORM_S8_UINT);
		}

		std::map<ESurfaceLocation, SurfaceViewPtr> viewMap;
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceViewPtr>(SL_DepthStencil, sDepthBufferView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceViewPtr>(SL_RenderTarget1, _targetView));
		 
		_renderTarget = GEngine->RenderResourceMgr->CreateRenderSurface(viewMap);
	}

	void ShadowMap::PrepareRender(const glm::mat4& wldMat, const glm::mat4& viewMat, const glm::mat4& projMat)
	{
		//prepared buffer
		GEngine->RenderEngine->SetRenderTarget(_renderTarget);
		GEngine->RenderEngine->ClearRenderSurface(_renderTarget, glm::vec4(0.f, 0.f, 0.f, 1.0f), true, 1.0f, false, 0);

	}
}