#include "EngineInclude.h"

namespace Disorder
{
 
	Renderer::Renderer(std::string const& name)
		:Component(name,CT_Renderer)
	{
	}
 
	void Renderer::AddLight(LightPtr & light)
	{
		if( light->LightType == LT_Directional )
		{
			DirectionLightPtr dLight = boost::dynamic_pointer_cast<DirectionLight>(light);
			_vDirectionLights.push_back(dLight);
		}
		else
			_vNonDirectionLights.push_back(light);
	}

	void Renderer::ClearLight()
	{
		_vNonDirectionLights.clear();
		_vDirectionLights.clear();
	}

//////////////////////////////////////////////////////////////////////////

	void BatchScreenString::SetTexture(RenderSurfacePtr const& texture)
	{
		_texture = texture;
 
	    ShaderPropertyManagerPtr globalProperty = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		ShaderPropertyPtr texProperty = globalProperty->CreateProperty(ShaderPropertyManager::sTextTexture,eSP_ShaderResource,1);
	    texProperty->SetData(_texture);
	 
		ShaderPropertyPtr Sampler = globalProperty->CreateProperty(ShaderPropertyManager::sTextSampler,eSP_SampleState,1);
		RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(texture->GetResource(SL_ShaderResource));
		Sampler->SetData(tex->Sampler);
 
	}

	void BatchScreenString::Render(CameraPtr const& camera)
	{
		BOOST_ASSERT(_vertexNum%4==0);

		if( _vertexNum == 0 )
			return;
 
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
 
		const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(vertexRenderBuffer,BA_Write_Only);
		unsigned int length = _vertexNum*sizeof(BatchTileVertex);
		memcpy(vertexBuffer,_vertexs,length);
		renderEngine->UnMap(vertexRenderBuffer);

		const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
		void* indexBuffer = renderEngine->Map(indexRenderBuffer,BA_Write_Only);
		memcpy(indexBuffer,_indexs,_indexNum*sizeof(unsigned short));
		renderEngine->UnMap(indexRenderBuffer);

		renderEngine->SetRenderLayout(_renderLayout);
	
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_indexNum,0,0);

