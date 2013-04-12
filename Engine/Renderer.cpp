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
		 
	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList);
 
		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read,_geometryObject,vertexShader);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read,_geometryObject,vertexShader);
		_renderLayout->BindIndexBuffer(indexBuffer);
 
	}

	void GeometryRenderer::SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat)
	{
		_geometryObject = geometry;
		_material = mat;

		BuildRenderResource();
	}

	void GeometryRenderer::Draw(MaterialViewType view)
	{
		BOOST_ASSERT(_renderLayout != NULL);

		_renderEffect = _material->Effect[view];
		if( _renderEffect == NULL )
			return;

		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);

		Matrix4 worldMat = gameObject->GetWorldMatrix();
		Matrix4 viewMat = GSceneManager->SceneCamera->ViewMatrix;
		Matrix4 projMat = GSceneManager->SceneCamera->ProjectMatrix;
		Matrix4 wvpMat = worldMat*viewMat*projMat;
		
		WorldMatrix->SetValue(worldMat);
		ViewMatrix->SetValue(viewMat);
		ProjMatrix->SetValue(projMat);
		WorldViewProjMatrix->SetValue(wvpMat.transpose());
		
		//renderEngine->SetBlendState(_renderEffect->GetBlendState());
		//renderEngine->SetRasterizeState(_renderEffect->GetRasterizeState());
	 
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(),0,0);
	}
}