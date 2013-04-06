#include "EngineInclude.h"

namespace Disorder
{

 
	//====================================================

	CubeRenderer::CubeRenderer(GameObjectPtr const& base,float length)
	{
		SetBase(base);

		////compile shaders
		//RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		//RenderEffectPtr technique =  resourceManager->CreateRenderEffect("SimpleFX_PT.fx",SM_4_0,"VS","PS");

		//ShaderObjectPtr vertexShader = technique->GetVertexShader();
		//ShaderObjectPtr pixelShader = technique->GetPixelShader();
		//
		//VertexInputDes vertexElementDes[] =
		//{
		//	{VIS_Position,PF_R32G32B32F,0,false},
		//	{VIS_Normal,  PF_R32G32B32F,0,false},
		//	{VIS_TexCoord0,PF_R32G32F,0,false},
		//};

		//RenderLayoutPtr renderLayout = resourceManager->CreateRenderLayout(vertexShader,vertexElementDes,3,TT_TriangleList);
 
		//BufferInitData vertexInitData;
		//vertexInitData.Data = vertexInit;
		//vertexInitData.RowPitch = 0;
		//vertexInitData.SlicePitch = 0;

		//RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,sizeof(SimpleVertex),sizeof(SimpleVertex)*24,BAH_GPU_Read,&vertexInitData);
		//renderLayout->BindVertexBuffer(vertexBuffer);

		// 
		//BufferInitData indexInitData;
		//indexInitData.Data = indices;
		//indexInitData.RowPitch = 0;
		//indexInitData.SlicePitch = 0;
		//RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,sizeof(WORD),sizeof(WORD)*36,BAH_GPU_Read,&indexInitData);
		//renderLayout->BindIndexBuffer(indexBuffer);

		////Const create and used in vertex shader.
		//RenderBufferPtr constBuffer = resourceManager->CreateRenderBuffer(RBT_Constant,sizeof(Matrix4),sizeof(Matrix4)*3,BAH_GPU_Read,0);
		//vertexShader->BindConstBuffer(constBuffer);

		////shader view
		//RenderViewPtr renderView = resourceManager->CreateTexture2DViewFromFile("seafloor.dds");
	 //   SamplerStatePtr sampler = resourceManager->CreateSamplerState(SF_Point,TAM_Wrap,0);
		//pixelShader->BindSamplerState(sampler);
		//pixelShader->BindShaderResource(renderView);

		//_renderLayout = renderLayout;
		//_renderEffect = technique;
	}

	void CubeRenderer::Draw()
	{
		

	}

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

	void GeometryRenderer::Draw()
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