#include "EngineInclude.h"

namespace Disorder
{
	ShadowMap::ShadowMap(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		_bUseGeometryShader = true;

		_propertyMgr = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerShadowMapGen);
	
		_viewMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapView, eSP_Float,16);
		_viewArrayMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapViewArray, eSP_Float, 16 * 6);
		_projMatrix = _propertyMgr->CreateProperty(ShaderPropertyManager::sShadowMapProj, eSP_Float,16);
 
		ShaderPropertyManager* globalProperty = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_shadowTexture2D = globalProperty->CreateProperty(ShaderPropertyManager::sShadowMapTexture2D, eSP_ShaderResource, 1);
		_shadowTextureCube = globalProperty->CreateProperty(ShaderPropertyManager::sShadowMapTextureCube, eSP_ShaderResource, 1);
		_shadowSampler = globalProperty->CreateProperty(ShaderPropertyManager::sShadowSampler, eSP_SampleState, 1);
 
		_DepthGenEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObject* vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "ShadowMap", SM_4_0, "DepthVertexShader");
		_DepthGenEffect->BindShader(vertexShader);

		_DepthCubeGenEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObject* vertexShaderCube = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "ShadowMap", SM_4_0, "DepthCubeMapVS");
		ShaderObject* geometryShader = GEngine->RenderResourceMgr->CreateShader(ST_GeometryShader, "ShadowMap", SM_4_0, "DepthCubeMapGS");
		_DepthCubeGenEffect->BindShader(vertexShaderCube);
		_DepthCubeGenEffect->BindShader(geometryShader);
		if (GConfig->pRenderConfig->RenderEngine == RET_OpenGL)  // for openGL , must define a pixel shader when we need rasterization.
		{
			ShaderObject* pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "ShadowMap", SM_4_0, "DepthPixelShader");
			_DepthGenEffect->BindShader(pixelShader);
			_DepthCubeGenEffect->BindShader(pixelShader);
		}

		_DepthCubeGenEffect->LinkShaders();
		_DepthGenEffect->LinkShaders();

		RasterizeDesc rDesc;
		rDesc.DepthBias = 50;
		rDesc.SlopeScaledDepthBias = 27.0f;
		RasterizeState* rasterObject= GEngine->RenderResourceMgr->CreateRasterizeState(&rDesc);
		_DepthGenEffect->BindRasterizeState(rasterObject);
		_DepthCubeGenEffect->BindRasterizeState(rasterObject);
		 
		SamplerDesc sDesc;
		sDesc.CompareFunc = CF_Less_Equal;
		sDesc.AddressU = sDesc.AddressV = sDesc.AddressW = TAM_Border;
		sDesc.Filter = SF_Min_Mag_Mip_Linear;
		sDesc.MaxAnisotropy = 0;
		sDesc.CompareTypeSampler = true;
		_shadowSamplerState = GEngine->RenderResourceMgr->CreateSamplerState(&sDesc);

		_shadowDataTex2D = GEngine->RenderResourceMgr->CreateTexture2D(_shadowSamplerState, PF_R32_TYPELESS, width, height, false, false, SV_DepthStencil | SV_ShaderResource, 1, NULL,0);
		_depthView2D = GEngine->RenderResourceMgr->CreateSurfaceView(SV_DepthStencil, _shadowDataTex2D, PF_D32_FLOAT);
		_shaderView2D = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _shadowDataTex2D, PF_R32_FLOAT);

		std::map<ESurfaceLocation, SurfaceView*> viewMap;
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_DepthStencil, _depthView2D));
		_depthSurface2D = GEngine->RenderResourceMgr->CreateRenderSurface(viewMap);

		unsigned int viewFlag = SF_AsCubeMap;
		if (!_bUseGeometryShader)
			viewFlag |= SF_MultiSliceView;
		_shadowDataTexCube = GEngine->RenderResourceMgr->CreateTexture2D(_shadowSamplerState, PF_R32_TYPELESS, width, height, false, false, SV_DepthStencil | SV_ShaderResource, 6, NULL, viewFlag);
		_depthViewCube = GEngine->RenderResourceMgr->CreateSurfaceView(SV_DepthStencil, _shadowDataTexCube, PF_D32_FLOAT, viewFlag);
		_shaderViewCube = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _shadowDataTexCube, PF_R32_FLOAT, viewFlag);

		//_shadowRenderTexCube = GEngine->RenderResourceMgr->CreateTexture2D(_shadowSamplerState, PF_R32_TYPELESS, width, height, false, false, SV_RenderTarget | SV_ShaderResource, 6, NULL, SF_AsCubeMap);
		//_renderViewCube = GEngine->RenderResourceMgr->CreateSurfaceView(SV_RenderTarget, _shadowRenderTexCube, PF_R32_FLOAT);
		//_shaderRenderViewCube = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _shadowRenderTexCube, PF_R32_FLOAT, SF_AsCubeMap);

		viewMap.clear();
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_DepthStencil, _depthViewCube));
		//viewMap.insert(std::pair<ESurfaceLocation, SurfaceViewPtr>(SL_RenderTarget1, _renderViewCube));
		_depthSurfaceCube = GEngine->RenderResourceMgr->CreateRenderSurface(viewMap);

	
		_shadowTexture2D->SetData(_shaderView2D);
	

	}

	void ShadowMap::PrepareRenderLight(const Light* light)
	{
		_shadowSampler->SetData(_shadowSamplerState);
		if (light->CastShadows)
		{
			if (light->LightType == LT_Point)
			{
				_shadowTextureCube->SetData(_shaderViewCube);
			}
			else
			   _shadowTexture2D->SetData(_shaderView2D);
		}
	}

	void ShadowMap::RenderDepth(Camera* camera, const std::vector<GeometryRenderer*>& geometryList, Light* light)
	{	 
		if (light->LightType == LT_Spot)
		{
			SpotLight* spot = (SpotLight*)light;
			_viewMatrix->SetData(glm::value_ptr(spot->ShadowViewMatrix));
			_projMatrix->SetData(glm::value_ptr(spot->ShadowProjMatrix));
			_propertyMgr->UpdateShaderProperty();

			//prepared buffer
			GEngine->RenderEngine->SetEffect(NULL);
			GEngine->RenderEngine->SetRenderTarget(_depthSurface2D);
			GEngine->RenderEngine->ClearRenderSurface(_depthSurface2D, glm::vec4(0), true, 1.0f, false, 0);
			GEngine->RenderEngine->SetViewport((float)_width, (float)_height, 0.f, 1.f, 0, 0);
			for (size_t j = 0; j < geometryList.size(); j++)
			{
				if (!geometryList[j]->GetGeometry()->CastShadow)
					continue;

				if (!light->Touch(geometryList[j]))
					continue;

				geometryList[j]->UpdateShaderProperty();
				geometryList[j]->RenderShadow(camera, _DepthGenEffect);
			}
		}
		else if (light->LightType == LT_Directional)
		{
			DirectionLight* dirLight = (DirectionLight*)light;
			_viewMatrix->SetData(glm::value_ptr(dirLight->ShadowViewMatrix));
			_projMatrix->SetData(glm::value_ptr(dirLight->ShadowProjMatrix));
			_propertyMgr->UpdateShaderProperty();

			//prepared buffer
			GEngine->RenderEngine->SetEffect(NULL);
			GEngine->RenderEngine->SetRenderTarget(_depthSurface2D);
			GEngine->RenderEngine->ClearRenderSurface(_depthSurface2D, glm::vec4(0), true, 1.0f, false, 0);
			GEngine->RenderEngine->SetViewport((float)_width, (float)_height, 0.f, 1.f, 0, 0);

			for (size_t j = 0; j < geometryList.size(); j++)
			{
				if (!geometryList[j]->GetGeometry()->CastShadow)
					continue;

				if (!light->Touch(geometryList[j]))
					continue;

				geometryList[j]->UpdateShaderProperty();
				geometryList[j]->RenderShadow(camera, _DepthGenEffect);
			}
		}
		else if ( light->LightType == LT_Point )
		{
			PointLight* pLight = (PointLight*)light;

			if (_bUseGeometryShader)
			{
				_viewArrayMatrix->SetData(glm::value_ptr(pLight->ShadowViewMatrix[0]));
				_projMatrix->SetData(glm::value_ptr(pLight->ShadowProjMatrix));

				_propertyMgr->UpdateShaderProperty();
				//prepared buffer
				GEngine->RenderEngine->SetRenderTarget(_depthSurfaceCube);
				GEngine->RenderEngine->ClearRenderSurface(_depthSurfaceCube, glm::vec4(0), true, 1.0f, false, 0);
				GEngine->RenderEngine->SetViewport((float)_width, (float)_height, 0.f, 1.f, 0, 0);

				for (size_t j = 0; j < geometryList.size(); j++)
				{
					if (!geometryList[j]->GetGeometry()->CastShadow)
						continue;

					if (!light->Touch(geometryList[j]))
						continue;

					geometryList[j]->UpdateShaderProperty();
					geometryList[j]->RenderShadow(camera, _DepthCubeGenEffect);
				}
			}
			else
			{
				GEngine->RenderEngine->SetViewport((float)_width, (float)_height, 0.f, 1.f, 0, 0);
				_viewArrayMatrix->SetData(glm::value_ptr(pLight->ShadowViewMatrix[0]));
				for (size_t slice = 0; slice < 6; slice++)
				{
					_viewMatrix->SetData(glm::value_ptr(pLight->ShadowViewMatrix[slice]));
					_projMatrix->SetData(glm::value_ptr(pLight->ShadowProjMatrix));
					_propertyMgr->UpdateShaderProperty();
					//prepared buffer
					GEngine->RenderEngine->SetRenderTarget(_depthSurfaceCube,slice);
					GEngine->RenderEngine->ClearRenderSurface(_depthSurfaceCube, glm::vec4(0), true, 1.0f, false, 0,slice);
					for (size_t j = 0; j < geometryList.size(); j++)
					{
						if (!geometryList[j]->GetGeometry()->CastShadow)
							continue;

						if (!light->Touch(geometryList[j]))
							continue;

						geometryList[j]->UpdateShaderProperty();
						geometryList[j]->RenderShadow(camera, _DepthGenEffect);
					}
				}
			}
		}
	}
 
}