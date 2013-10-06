#include "EngineInclude.h"

namespace Disorder
{
 
	Renderer::Renderer()
	{
		Type = CT_Renderer;
	}
 
	void Renderer::AddLight(LightPtr & light)
	{
		_vLightArray.push_back(light);
	}

	void Renderer::ClearLight()
	{
		_vLightArray.clear();
	}

	void GeometryRenderer::BindRenderResource()
	{
		BOOST_ASSERT(_geometryObject != NULL && _material != NULL);

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
 
		//compile shader
		ShaderObjectPtr vertexShader = _material->Effect[RPT_ForwardMultiPassLight][FRP_BaseLight]->GetVertexShader();
		 
	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList,false);
 
		std::vector<RenderBufferPtr> vertexBufferArray;
		resourceManager->CreateVertexBufferArray(_geometryObject,BAH_GPU_Read,vertexShader,vertexBufferArray);
		for(unsigned int index = 0;index<vertexBufferArray.size();index++)
		{
		    _renderLayout->BindVertexBuffer(vertexBufferArray[index]);
		}
	 
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read,_geometryObject,vertexShader);
		_renderLayout->BindIndexBuffer(indexBuffer);
 
	}

	void GeometryRenderer::SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat)
	{
		Name = geometry->Name;
		_geometryObject = geometry;
		_material = mat;

		BindRenderResource();
	}

	 void GeometryRenderer::PreDraw(CameraPtr const& camera)
	 {
		 if( _material != NULL )
		 {
			GameObjectPtr gameObject = _baseObject.lock();
			_material->UpdateMaterialParameters(gameObject,camera);
		 }
	 }
	
	 void GeometryRenderer::PostDraw(CameraPtr const& camera)
	 {

	 }
 
	void GeometryRenderer::SetBaseLightPass()
	{
		if( _vLightArray.size() == 0 )
			return;

		unsigned int lightNumber = 0;
		Vector4 intensityVec(0.0f);
		std::vector<Vector3> dirVec;
		std::vector<Vector3> colorVec;
		for(unsigned int i=0;i<_vLightArray.size();i++)
		{
			if( _vLightArray[i]->LType == LT_Parallel )
			{
			   intensityVec[lightNumber] = _vLightArray[i]->Intensity;
			   dirVec.push_back(_vLightArray[i]->GetDirection());
			   colorVec.push_back(_vLightArray[i]->Color);
			   lightNumber++;

			   if( lightNumber >= _material->LightColorArray->MaxElementNumber )
				   break;
			}
		}

		if( lightNumber == 0 )
			return;
 
		_material->LightNumber->SetValue(lightNumber);
		_material->LightIntensityPack->SetValue(intensityVec);
		_material->LightDirArray->SetValueArray(dirVec);
		_material->LightColorArray->SetValueArray(colorVec);
	}

	void GeometryRenderer::SetDynamicLightPass(LightPtr const& light)
	{
		BOOST_ASSERT(light->Type != LT_Parallel);

		_material->LightType->SetValue((int)(light->Type));
		_material->LightIntensity->SetValue((float)(light->Intensity));
		GameObjectPtr go = light->GetBase();
		if( light->Type == LT_Point )
		{
			_material->LightPos->SetValue(go->GetWorldPosition());
		}

		_material->LightColor->SetValue(light->Color);
	}

	void GeometryRenderer::Draw(RenderPathType pathType,int pass,CameraPtr const& camera)
	{
		BOOST_ASSERT(_renderLayout != NULL);

		_renderEffect = _material->Effect[pathType][pass];
		if( _renderEffect == NULL )
			return;

		if( pathType == RPT_ForwardMultiPassLight )
		{
			if( pass == FRP_BaseLight )
			{
				SetBaseLightPass();
				_Draw();
			}
			else if( pass == FRP_DynamicLight )
			{
				for(unsigned int i=0;i< _vLightArray.size();i++)
				{
					if( _vLightArray[i]->LType != LT_Parallel )
					{
						SetDynamicLightPass(_vLightArray[i]);
						_Draw();
					}
				}
			}
		}
	}

	void GeometryRenderer::_Draw()
	{
		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);
 
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(),0,0);
	}

	
}