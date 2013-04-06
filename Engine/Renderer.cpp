#include "EngineInclude.h"

namespace Disorder
{

	GeometryRenderer::GeometryRenderer(GameObjectPtr const& base)
	{
		SetBase(base);
	}

	void GeometryRenderer::BuildRenderResource()
	{
		BOOST_ASSERT(_geometryObject != NULL && _material != NULL);

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;

		WorldViewProjMatrix = _material->Effect[MVT_Perspective]->GetMatrixParameter("WorldViewProjMatrix");
		WorldMatrix = _material->Effect[MVT_Perspective]->GetMatrixParameter("WorldMatrix");
		ViewMatrix = _material->Effect[MVT_Perspective]->GetMatrixParameter("ViewMatrix");
		ProjMatrix =  _material->Effect[MVT_Perspective]->GetMatrixParameter("ProjMatrix");
 
		//compile shader
		ShaderObjectPtr vertexShader = _material->Effect[MVT_Perspective]->GetVertexShader();
		ShaderObjectPtr pixelShader = _material->Effect[MVT_Perspective]->GetPixelShader();
 
		RenderLayoutPtr renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList);
 
		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read,_geometryObject);
		renderLayout->BindVertexBuffer(vertexBuffer);

		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read,_geometryObject);
		renderLayout->BindIndexBuffer(indexBuffer);
 
		//shader view
		RenderViewPtr renderView = resourceManager->CreateTexture2DViewFromFile("seafloor.dds");
	    SamplerStatePtr sampler = resourceManager->CreateSamplerState(SF_Point,TAM_Wrap,0);
		pixelShader->BindSamplerState(sampler);
		pixelShader->BindShaderResource(renderView);
	}

	void GeometryRenderer::SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat)
	{
		_geometryObject = geometry;
		_material = mat;

		BuildRenderResource();
	}

	void GeometryRenderer::Draw(MaterialViewType view)
	{
		BOOST_ASSERT(_renderLayout != NULL && _renderEffect != NULL );

		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);

		renderEngine->UpdateMVPMatrix(_renderEffect,gameObject->GetWorldMatrix(),GSceneManager->SceneCamera->ViewMatrix,GSceneManager->SceneCamera->ProjectMatrix);
	
		renderEngine->SetFX(_renderEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(),0,0);
	}
}