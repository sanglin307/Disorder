#include "EngineInclude.h"

namespace Disorder
{
 
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
	
		_viewMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapView, eSP_Float,16);
		_projMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapProj, eSP_Float, 16);
 
		ShaderPropertyManagerPtr globalProperty = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_shadowTexture2D = globalProperty->CreateProperty(ShaderPropertyManager::sShadowMapTexture2D, eSP_ShaderResource, 1);
		_shadowSampler = globalProperty->CreateProperty(ShaderPropertyManager::sShadowSampler, eSP_SampleState, 1);

		if (sDepthGenEffect == NULL)
		{
			sDepthGenEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
			ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "ShadowMap", SM_4_0, "DepthVertexShader");
			sDepthGenEffect->BindShader(vertexShader);
			sDepthGenEffect->LinkShaders();

			RasterizeDesc rDesc;
			rDesc.DepthBias = 25;
			//rDesc.SlopeScaledDepthBias = 20.0f;
			//rDesc.FrontCounterClockwise = false;
			RasterizeStatePtr rasterObject= GEngine->RenderResourceMgr->CreateRasterizeState(&rDesc);
			sDepthGenEffect->BindRasterizeState(rasterObject);

		
		}

		_shadowDataTex = GEngine->RenderResourceMgr->CreateTexture2D(NULL, PF_R32_TYPELESS, width, height, false, false, SV_DepthStencil | SV_ShaderResource, NULL);
		_depthView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_DepthStencil, _shadowDataTex, PF_D32_FLOAT);
		_shaderView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _shadowDataTex, PF_R32_FLOAT);

		SamplerDesc sDesc;
		sDesc.CompareFunc = CF_Less_Equal;
		sDesc.AddressU = sDesc.AddressV = sDesc.AddressW = TAM_Border;
		sDesc.Filter = SF_Min_Mag_Linear_Mip_Point;
		sDesc.MaxAnisotropy = 0;
		sDesc.CompareTypeSampler = true;
		_shadowSamplerState = GEngine->RenderResourceMgr->CreateSamplerState(&sDesc);
	

	}

	void ShadowMap::PrepareRenderLight(const LightPtr& light)
	{
		_shadowSampler->SetData(_shadowSamplerState);
		if (light->CastShadows)
			_shadowTexture2D->SetData(_shaderView);
		else
			_shadowTexture2D->SetData(GEngine->RenderResourceMgr->DefaultWhiteTexture2D);

		UpdateShaderProperty();
	}

	void ShadowMap::PrepareRenderDepth(const glm::mat4& viewMat, const glm::mat4& projMat)
	{
		_viewMat = viewMat;
		_projMat = projMat;
		UpdateShaderProperty();
		//prepared buffer
		GEngine->RenderEngine->SetRenderTarget(_depthView);
		GEngine->RenderEngine->ClearDepthStencil(_depthView, true, 1.0f, false, 0);
		GEngine->RenderEngine->SetViewport((float)_width, (float)_height, 0.f, 1.f, 0, 0);
	}

	void ShadowMap::UpdateShaderProperty()
	{
		_viewMatrix->SetData(glm::value_ptr(_viewMat));
		_projMatrix->SetData(glm::value_ptr(_projMat));
 
		glm::vec4 test = _projMat * _viewMat * glm::vec4(0, 0, 0,1);
		test = test / test.w;
		_propertyMgr->UpdateShaderProperty();
	}

	void ShadowMap::RenderObject(const CameraPtr& camera,const GeometryRendererPtr object)
	{
		object->SetRenderEffect(sDepthGenEffect);
		object->Render(camera);

	}
}