		_vertexNum = 0;
		_indexNum = 0;
	}

	unsigned int BatchScreenString::GetCurrentDrawTriNumber()
	{
		return _indexNum /3;
	}

	BatchTileVertex* BatchScreenString::PrepareAddVertex()
	{
		if( _vertexNum + 4 >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			BatchTileVertex *pBuffer = new BatchTileVertex[_savedVertexBufferSize];
			memcpy(pBuffer,_vertexs,_vertexNum*sizeof(BatchTileVertex));
			delete []_vertexs;
			_vertexs = pBuffer;

			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(BatchTileVertex) * _savedVertexBufferSize);
		}

		if( _indexNum + 6  >= _savedIndexBufferSize )
		{
			_savedIndexBufferSize *= 2;
			unsigned short *pBuffer = new unsigned short[_savedIndexBufferSize];
			memcpy(pBuffer,_indexs,_indexNum*sizeof(unsigned short));
			delete[] _indexs;
			_indexs = pBuffer;

			const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
			indexRenderBuffer->Resize(sizeof(WORD) * _savedIndexBufferSize);
		}

		return &(_vertexs[_vertexNum]);
	}

	void BatchScreenString::EndAddVertex()
	{
		_indexs[_indexNum] = _vertexNum;
		_indexs[_indexNum+1] = _vertexNum+1;
		_indexs[_indexNum+2] = _vertexNum+2;
		_indexs[_indexNum+3] = _vertexNum+2;
		_indexs[_indexNum+4] = _vertexNum+1;
		_indexs[_indexNum+5] = _vertexNum+3;
		_indexNum += 6;
		_vertexNum += 4;
	}

	BatchScreenString::BatchScreenString(std::string const& name)
		:Renderer(name)
	{
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResourceMgr;
		_renderEffect = resourceManager->CreateRenderEffect(); 
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"2DText",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"2DText",SM_4_0,"PS");
	    
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
		_renderEffect->LinkShaders();
 
		BlendDesc blendDesc;
		blendDesc.BlendEnable = true;
		blendDesc.SrcBlend = BLEND_SRC_ALPHA;
		blendDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = BLEND_OP_ADD;
		 
		BlendStatePtr blendState = resourceManager->CreateBlendState(&blendDesc,1);
		_renderEffect->BindBlendState(blendState);

		RasterizeDesc rDesc;
		rDesc.FrontCounterClockwise = false;
		RasterizeStatePtr rState = resourceManager->CreateRasterizeState(&rDesc);
		_renderEffect->BindRasterizeState(rState);

		_renderLayout = resourceManager->CreateRenderLayout(_renderEffect,TT_TriangleList,true);
	 
		_savedVertexBufferSize = 2048;
		_savedIndexBufferSize = (UINT)(_savedVertexBufferSize * 1.5f);
        
		_vertexs = new BatchTileVertex[_savedVertexBufferSize];
		_vertexNum = 0;
		_indexs = new unsigned short[_savedIndexBufferSize];
		_indexNum = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateBuffer(RBT_Vertex,BU_DynamicDraw,sizeof(BatchTileVertex),sizeof(BatchTileVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		//Index buffer
		RenderBufferPtr indexBuffer = resourceManager->CreateBuffer(RBT_Index,BU_DynamicDraw,sizeof(WORD),sizeof(WORD)*_savedIndexBufferSize,NULL);
		_renderLayout->BindIndexBuffer(indexBuffer);

		_renderLayout->FinishBufferBinding(_renderEffect);
	}

	BatchScreenString::~BatchScreenString()
	{
		if( _vertexs )
			delete []_vertexs;
		if( _indexs )
			delete[] _indexs;

		_vertexs = 0;
		_indexs = 0;
		_vertexNum = 0;
		_indexNum = 0;
	}
	//////////////////////////////////////////////////////////////////////////
 
	////////////////////////////////////////////////////////////////////////////
	SimpleTile::SimpleTile()
		:Renderer("DefaultSimpleTile")
	{
	}

	SimpleTile::SimpleTile(std::string const& name,const TileTexVertex* positions,const RenderEffectPtr& renderEffect)
		:Renderer(name)
	{
		_renderEffect = renderEffect;
		_renderLayout = GEngine->RenderResourceMgr->CreateRenderLayout(_renderEffect,TT_TriangleStrip,true);
		RenderBufferPtr vertexBuffer = GEngine->RenderResourceMgr->CreateBuffer(RBT_Vertex,BU_StaticDraw,sizeof(TileTexVertex),sizeof(TileTexVertex)*4,(void*)positions);
		_renderLayout->BindVertexBuffer(vertexBuffer);
		_renderLayout->FinishBufferBinding(_renderEffect);
	}

	 

	void SimpleTile::Render(CameraPtr const& camera)
	{
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);
		renderEngine->SetEffect(_renderEffect);
		renderEngine->Draw(4,0);
	}

	/////////////////////////////////////////////////////////////////////////
	BatchLines::BatchLines(std::string const& name)
		:Renderer(name)
	{
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResourceMgr;
		_renderEffect =  resourceManager->CreateRenderEffect();
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"BatchLines",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"BatchLines",SM_4_0,"PS");
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
		_renderEffect->LinkShaders();

		RasterizeDesc rDesc;
		rDesc.AntialiasedLineEnable = true;
		rDesc.MultisampleEnable = true;
		RasterizeStatePtr rState = resourceManager->CreateRasterizeState(&rDesc);
		_renderEffect->BindRasterizeState(rState);


		_renderLayout = resourceManager->CreateRenderLayout(_renderEffect,TT_LineList,true);
	 
		_savedVertexBufferSize = 1024;
		_vertexs = new BatchLineVertex[_savedVertexBufferSize];
		_vertexNum = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateBuffer(RBT_Vertex,BU_DynamicDraw,sizeof(BatchLineVertex),sizeof(BatchLineVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);
		_renderLayout->FinishBufferBinding(_renderEffect);

	}
	
	BatchLines::~BatchLines()
	{
		if( _vertexs )
			delete[] _vertexs;

		_vertexs = 0;
		_vertexNum = 0;
	}

	BatchLineVertex* BatchLines::PrepareAddVertex()
	{
		if( _vertexNum + 2 >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			BatchLineVertex* pBuffer = new BatchLineVertex[_savedVertexBufferSize];
			memcpy(pBuffer,_vertexs,sizeof(BatchLineVertex)*_vertexNum);
			delete[] _vertexs;
			_vertexs = pBuffer;

			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(BatchLineVertex) * _savedVertexBufferSize);
		}
 
		return &(_vertexs[_vertexNum]);
	}

	void BatchLines::EndAddVertex()
	{
		_vertexNum += 2;
	}

	void BatchLines::AddLine(glm::vec3 const& beginPos, glm::vec4 const& beginColor, glm::vec3 const& endPos, glm::vec4 const& endColor)
	{
		if( _vertexNum >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			BatchLineVertex* pBuffer = new BatchLineVertex[_savedVertexBufferSize];
			memcpy(pBuffer,_vertexs,sizeof(BatchLineVertex)*_vertexNum);
			delete[] _vertexs;
			_vertexs = pBuffer;

			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(BatchLineVertex) * _savedVertexBufferSize);
		}
 
		
		_vertexs[_vertexNum].position = beginPos;
		_vertexs[_vertexNum].color = beginColor;
 
		_vertexs[_vertexNum+1].position = endPos;
		_vertexs[_vertexNum+1].color = endColor;
		
		_vertexNum += 2;
 
	}

	void BatchLines::Render(CameraPtr const& camera)
	{
		BOOST_ASSERT(_vertexNum%2==0);

		if( _vertexNum == 0 )
			return;
 
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(vertexRenderBuffer,BA_Write_Only);
		memcpy(vertexBuffer,_vertexs,_vertexNum*sizeof(BatchLineVertex));
		renderEngine->UnMap(vertexRenderBuffer);

		renderEngine->SetRenderLayout(_renderLayout);
	
		renderEngine->SetEffect(_renderEffect);
		renderEngine->Draw(_vertexNum,0);
		_vertexNum = 0;
	}


	//////////////////////////////////////////////////////////////////////////
	

	GeometryRenderer::GeometryRenderer(std::string const& name)
		:Renderer(name)
	{
	}

	bool GeometryRenderer::Overlaps(const Frustrum& frustrum)
	{
		return frustrum.Overlaps( _geometryObject->BoundingBox.GetBox());
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
	}

	void GeometryRenderer::BuildRenderLayout(RenderEffectPtr const& effect,bool releaseOld)
	{
		if( _geometryObject == NULL )
			return;

		if( _renderLayout != NULL && releaseOld == false )
			return;

		_renderLayout = GEngine->RenderResourceMgr->CreateRenderLayout(effect,TT_TriangleList,false);
 
		std::vector<RenderBufferPtr> bufferArray;
		GEngine->RenderResourceMgr->CreateBufferArray(_geometryObject,BU_StaticDraw,effect,bufferArray);
		for(unsigned int index = 0;index<bufferArray.size();index++)
		{
			if( bufferArray[index]->GetBufferType() == RBT_Vertex )
		        _renderLayout->BindVertexBuffer(bufferArray[index]);
			else if(  bufferArray[index]->GetBufferType() == RBT_Index )
				_renderLayout->BindIndexBuffer(bufferArray[index]);
		}

		_renderLayout->FinishBufferBinding(effect);
	}

	 void GeometryRenderer::PreRender(CameraPtr const& camera)
	 {
		 GetBase()->UpdateShaderProperty();
		 if( _material != NULL )
		 {
			_material->UpdateShaderProperty();
		 }
	 }

	 void GeometryRenderer::DrawBoundingBox(CameraPtr const& camera)
	 {
		 glm::vec4 boxVec1( _geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			                _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
						    _geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec2(_geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec3(_geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec4(_geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z - _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec5(_geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec6(_geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec7(_geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y - _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z,1);
		 glm::vec4 boxVec8(_geometryObject->BoundingBox.Origin.x + _geometryObject->BoundingBox.BoxExtent.x,
			               _geometryObject->BoundingBox.Origin.y + _geometryObject->BoundingBox.BoxExtent.y,
						   _geometryObject->BoundingBox.Origin.z + _geometryObject->BoundingBox.BoxExtent.z,1);

		 GameObjectPtr go = GetBase();
		 const glm::mat4& worldMat = go->GetWorldMatrix();
		 boxVec1 = worldMat * boxVec1;
		 boxVec2 = worldMat * boxVec2;
		 boxVec3 = worldMat * boxVec3;
		 boxVec4 = worldMat * boxVec4;
		 boxVec5 = worldMat * boxVec5;
		 boxVec6 = worldMat * boxVec6;
		 boxVec7 = worldMat * boxVec7;
		 boxVec8 = worldMat * boxVec8;

		 boxVec1 = boxVec1 / boxVec1.w;
		 boxVec2 = boxVec2 / boxVec2.w;
		 boxVec3 = boxVec3 / boxVec3.w;
		 boxVec4 = boxVec4 / boxVec4.w;
		 boxVec5 = boxVec5 / boxVec5.w;
		 boxVec6 = boxVec6 / boxVec6.w;
		 boxVec7 = boxVec7 / boxVec7.w;
		 boxVec8 = boxVec8 / boxVec8.w;
	 

		 glm::vec3 b1(boxVec1.x, boxVec1.y, boxVec1.z);
		 glm::vec3 b2(boxVec2.x, boxVec2.y, boxVec2.z);
		 glm::vec3 b3(boxVec3.x, boxVec3.y, boxVec3.z);
		 glm::vec3 b4(boxVec4.x, boxVec4.y, boxVec4.z);
		 glm::vec3 b5(boxVec5.x, boxVec5.y, boxVec5.z);
		 glm::vec3 b6(boxVec6.x, boxVec6.y, boxVec6.z);
		 glm::vec3 b7(boxVec7.x, boxVec7.y, boxVec7.z);
		 glm::vec3 b8(boxVec8.x, boxVec8.y, boxVec8.z);

		 glm::vec4 color(1.0f,0,0,1.0f);
		 GEngine->GameCanvas->DrawLine(b1,color,b2,color);
		 GEngine->GameCanvas->DrawLine(b2,color,b3,color);
		 GEngine->GameCanvas->DrawLine(b3,color,b4,color);
		 GEngine->GameCanvas->DrawLine(b4,color,b1,color);

		 GEngine->GameCanvas->DrawLine(b4,color,b5,color);
	     GEngine->GameCanvas->DrawLine(b1,color,b6,color);
		 GEngine->GameCanvas->DrawLine(b2,color,b7,color);
	     GEngine->GameCanvas->DrawLine(b3,color,b8,color);

		 GEngine->GameCanvas->DrawLine(b5,color,b6,color);
		 GEngine->GameCanvas->DrawLine(b6,color,b7,color);
	     GEngine->GameCanvas->DrawLine(b7,color,b8,color);		
	     GEngine->GameCanvas->DrawLine(b8,color,b5,color);
	 }
	
	 void GeometryRenderer::DebugDraw()
	 {
		  CameraPtr camera = GSceneManager->GetDefaultCamera();
		  DrawAxis(camera);
		  DrawBoundingBox(camera);
	 }

	  void GeometryRenderer::DrawAxis(CameraPtr const& camera)
	  {
		 // draw local axis for debug
		 GameObjectPtr go = GetBase();
		 glm::vec3 original = go->GetLocalPosition();
		 float AxisLength = 1.0f;
		 glm::vec3 xAxis = original + glm::vec3(AxisLength, 0, 0);
		 glm::vec3 yAxis = original + glm::vec3(0, AxisLength, 0);
		 glm::vec3 zAxis = original + glm::vec3(0, 0, AxisLength);

		 glm::vec4 x(xAxis.x, xAxis.y, xAxis.z, 1);
		 glm::vec4 y(yAxis.x, yAxis.y, yAxis.z, 1);
		 glm::vec4 z(zAxis.x, zAxis.y, zAxis.z, 1);
		 glm::vec4 o(original.x, original.y, original.z, 1);

		 o = go->GetWorldMatrix() * o;
		 x = go->GetWorldMatrix() * x;
		 y = go->GetWorldMatrix() * y;
		 z = go->GetWorldMatrix() * z;

		 o = o / o.w;
		 x = x / x.w;
		 y = y / y.w;
		 z = z / z.w;

		 original.x = o.x; original.y = o.y; original.z = o.z;
		 xAxis.x = x.x; xAxis.y = x.y; xAxis.z = x.z;
		 yAxis.x = y.x; yAxis.y = y.y; yAxis.z = y.z;
		 zAxis.x = z.x; zAxis.y = z.y; zAxis.z = z.z;

		 GEngine->GameCanvas->DrawLine(original, glm::vec4(1.0f, 0, 0, 1.0f), xAxis, glm::vec4(1.0f, 0, 0, 1.0f));
		 GEngine->GameCanvas->DrawLine(original, glm::vec4(0, 1.0f, 0, 1.0f), yAxis, glm::vec4(0, 1.0f, 0, 1.0f));
		 GEngine->GameCanvas->DrawLine(original, glm::vec4(0, 0, 1.0f, 1.0f), zAxis, glm::vec4(0, 0, 1.0f, 1.0f));

	//	 Matrix4 ProjectViewMatrix = camera->ProjectMatrix.Transpose() * camera->ViewMatrix.Transpose();
	//	 xAxis = ProjectViewMatrix * xAxis;
	//	 yAxis = ProjectViewMatrix * yAxis;
	//	 zAxis = ProjectViewMatrix * zAxis;

	//	 GEngine->GameCanvas->DrawStringDeviceSpace(Eigen::Vector2f(xAxis.x,xAxis.y),35,Eigen::Vector4f(1.0f,0,0,1.0f),std::string("X"));
	//	 GEngine->GameCanvas->DrawStringDeviceSpace(Eigen::Vector2f(yAxis.x,yAxis.y),35,Eigen::Vector4f(0,1.0f,0,1.0f),std::string("Y"));
	//	 GEngine->GameCanvas->DrawStringDeviceSpace(Eigen::Vector2f(zAxis.x,zAxis.y),35,Eigen::Vector4f(0,0,1.0f,1.0f),std::string("Z"));

	  }

	 void GeometryRenderer::PostRender(CameraPtr const& camera)
	 {	 
	 }
 

	void GeometryRenderer::Render(CameraPtr const& camera)
	{
		BOOST_ASSERT(_renderLayout != NULL);
 
		if( _renderEffect == NULL )
			return;
 
		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		renderEngine->SetRenderLayout(_renderLayout);
 
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(),0,0);
		 
	}



	
}