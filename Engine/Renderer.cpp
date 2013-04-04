#include "EngineInclude.h"

namespace Disorder
{

 
	//====================================================

	CubeRenderer::CubeRenderer(GameObjectPtr const& base,float length)
	{
		SetBase(base);

		//compile shaders
		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		RenderEffectPtr technique =  resourceManager->CreateRenderEffect("SimpleFX_PT.fx",SM_4_0,"VS","PS");

		ShaderObjectPtr vertexShader = technique->GetVertexShader();
		ShaderObjectPtr pixelShader = technique->GetPixelShader();
		
		VertexInputDes vertexElementDes[] =
		{
			{VIS_Position,PF_R32G32B32F,0,false},
			{VIS_Normal,  PF_R32G32B32F,0,false},
			{VIS_TexCoord0,PF_R32G32F,0,false},
		};

		RenderLayoutPtr renderLayout = resourceManager->CreateRenderLayout(vertexShader,vertexElementDes,3,TT_TriangleList);
 
		BufferInitData vertexInitData;
		vertexInitData.Data = vertexInit;
		vertexInitData.RowPitch = 0;
		vertexInitData.SlicePitch = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,sizeof(SimpleVertex),sizeof(SimpleVertex)*24,BAH_GPU_Read,&vertexInitData);
		renderLayout->BindVertexBuffer(vertexBuffer);

		 
		BufferInitData indexInitData;
		indexInitData.Data = indices;
		indexInitData.RowPitch = 0;
		indexInitData.SlicePitch = 0;
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,sizeof(WORD),sizeof(WORD)*36,BAH_GPU_Read,&indexInitData);
		renderLayout->BindIndexBuffer(indexBuffer);

		//Const create and used in vertex shader.
		RenderBufferPtr constBuffer = resourceManager->CreateRenderBuffer(RBT_Constant,sizeof(Matrix4),sizeof(Matrix4)*3,BAH_GPU_Read,0);
		vertexShader->BindConstBuffer(constBuffer);

		//shader view
		RenderViewPtr renderView = resourceManager->CreateTexture2DViewFromFile("seafloor.dds");
	    SamplerStatePtr sampler = resourceManager->CreateSamplerState(SF_Point,TAM_Wrap,0);
		pixelShader->BindSamplerState(sampler);
		pixelShader->BindShaderResource(renderView);

		_renderLayout = renderLayout;
		_renderEffect = technique;
	}

	void CubeRenderer::Draw()
	{
		BOOST_ASSERT(_renderLayout != NULL && _renderEffect != NULL );

		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);

		renderEngine->UpdateMVPMatrix(_renderEffect,gameObject->GetWorldMatrix(),GSceneManager->SceneCamera->ViewMatrix,GSceneManager->SceneCamera->ProjectMatrix);
	
		renderEngine->SetFX(_renderEffect);
		renderEngine->DrawIndexed(36,0,0);

	}

	GeometryRenderer::GeometryRenderer(GameObjectPtr const& base)
	{
		SetBase(base);
	}

	void GeomotryRenderer::BuildRenderResource()
	{
		BOOST_ASSERT(_geometryObject != NULL && _material != NULL);

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;

		//compile shader
		ShaderObjectPtr vertexShader = _material->Effect[MVT_Perspective]->GetVertexShader();
		ShaderObjectPtr pixelShader = _material->Effect[MVT_Perspective]->GetPixelShader();
		
		std::vector<VertexInputDes> vertexElement;
		if( _geometryObject->Positions.size() > 0 )
		{
			VertexInputDes pos;
			pos.Semantic = VIS_Position;
			pos.Format = PF_R32G32B32F;
			pos.InstanceDataStepRate = 0;
			pos.InstanceData = false;
			vertexElement.push_back(pos);
		}

		if(_geometryObject->Normals.size()> 0 )
		{
			VertexInputDes normal;
			normal.Semantic = VIS_Normal;
			normal.Format = PF_R32G32B32F;
			normal.InstanceDataStepRate = 0;
			normal.InstanceData = false;
			vertexElement.push_back(normal);
		}

		if(_geometryObject->Texcoords.size()> 0 )
		{
			VertexInputDes tex;
			tex.Semantic = VIS_TexCoord0;
			tex.Format = PF_R32G32F;
			tex.InstanceDataStepRate = 0;
			tex.InstanceData = false;
			vertexElement.push_back(tex);
		}

	 
		RenderLayoutPtr renderLayout = resourceManager->CreateRenderLayout(vertexShader,vertexElement,TT_TriangleList);
 
		BufferInitData vertexInitData;
		vertexInitData.Data = vertexInit;
		vertexInitData.RowPitch = 0;
		vertexInitData.SlicePitch = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,sizeof(SimpleVertex),sizeof(SimpleVertex)*24,BAH_GPU_Read,&vertexInitData);
		renderLayout->BindVertexBuffer(vertexBuffer);

		 
		BufferInitData indexInitData;
		indexInitData.Data = indices;
		indexInitData.RowPitch = 0;
		indexInitData.SlicePitch = 0;
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,sizeof(WORD),sizeof(WORD)*36,BAH_GPU_Read,&indexInitData);
		renderLayout->BindIndexBuffer(indexBuffer);

		//Const create and used in vertex shader.
		RenderBufferPtr constBuffer = resourceManager->CreateRenderBuffer(RBT_Constant,sizeof(Matrix4),sizeof(Matrix4)*3,BAH_GPU_Read,0);
		vertexShader->BindConstBuffer(constBuffer);

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
	}
}