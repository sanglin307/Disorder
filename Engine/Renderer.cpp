#include "EngineInclude.h"

namespace Disorder
{
 
	Renderer::Renderer(std::string const& name)
		:Component(name,CT_Renderer)
	{
	}
 
	void Renderer::AddLight(LightPtr & light)
	{
		_vLightArray.push_back(light);
	}

	void Renderer::ClearLight()
	{
		_vLightArray.clear();
	}

//////////////////////////////////////////////////////////////////////////

	void BatchScreenString::SetTexture(RenderSurfacePtr const& texture)
	{
		_texture = texture;
 
	    ShaderPropertyManagerPtr globalProperty = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		ShaderPropertyPtr texProperty = globalProperty->CreateProperty(ShaderPropertyManager::sFontTexture,eSP_ShaderResource);
	    texProperty->SetData(_texture);
	 
		ShaderPropertyPtr Sampler = globalProperty->CreateProperty(ShaderPropertyManager::sFontSampler,eSP_SampleState);
		Sampler->SetData(texture->Tex2DResource->Sampler);
 
	}

	void BatchScreenString::Draw(int pass,CameraPtr const& camera)
	{
		BOOST_ASSERT(_vertexs.size()%4==0);

		if( _vertexs.size() == 0 )
			return;
 
		RenderEnginePtr renderEngine = GEngine->RenderEngine;


		const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(vertexRenderBuffer,BA_Write_Only);
		memcpy(vertexBuffer,_vertexs.data(),_vertexs.size()*sizeof(BatchTileVertex));
		renderEngine->UnMap(vertexRenderBuffer);

		const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
		void* indexBuffer = renderEngine->Map(indexRenderBuffer,BA_Write_Only);
		memcpy(indexBuffer,_indexs.data(),_indexs.size()*sizeof(WORD));
		renderEngine->UnMap(indexRenderBuffer);

		renderEngine->SetRenderLayout(_renderLayout);
	
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_indexs.size(),0,0);

