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
		Sampler->SetData(texture->Tex2DResource->Sampler);
 
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
		_renderEffect = RenderEffect::Create(); 
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"2DText",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"2DText",SM_4_0,"PS");
	    
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
 
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

	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList,true);
	 
		_savedVertexBufferSize = 2048;
		_savedIndexBufferSize = (UINT)(_savedVertexBufferSize * 1.5f);
        
		_vertexs = new BatchTileVertex[_savedVertexBufferSize];
		_vertexNum = 0;
		_indexs = new unsigned short[_savedIndexBufferSize];
		_indexNum = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read | BAH_CPU_Write,sizeof(BatchTileVertex),sizeof(BatchTileVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		//Index buffer
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read | BAH_CPU_Write,sizeof(WORD),sizeof(WORD)*_savedIndexBufferSize,NULL);
		_renderLayout->BindIndexBuffer(indexBuffer);
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
		_renderLayout = GEngine->RenderResourceMgr->CreateRenderLayout(_renderEffect->GetVertexShader(),TT_TriangleStrip,true);
		RenderBufferPtr vertexBuffer = GEngine->RenderResourceMgr->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read,sizeof(TileTexVertex),sizeof(TileTexVertex)*4,(void*)positions);
		_renderLayout->BindVertexBuffer(vertexBuffer);
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
		_renderEffect =  RenderEffect::Create();
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader,"BatchLines",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader,"BatchLines",SM_4_0,"PS");
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);

		RasterizeDesc rDesc;
		rDesc.AntialiasedLineEnable = true;

		RasterizeStatePtr rState = resourceManager->CreateRasterizeState(&rDesc);
		_renderEffect->BindRasterizeState(rState);

	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_LineList,true);
	 
		_savedVertexBufferSize = 1024;
		_vertexs = new BatchLineVertex[_savedVertexBufferSize];
		_vertexNum = 0;

		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read | BAH_CPU_Write,sizeof(BatchLineVertex),sizeof(BatchLineVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

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

	void BatchLines::AddLine(Eigen::Vector3f const& beginPos,Eigen::Vector4f const& beginColor,Eigen::Vector3f const& endPos,Eigen::Vector4f const& endColor)
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
 
		
		_vertexs[_vertexNum].position.noalias() = beginPos;
		_vertexs[_vertexNum].color.noalias() = beginColor;
 
		_vertexs[_vertexNum+1].position.noalias() = endPos;
		_vertexs[_vertexNum+1].color.noalias() = endColor;
		
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

	void GeometryRenderer::BuildRenderLayout(ShaderObjectPtr const& shaderObject,bool releaseOld)
	{
		if( _geometryObject == NULL )
			return;

		if( _renderLayout != NULL && releaseOld == false )
			return;

	    _renderLayout = GEngine->RenderResourceMgr->CreateRenderLayout(shaderObject,TT_TriangleList,false);
 
		std::vector<RenderBufferPtr> bufferArray;
		GEngine->RenderResourceMgr->CreateRenderBufferArray(_geometryObject,BAH_GPU_Read,shaderObject,bufferArray);
		for(unsigned int index = 0;index<bufferArray.size();index++)
		{
			if( bufferArray[index]->GetBufferType() == RBT_Vertex )
		        _renderLayout->BindVertexBuffer(bufferArray[index]);
			else if(  bufferArray[index]->GetBufferType() == RBT_Index )
				_renderLayout->BindIndexBuffer(bufferArray[index]);
		}
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
		 Eigen::Vector3f boxVec1 = _geometryObject->BoundingBox.Origin - _geometryObject->BoundingBox.BoxExtent;
		 Eigen::Vector3f boxVec2 = Eigen::Vector3f( _geometryObject->BoundingBox.Origin.x() + _geometryObject->BoundingBox.BoxExtent.x(),
			                        _geometryObject->BoundingBox.Origin.y() - _geometryObject->BoundingBox.BoxExtent.y(),
									_geometryObject->BoundingBox.Origin.z() - _geometryObject->BoundingBox.BoxExtent.z());
		 Eigen::Vector3f boxVec3 = Eigen::Vector3f( _geometryObject->BoundingBox.Origin.x() + _geometryObject->BoundingBox.BoxExtent.x(),
			                        _geometryObject->BoundingBox.Origin.y() + _geometryObject->BoundingBox.BoxExtent.y(),
									_geometryObject->BoundingBox.Origin.z() - _geometryObject->BoundingBox.BoxExtent.z());
		 Eigen::Vector3f boxVec4 = Eigen::Vector3f( _geometryObject->BoundingBox.Origin.x() - _geometryObject->BoundingBox.BoxExtent.x(),
			                        _geometryObject->BoundingBox.Origin.y() + _geometryObject->BoundingBox.BoxExtent.y(),
									_geometryObject->BoundingBox.Origin.z() - _geometryObject->BoundingBox.BoxExtent.z());
		 Eigen::Vector3f boxVec5 = Eigen::Vector3f( _geometryObject->BoundingBox.Origin.x() - _geometryObject->BoundingBox.BoxExtent.x(),
			                        _geometryObject->BoundingBox.Origin.y() + _geometryObject->BoundingBox.BoxExtent.y(),
									_geometryObject->BoundingBox.Origin.z() + _geometryObject->BoundingBox.BoxExtent.z());
		 Eigen::Vector3f boxVec6 = Eigen::Vector3f( _geometryObject->BoundingBox.Origin.x() - _geometryObject->BoundingBox.BoxExtent.x(),
			                        _geometryObject->BoundingBox.Origin.y() - _geometryObject->BoundingBox.BoxExtent.y(),
									_geometryObject->BoundingBox.Origin.z() + _geometryObject->BoundingBox.BoxExtent.z());
		 Eigen::Vector3f boxVec7 = Eigen::Vector3f( _geometryObject->BoundingBox.Origin.x() + _geometryObject->BoundingBox.BoxExtent.x(),
			                        _geometryObject->BoundingBox.Origin.y() - _geometryObject->BoundingBox.BoxExtent.y(),
									_geometryObject->BoundingBox.Origin.z() + _geometryObject->BoundingBox.BoxExtent.z());
		 Eigen::Vector3f boxVec8 = _geometryObject->BoundingBox.Origin + _geometryObject->BoundingBox.BoxExtent;

		 GameObjectPtr go = GetBase();

		 boxVec1 = go->GetWorldMatrix() * boxVec1;
		 boxVec2 = go->GetWorldMatrix() * boxVec2;
		 boxVec3 = go->GetWorldMatrix() * boxVec3;
		 boxVec4 = go->GetWorldMatrix() * boxVec4;
		 boxVec5 = go->GetWorldMatrix() * boxVec5;
		 boxVec6 = go->GetWorldMatrix() * boxVec6;
		 boxVec7 = go->GetWorldMatrix() * boxVec7;
		 boxVec8 = go->GetWorldMatrix() * boxVec8;

		 Eigen::Vector4f color(1.0f,0,0,1.0f);
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
		 Eigen::Vector3f original = go->GetLocalPosition();
		 float AxisLength = 1.0f;
		 Eigen::Vector3f xAxis = original + Eigen::Vector3f(AxisLength,0,0);
		 Eigen::Vector3f yAxis = original + Eigen::Vector3f(0,AxisLength,0);
		 Eigen::Vector3f zAxis = original + Eigen::Vector3f(0,0,AxisLength);

		 original = go->GetWorldMatrix() * original;
		 xAxis = go->GetWorldMatrix() * xAxis;
		 yAxis = go->GetWorldMatrix() * yAxis;
		 zAxis = go->GetWorldMatrix() * zAxis;

		 GEngine->GameCanvas->DrawLine(original,Eigen::Vector4f(1.0f,0,0,1.0f),xAxis,Eigen::Vector4f(1.0f,0,0,1.0f));
		 GEngine->GameCanvas->DrawLine(original,Eigen::Vector4f(0,1.0f,0,1.0f),yAxis,Eigen::Vector4f(0,1.0f,0,1.0f));
		 GEngine->GameCanvas->DrawLine(original,Eigen::Vector4f(0,0,1.0f,1.0f),zAxis,Eigen::Vector4f(0,0,1.0f,1.0f));

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