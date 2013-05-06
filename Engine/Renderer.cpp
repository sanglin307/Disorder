#include "EngineInclude.h"

namespace Disorder
{
 
	void GeometryRenderer::BuildRenderResource()
	{
		BOOST_ASSERT(_geometryObject != NULL && _material != NULL);

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
 
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

	void GeometryRenderer::SetDirectLightParam(std::vector<LightPtr> const& lightArray)
	{
		int lightNumber = lightArray.size();
		if( _material->Effect[MVT_Lights] == 0 )
			return;

		if( lightNumber > _material->LightColorArray->MaxElementNumber )
			lightNumber = _material->LightColorArray->MaxElementNumber;

		_material->LightNumber->SetValue(lightNumber);
		std::vector<float> intensityVec;
		std::vector<Vector3> dirVec;
		std::vector<Vector3> colorVec;
		for(int i=0;i<lightArray.size();i++)
		{
			intensityVec.push_back(lightArray[i]->Intensity);
			dirVec.push_back(lightArray[i]->GetDirect());
			colorVec.push_back(lightArray[i]->Color);
		}

		_material->LightIntensityArray->SetValueArray(intensityVec);
		_material->LightDirArray->SetValueArray(dirVec);
		_material->LightColorArray->SetValueArray(colorVec);


	}

	void GeometryRenderer::SetLightParam(LightPtr const& light)
	{
		_material->LightType->SetValue((int)(light->Type));
		_material->LightIntensity->SetValue((float)(light->Intensity));
		GameObjectPtr go = light->GetBase();
		if(light->Type == LT_Parallel)
		{
			_material->LightDir->SetValue(light->GetDirect());
		}
		else if( light->Type == LT_Point )
		{
			_material->LightPos->SetValue(go->GetPosition());
		}

		_material->LightColor->SetValue(light->Color);
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

		if( view == MVT_Perspective )
		{
			Matrix4 worldMat = gameObject->GetWorldMatrix();
			Matrix4 viewMat = camera->ViewMatrix;
			Matrix4 projMat = camera->ProjectMatrix;
			Matrix4 wvpMat = projMat*viewMat*worldMat;
		
			_material->WorldMatrix->SetValue(worldMat);
			_material->WorldNormal->SetValue(worldMat.GetNormalMatrix());
			_material->ViewMatrix->SetValue(viewMat);
			_material->ProjMatrix->SetValue(projMat);
			_material->WorldViewProjMatrix->SetValue(wvpMat);
		
		}

		renderEngine->SetBlendState(_renderEffect->GetBlendState());
		renderEngine->SetRasterizeState(_renderEffect->GetRasterizeState());
	 
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(),0,0);
	}
}