		_indexs.clear();
		_vertexs.clear();
	}

	unsigned int BatchScreenString::GetCurrentDrawTriNumber()
	{
		return _indexs.size() /3;
	}

	void BatchScreenString::AddVertex(Vector3 const& position,Vector4 const& color,Vector2 const& texcoord)
	{
		if( _vertexs.size() >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(BatchTileVertex) * _savedVertexBufferSize);
		}

		if( _indexs.size() >= _savedIndexBufferSize )
		{
			_savedIndexBufferSize *= 2;
			const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
			indexRenderBuffer->Resize(sizeof(WORD) * _savedIndexBufferSize);
		}

	

		BatchTileVertex vertex;
		vertex.position = position;
		vertex.color = color;
		vertex.texcoord = texcoord;

		_vertexs.push_back(vertex);
		
		if(_vertexs.size() % 4 == 0 )
		{
			WORD index = _vertexs.size() - 4;
			_indexs.push_back(index);
			_indexs.push_back(index+1);
			_indexs.push_back(index+2);
			_indexs.push_back(index+2);
			_indexs.push_back(index+1);
			_indexs.push_back(index+3);
		}
 
	}

	BatchScreenString::BatchScreenString(std::string const& name)
		:Renderer(name)
	{
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
		_renderEffect = RenderEffect::Create(); 
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"2DFont",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"2DFont",SM_4_0,"PS");
	    
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
 
		BlendDesc blendDesc;
		blendDesc.BlendEnable = true;
		blendDesc.SrcBlend = BLEND_SRC_ALPHA;
		blendDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = BLEND_OP_ADD;
		 
		BlendStatePtr blendState = resourceManager->CreateBlendState(&blendDesc,1);
		_renderEffect->BindBlendState(blendState);

	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList,true);
	 
		_savedVertexBufferSize = 2048;
		_savedIndexBufferSize = (UINT)(_savedVertexBufferSize * 1.5f);
 
		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read | BAH_CPU_Write,sizeof(BatchTileVertex),sizeof(BatchTileVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		//Index buffer
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read | BAH_CPU_Write,sizeof(WORD),sizeof(WORD)*_savedIndexBufferSize,NULL);
		_renderLayout->BindIndexBuffer(indexBuffer);
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	BatchScreenTilesPtr BatchScreenTiles::Create(std::string const& name)
	{
		BatchScreenTiles *pTiles = new BatchScreenTiles(name);
		return BatchScreenTilesPtr(pTiles);
	}

	void BatchScreenTiles::SetTexture(RenderSurfacePtr const& texture)
	{
		_texture = texture;
 
	    ShaderPropertyManagerPtr globalProperty = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		ShaderPropertyPtr texProperty = globalProperty->CreateProperty(ShaderPropertyManager::sScreenTileTexture,eSP_ShaderResource);
	    texProperty->SetData(_texture);
	 
		ShaderPropertyPtr Sampler = globalProperty->CreateProperty(ShaderPropertyManager::sScreenTileSampler,eSP_SampleState);
		Sampler->SetData(texture->Tex2DResource->Sampler);
 
	}

	void BatchScreenTiles::Draw(int pass,CameraPtr const& camera)
	{
		BOOST_ASSERT(_vertexs.size()%4==0);

		if( _vertexs.size() == 0 )
			return;
 
		RenderEnginePtr renderEngine = GEngine->RenderEngine;


		const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(vertexRenderBuffer,BA_Write_Only);
		memcpy(vertexBuffer,_vertexs.data(),_vertexs.size()*sizeof(BatchTileVertex));
		renderEngine->UnMap(vertexRenderBuffer);

		const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
		void* indexBuffer = renderEngine->Map(indexRenderBuffer,BA_Write_Only);
		memcpy(indexBuffer,_indexs.data(),_indexs.size()*sizeof(WORD));
		renderEngine->UnMap(indexRenderBuffer);

		renderEngine->SetRenderLayout(_renderLayout);
	
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_indexs.size(),0,0);

		_indexs.clear();
		_vertexs.clear();
	}

	unsigned int BatchScreenTiles::GetCurrentDrawTriNumber()
	{
		return _indexs.size() /3;
	}

	void BatchScreenTiles::AddVertex(Vector3 const& position,Vector4 const& color,Vector2 const& texcoord)
	{
		if( _vertexs.size() >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(BatchTileVertex) * _savedVertexBufferSize);
		}

		if( _indexs.size() >= _savedIndexBufferSize )
		{
			_savedIndexBufferSize *= 2;
			const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
			indexRenderBuffer->Resize(sizeof(WORD) * _savedIndexBufferSize);
		}

	

		BatchTileVertex vertex;
		vertex.position = position;
		vertex.color = color;
		vertex.texcoord = texcoord;

		_vertexs.push_back(vertex);
		
		if(_vertexs.size() % 4 == 0 )
		{
			WORD index = _vertexs.size() - 4;
			_indexs.push_back(index);
			_indexs.push_back(index+1);
			_indexs.push_back(index+2);
			_indexs.push_back(index+2);
			_indexs.push_back(index+1);
			_indexs.push_back(index+3);
		}
 
	}

	BatchScreenTiles::BatchScreenTiles(std::string const& name)
		:Renderer(name)
	{
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
		_renderEffect = RenderEffect::Create(); 
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"ScreenTiles",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"ScreenTiles",SM_4_0,"PS");
	    
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
 
		BlendDesc blendDesc;
		blendDesc.BlendEnable = true;
		blendDesc.SrcBlend = BLEND_SRC_ALPHA;
		blendDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = BLEND_OP_ADD;
		 
		BlendStatePtr blendState = resourceManager->CreateBlendState(&blendDesc,1);
		_renderEffect->BindBlendState(blendState);

	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList,true);
	 
		_savedVertexBufferSize = 16;
		_savedIndexBufferSize = (UINT)(_savedVertexBufferSize * 1.5f);
 
		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read | BAH_CPU_Write,sizeof(BatchTileVertex),sizeof(BatchTileVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		//Index buffer
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read | BAH_CPU_Write,sizeof(WORD),sizeof(WORD)*_savedIndexBufferSize,NULL);
		_renderLayout->BindIndexBuffer(indexBuffer);
	}

	/////////////////////////////////////////////////////////////////////////
	BatchLines::BatchLines(std::string const& name)
		:Renderer(name)
	{
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
		_renderEffect =  RenderEffect::Create();
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"BatchLines",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"BatchLines",SM_4_0,"PS");
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);

	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_LineList,true);
	 
		_savedVertexBufferSize = 1024;
 
		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read | BAH_CPU_Write,sizeof(BatchLineVertex),sizeof(BatchLineVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

	}
	
	void BatchLines::AddLine(Vector3 const& beginPos,Vector4 const& beginColor,Vector3 const& endPos,Vector4 const& endColor)
	{
		if( _vertexs.size() >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(BatchLineVertex) * _savedVertexBufferSize);
		}
 
		BatchLineVertex beginVertex;
		beginVertex.position = beginPos;
		beginVertex.color = beginColor;
		_vertexs.push_back(beginVertex);

		BatchLineVertex endVertex;
		endVertex.position = endPos;
		endVertex.color = endColor;
		_vertexs.push_back(endVertex);
 
	}

	void BatchLines::Draw(int pass,CameraPtr const& camera)
	{
		BOOST_ASSERT(_vertexs.size()%2==0);

		if( _vertexs.size() == 0 )
			return;
 
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(vertexRenderBuffer,BA_Write_Only);
		memcpy(vertexBuffer,_vertexs.data(),_vertexs.size()*sizeof(BatchLineVertex));
		renderEngine->UnMap(vertexRenderBuffer);

		renderEngine->SetRenderLayout(_renderLayout);
	
		renderEngine->SetEffect(_renderEffect);
		renderEngine->Draw(_vertexs.size(),0);
		_vertexs.clear();
	}


	//////////////////////////////////////////////////////////////////////////
	

	GeometryRenderer::GeometryRenderer(std::string const& name)
		:Renderer(name)
	{
		_LightPropertyManager = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerLight);

		_LightNumberProperty = _LightPropertyManager->CreateProperty(ShaderPropertyManager::sLightNumber,eSP_Int);
		_LightIntensityPackProperty = _LightPropertyManager->CreateProperty(ShaderPropertyManager::sLightIntensityPack,eSP_Vector4);
		_LightDirArrayProperty = _LightPropertyManager->CreateProperty(ShaderPropertyManager::sLightDirArray,eSP_Vector3);
		_LightColorArrayProperty = _LightPropertyManager->CreateProperty(ShaderPropertyManager::sLightColorArray,eSP_Vector3);
	}

	bool GeometryRenderer::Overlaps(const Frustrum& frustrum)
	{
		return frustrum.Overlaps( _geometryObject->BoundingBox.GetBox());
	}

	void GeometryRenderer::BindRenderResource()
	{
		BOOST_ASSERT(_geometryObject != NULL && _material != NULL);

		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
 
		//compile shader
		ShaderObjectPtr vertexShader = _material->RenderPass[FRP_BaseLight]->GetVertexShader();
		 
	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList,false);
 
		std::vector<RenderBufferPtr> bufferArray;
		resourceManager->CreateRenderBufferArray(_geometryObject,BAH_GPU_Read,vertexShader,bufferArray);
		for(unsigned int index = 0;index<bufferArray.size();index++)
		{
			if( bufferArray[index]->GetBufferType() == RBT_Vertex )
		        _renderLayout->BindVertexBuffer(bufferArray[index]);
			else if(  bufferArray[index]->GetBufferType() == RBT_Index )
				_renderLayout->BindIndexBuffer(bufferArray[index]);
		}
	 
	 
	}

	GeometryRendererPtr GeometryRenderer::Create(std::string const& name)
	{
		GeometryRenderer *pRender = new GeometryRenderer(name);
		return GeometryRendererPtr(pRender);
	}

	void GeometryRenderer::SetGeometry(GeometryPtr const& geometry,SurfaceMaterialPtr const& mat)
	{
		_geometryObject = geometry;
		_material = mat;

		BindRenderResource();
	}

	 void GeometryRenderer::PreDraw(CameraPtr const& camera)
	 {
		 GetBase()->UpdateShaderProperty();
		 if( _material != NULL )
		 {
			_material->UpdateShaderProperty();
		 }


	 }

	 void GeometryRenderer::DrawBoundingBox(CameraPtr const& camera)
	 {
		 Vector3 boxVec1 = _geometryObject->BoundingBox.Origin - _geometryObject->BoundingBox.BoxExtent;
		 Vector3 boxVec2 = Vector3( _geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			                        _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
									_geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z);
		 Vector3 boxVec3 = Vector3( _geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			                        _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
									_geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z);
		 Vector3 boxVec4 = Vector3( _geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			                        _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
									_geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z);
		 Vector3 boxVec5 = Vector3( _geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			                        _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
									_geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z);
		 Vector3 boxVec6 = Vector3( _geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			                        _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
									_geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z);
		 Vector3 boxVec7 = Vector3( _geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			                        _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
									_geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z);
		 Vector3 boxVec8 = _geometryObject->BoundingBox.Origin + _geometryObject->BoundingBox.BoxExtent;

		 GameObjectPtr go = GetBase();

		 boxVec1 = go->GetWorldMatrix() * boxVec1;
		 boxVec2 = go->GetWorldMatrix() * boxVec2;
		 boxVec3 = go->GetWorldMatrix() * boxVec3;
		 boxVec4 = go->GetWorldMatrix() * boxVec4;
		 boxVec5 = go->GetWorldMatrix() * boxVec5;
		 boxVec6 = go->GetWorldMatrix() * boxVec6;
		 boxVec7 = go->GetWorldMatrix() * boxVec7;
		 boxVec8 = go->GetWorldMatrix() * boxVec8;

		 Vector4 color(1.0f,0,0,1.0f);
		 GEngine->GameCanvas->DrawLine(boxVec1,color,boxVec2,color);
		 GEngine->GameCanvas->DrawLine(boxVec2,color,boxVec3,color);
		 GEngine->GameCanvas->DrawLine(boxVec3,color,boxVec4,color);
		 GEngine->GameCanvas->DrawLine(boxVec4,color,boxVec1,color);

		 GEngine->GameCanvas->DrawLine(boxVec4,color,boxVec5,color);
	     GEngine->GameCanvas->DrawLine(boxVec1,color,boxVec6,color);
		 GEngine->GameCanvas->DrawLine(boxVec2,color,boxVec7,color);
	     GEngine->GameCanvas->DrawLine(boxVec3,color,boxVec8,color);

		 GEngine->GameCanvas->DrawLine(boxVec5,color,boxVec6,color);
		 GEngine->GameCanvas->DrawLine(boxVec6,color,boxVec7,color);
	     GEngine->GameCanvas->DrawLine(boxVec7,color,boxVec8,color);		
	     GEngine->GameCanvas->DrawLine(boxVec8,color,boxVec5,color);
	 }
	
	  void GeometryRenderer::DrawAxis(CameraPtr const& camera)
	  {
		 // draw local axis for debug
		 GameObjectPtr go = GetBase();
		 Vector3 original = go->GetLocalPosition();
		 float AxisLength = 1.0f;
		 Vector3 xAxis = original + Vector3(AxisLength,0,0);
		 Vector3 yAxis = original + Vector3(0,AxisLength,0);
		 Vector3 zAxis = original + Vector3(0,0,AxisLength);

		 original = go->GetWorldMatrix() * original;
		 xAxis = go->GetWorldMatrix() * xAxis;
		 yAxis = go->GetWorldMatrix() * yAxis;
		 zAxis = go->GetWorldMatrix() * zAxis;

		 GEngine->GameCanvas->DrawLine(original,Vector4(1.0f,0,0,1.0f),xAxis,Vector4(1.0f,0,0,1.0f));
		 GEngine->GameCanvas->DrawLine(original,Vector4(0,1.0f,0,1.0f),yAxis,Vector4(0,1.0f,0,1.0f));
		 GEngine->GameCanvas->DrawLine(original,Vector4(0,0,1.0f,1.0f),zAxis,Vector4(0,0,1.0f,1.0f));

		 Matrix4 ProjectViewMatrix = camera->ProjectMatrix * camera->ViewMatrix;
		 xAxis = ProjectViewMatrix * xAxis;
		 yAxis = ProjectViewMatrix * yAxis;
		 zAxis = ProjectViewMatrix * zAxis;

		 GEngine->GameCanvas->DrawStringDeviceSpace(Vector2(xAxis.x,xAxis.y),35,Vector4(1.0f,0,0,1.0f),std::string("X"));
		 GEngine->GameCanvas->DrawStringDeviceSpace(Vector2(yAxis.x,yAxis.y),35,Vector4(0,1.0f,0,1.0f),std::string("Y"));
		 GEngine->GameCanvas->DrawStringDeviceSpace(Vector2(zAxis.x,zAxis.y),35,Vector4(0,0,1.0f,1.0f),std::string("Z"));

	  }

	 void GeometryRenderer::PostDraw(CameraPtr const& camera)
	 {
		  DrawAxis(camera);
		  DrawBoundingBox(camera);
	 }
 
	void GeometryRenderer::SetBaseLightPass()
	{
		if( _vLightArray.size() == 0 )
			return;

		int lightNumber = 0;
		Vector4 intensityVec(0.0f);
		Vector3 dirVec;
		Vector3 colorVec;
		for(unsigned int i=0;i<_vLightArray.size();i++)
		{
			if( _vLightArray[i]->LightType == LT_Directional )
			{
			   intensityVec[lightNumber] = _vLightArray[i]->Intensity;
			   dirVec = _vLightArray[i]->GetDirection();
			   colorVec = _vLightArray[i]->Color;
			   lightNumber++;

			   break;
			}
		}

		if( lightNumber == 0 )
			return;
 
		_LightNumberProperty->SetData(lightNumber);
		_LightIntensityPackProperty->SetData(intensityVec);
		_LightDirArrayProperty->SetData(dirVec);
		_LightColorArrayProperty->SetData(colorVec);
		_LightPropertyManager->UpdateShaderProperty();
	}

	void GeometryRenderer::SetDynamicLightPass(LightPtr const& light)
	{
		BOOST_ASSERT(light->LightType != LT_Directional);

	/*	_material->LightType->SetData((int)(light->LightType));
		_material->LightIntensity->SetData((float)(light->Intensity));
		GameObjectPtr go = light->GetBase();
		if( light->LightType == LT_Point )
		{
			_material->LightPos->SetData(go->GetWorldPosition());
		}

		_material->LightColor->SetData(light->Color);*/
	}

	void GeometryRenderer::Draw(int pass,CameraPtr const& camera)
	{
		BOOST_ASSERT(_renderLayout != NULL);
 
		_renderEffect = _material->RenderPass[pass];
		if( _renderEffect == NULL )
			return;
 
		if( pass == FRP_BaseLight )
		{
			SetBaseLightPass();
			_Draw();
		}
		else if( pass == FRP_DynamicLight )
		{
			for(unsigned int i=0;i< _vLightArray.size();i++)
			{
				if( _vLightArray[i]->LightType != LT_Directional )
				{
					SetDynamicLightPass(_vLightArray[i]);
					_Draw();
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