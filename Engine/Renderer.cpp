#include "EngineInclude.h"

namespace Disorder
{

 
	//====================================================

	CubeRenderer::CubeRenderer(GameObjectPtr const& base,float length)
	{
		SetBase(base);

		//compile shaders
		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		RenderTechniquePtr technique =  resourceManager->CreateRenderTechnique("SimpleFX_PT.fx",SM_4_0,"VS","PS");

		VertexShaderPtr vertexShader = technique->GetVertexShader();
		PixelShaderPtr pixelShader = technique->GetPixelShader();
		
		VertexInputDes vertexElementDes[] =
		{
			{VIS_Position,PF_R32G32B32F,0,false},
			{VIS_Normal,  PF_R32G32B32F,0,false},
			{VIS_TexCoord0,PF_R32G32F,0,false},
		};

		RenderLayoutPtr renderLayout = resourceManager->CreateRenderLayout(vertexShader,vertexElementDes,3,TT_TriangleList);
	
		 
		SimpleVertex vertexInit[] =
		{
			{ Vector3( -1.0f, 1.0f, -1.0f )*length, Vector3(0.0f,1.0f,0.0f),  Vector2( 0.0f, 0.0f ) },
			{ Vector3( 1.0f, 1.0f, -1.0f )*length,  Vector3(0.0f,1.0f,0.0f),  Vector2( 1.0f, 0.0f ) },
			{ Vector3( 1.0f, 1.0f, 1.0f )*length,   Vector3(0.0f,1.0f,0.0f),  Vector2( 1.0f, 1.0f ) },
			{ Vector3( -1.0f, 1.0f, 1.0f )*length,  Vector3(0.0f,1.0f,0.0f),  Vector2( 0.0f, 1.0f ) },

			{ Vector3( -1.0f, -1.0f, -1.0f )*length,Vector3( 0.0f, -1.0f, 0.0f),Vector2( 0.0f, 0.0f )},
			{ Vector3( 1.0f, -1.0f, -1.0f )*length, Vector3( 0.0f, -1.0f, 0.0f),Vector2( 1.0f, 0.0f ) },
			{ Vector3( 1.0f, -1.0f, 1.0f )*length,  Vector3( 0.0f, -1.0f, 0.0f),Vector2( 1.0f, 1.0f ) },
			{ Vector3( -1.0f, -1.0f, 1.0f )*length, Vector3( 0.0f, -1.0f, 0.0f),Vector2( 0.0f, 1.0f ) },

			{ Vector3( -1.0f, -1.0f, 1.0f )*length, Vector3(-1.0f,0.0f,0.0f), Vector2( 0.0f, 0.0f ) },
			{ Vector3( -1.0f, -1.0f, -1.0f )*length,Vector3(-1.0f,0.0f,0.0f), Vector2( 1.0f, 0.0f ) },
			{ Vector3( -1.0f, 1.0f, -1.0f )*length, Vector3(-1.0f,0.0f,0.0f), Vector2( 1.0f, 1.0f ) },
			{ Vector3( -1.0f, 1.0f, 1.0f )*length,  Vector3(-1.0f,0.0f,0.0f), Vector2( 0.0f, 1.0f ) },

			{ Vector3( 1.0f, -1.0f, 1.0f )*length,  Vector3(1.0f,0.0f,0.0f), Vector2( 0.0f, 0.0f ) },
			{ Vector3( 1.0f, -1.0f, -1.0f )*length, Vector3(1.0f,0.0f,0.0f), Vector2( 1.0f, 0.0f ) },
			{ Vector3( 1.0f, 1.0f, -1.0f )*length,  Vector3(1.0f,0.0f,0.0f), Vector2( 1.0f, 1.0f ) },
			{ Vector3( 1.0f, 1.0f, 1.0f )*length,   Vector3(1.0f,0.0f,0.0f), Vector2( 0.0f, 1.0f ) },

			{ Vector3( -1.0f, -1.0f, -1.0f )*length,Vector3(0.0f, 0.0f, -1.0f), Vector2( 0.0f, 0.0f ) },
			{ Vector3( 1.0f, -1.0f, -1.0f )*length, Vector3(0.0f, 0.0f, -1.0f), Vector2( 1.0f, 0.0f ) },
			{ Vector3( 1.0f, 1.0f, -1.0f )*length,  Vector3(0.0f, 0.0f, -1.0f), Vector2( 1.0f, 1.0f ) },
			{ Vector3( -1.0f, 1.0f, -1.0f )*length, Vector3(0.0f, 0.0f, -1.0f), Vector2( 0.0f, 1.0f ) },

			{ Vector3( -1.0f, -1.0f, 1.0f )*length, Vector3( 0.0f, 0.0f, 1.0f), Vector2( 0.0f, 0.0f ) },
			{ Vector3( 1.0f, -1.0f, 1.0f )*length,  Vector3( 0.0f, 0.0f, 1.0f), Vector2( 1.0f, 0.0f ) },
			{ Vector3( 1.0f, 1.0f, 1.0f )*length,   Vector3( 0.0f, 0.0f, 1.0f), Vector2( 1.0f, 1.0f ) },
			{ Vector3( -1.0f, 1.0f, 1.0f )*length,  Vector3( 0.0f, 0.0f, 1.0f), Vector2( 0.0f, 1.0f ) },
		};
		BufferInitData vertexInitData;
		vertexInitData.Data = vertexInit;
		vertexInitData.RowPitch = 0;
		vertexInitData.SlicePitch = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,sizeof(SimpleVertex),sizeof(SimpleVertex)*24,BAH_GPU_Read,&vertexInitData);
		renderLayout->BindVertexBuffer(vertexBuffer);

		//Index buffer
		WORD indices[] =
		{
			3,1,0,
			2,1,3,

			6,4,5,
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
		};
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
		_renderTechnique = technique;
	}

	void CubeRenderer::Draw()
	{
		BOOST_ASSERT(_renderLayout != NULL && _renderTechnique != NULL );

		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);

		renderEngine->UpdateMVPMatrix(_renderTechnique,gameObject->GetWorldMatrix(),GSceneManager->SceneCamera->ViewMatrix,GSceneManager->SceneCamera->ProjectMatrix);
	
		renderEngine->SetFX(_renderTechnique);
		renderEngine->DrawIndexed(36,0,0);

	}

	void GeometryRenderer::SetGeometry(GeometryPtr const& geometry)
	{
		_geometryObject = geometry;
	}

	void GeometryRenderer::Draw()
	{
	}
}