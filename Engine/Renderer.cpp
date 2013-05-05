#include "EngineInclude.h"

namespace Disorder
{
 
	void GeometryRenderer::BuildRenderResource()
	{
		BOOST_ASSERT(_geometryObject != NULL && _material != NULL);

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;

		WorldViewProjMatrix = _material->Effect[MVT_Perspective]->GetMatrixParameter("WorldViewProjMatrix");
		WorldMatrix = _material->Effect[MVT_Perspective]->GetMatrixParameter("World");
		ViewMatrix = _material->Effect[MVT_Perspective]->GetMatrixParameter("View");
		ProjMatrix =  _material->Effect[MVT_Perspective]->GetMatrixParameter("Projection");
		WorldNormal = _material->Effect[MVT_Perspective]->GetMatrixParameter("WorldNormal");
 
		LightType = _material->Effect[MVT_Perspective]->GetIntParameter("LightType");
		LightIntensity = _material->Effect[MVT_Perspective]->GetFloatParameter("LightIntensity");
		LightPos =  _material->Effect[MVT_Perspective]->GetVector3Parameter("LightPos");
	    LightDir = _material->Effect[MVT_Perspective]->GetVector3Parameter("LightDir");
	    LightColor = _material->Effect[MVT_Perspective]->GetVector3Parameter("LightColor");

		//compile shader
		ShaderObjectPtr vertexShader = _material->Effect[MVT_Perspective]->GetVertexShader();
		 
	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList,false);
 
		std::vector<RenderBufferPtr> vertexBufferArray;
		resourceManager->CreateVertexBufferArray(_geometryObject,BAH_GPU_Read,vertexShader,vertexBufferArray);
		for(int index=0;index<vertexBufferArray.size();index++)
		{
		    _renderLayout->BindVertexBuffer(vertexBufferArray[index]);
		}

		//RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read,_geometryObject,vertexShader);
		//_renderLayout->BindVertexBuffer(vertexBuffer);


		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read,_geometryObject,vertexShader);
		_renderLayout->BindIndexBuffer(indexBuffer);
 
	}

	void GeometryRenderer::SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat)
	{
		Name = geometry->Name;
		_geometryObject = geometry;
		_material = mat;

		BuildRenderResource();
	}

	 void GeometryRenderer::SetLightParam(LightPtr const& light)
	 {
		 LightType->SetValue((int)(light->Type));
		 LightIntensity->SetValue((float)(light->Intensity));
		 GameObjectPtr go = light->GetBase();
		 if(light->Type == LT_Parallel)
		 {
			 LightDir->SetValue(go->GetRotation()*Light::DirectLight);
		 }
		 else if( light->Type == LT_Point )
		 {
			 LightPos->SetValue(go->GetPosition());
		 }

		 LightColor->SetValue(light->Color);
	 }

	 void GeometryRenderer::Draw(MaterialViewType view,CameraPtr const& camera)
	{
		BOOST_ASSERT(_renderLayout != NULL);

		_renderEffect = _material->Effect[view];
		if( _renderEffect == NULL )
			return;

		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);

		Matrix4 worldMat = gameObject->GetWorldMatrix();
		Matrix4 viewMat = camera->ViewMatrix;
		Matrix4 projMat = camera->ProjectMatrix;
		Matrix4 wvpMat = projMat*viewMat*worldMat;
		
		WorldMatrix->SetValue(worldMat);
		if( worldMat.IdentityScale() )
			WorldNormal->SetValue(worldMat);
		else
		    WorldNormal->SetValue(worldMat.inverse().transpose());
		ViewMatrix->SetValue(viewMat);
		ProjMatrix->SetValue(projMat);
		WorldViewProjMatrix->SetValue(wvpMat);
		
		renderEngine->SetBlendState(_renderEffect->GetBlendState());
		renderEngine->SetRasterizeState(_renderEffect->GetRasterizeState());
	 
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(),0,0);
	}
}