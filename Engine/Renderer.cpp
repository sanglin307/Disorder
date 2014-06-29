#include "EngineInclude.h"

namespace Disorder
{
 
	Renderer::Renderer(std::string const& name)
		:Component(name,CT_Renderer)
	{
	}
 
//////////////////////////////////////////////////////////////////////////

	void BatchScreenString::SetTexture(SurfaceViewPtr const& texture)
	{
		_texture = texture;
 
	    ShaderPropertyManagerPtr globalProperty = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		ShaderPropertyPtr texProperty = globalProperty->CreateProperty(ShaderPropertyManager::sTextTexture,eSP_ShaderResource,1);
	    texProperty->SetData(_texture);
	 
		ShaderPropertyPtr Sampler = globalProperty->CreateProperty(ShaderPropertyManager::sTextSampler,eSP_SampleState,1);
		RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(texture->Resource);
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
	
	void BatchVolumeLines::BuildBuffer(int maxLineNumber)
	{
		if (_vertexs)
		{
			BatchLineVertex* pBuffer = new BatchLineVertex[_savedVertexBufferSize];
			memcpy(pBuffer, _vertexs, sizeof(BatchLineVertex)*_vertexNum);
			delete[] _vertexs;
			_vertexs = pBuffer;
		}
		else
		   _vertexs = new BatchLineVertex[_savedVertexBufferSize];

		_vertexNum = 0;

		if (_positions)
			delete[] _positions;
		if (_otherPositions)
			delete[] _otherPositions;
		if (_colors)
			delete[] _colors;

		_positions = new float[maxLineNumber * 8 * 3];
		_otherPositions = new float[maxLineNumber * 8 * 3];
		_colors = new float[maxLineNumber * 8 * 3];

		float* offsetDirUv = new float[maxLineNumber * 4 * 8];
		for (int v = 0; v< maxLineNumber * 4 * 8; v += 4 * 8)
		{
			offsetDirUv[v] = 1.0f;		    offsetDirUv[v + 1] = 1.0f;		offsetDirUv[v + 2] = 1.0f;		offsetDirUv[v + 3] = 0.0f;
			offsetDirUv[v + 4] = 1.0f;		offsetDirUv[v + 5] = -1.0f;		offsetDirUv[v + 6] = 1.0f;		offsetDirUv[v + 7] = 1.0f;
			offsetDirUv[v + 8] = 0.0f;		offsetDirUv[v + 9] = 1.0f;		offsetDirUv[v + 10] = 0.5f;		offsetDirUv[v + 11] = 0.0f;
			offsetDirUv[v + 12] = 0.0f;		offsetDirUv[v + 13] = -1.0f;		offsetDirUv[v + 14] = 0.5f;		offsetDirUv[v + 15] = 1.0f;
			offsetDirUv[v + 16] = 0.0f;		offsetDirUv[v + 17] = -1.0f;		offsetDirUv[v + 18] = 0.5f;		offsetDirUv[v + 19] = 0.0f;
			offsetDirUv[v + 20] = 0.0f;		offsetDirUv[v + 21] = 1.0f;		offsetDirUv[v + 22] = 0.5f;		offsetDirUv[v + 23] = 1.0f;
			offsetDirUv[v + 24] = 1.0f;		offsetDirUv[v + 25] = -1.0f;		offsetDirUv[v + 26] = 0.0f;		offsetDirUv[v + 27] = 0.0f;
			offsetDirUv[v + 28] = 1.0f;		offsetDirUv[v + 29] = 1.0f;		offsetDirUv[v + 30] = 0.0f;		offsetDirUv[v + 31] = 1.0f;
		}

		RenderBufferPtr positionBuffer = GEngine->RenderResourceMgr->CreateBuffer(RBT_Vertex, BU_DynamicDraw, sizeof(float)* 3, maxLineNumber * 8 * 3 * sizeof(float), NULL, 0);
		RenderBufferPtr otherpositionBuffer = GEngine->RenderResourceMgr->CreateBuffer(RBT_Vertex, BU_DynamicDraw, sizeof(float)* 3, maxLineNumber * 8 * 3 * sizeof(float), NULL, 1);
		RenderBufferPtr offsetBuffer = GEngine->RenderResourceMgr->CreateBuffer(RBT_Vertex, BU_StaticDraw, sizeof(float)* 4, maxLineNumber * 4 * 8 * sizeof(float), offsetDirUv, 2);
		RenderBufferPtr colorBuffer = GEngine->RenderResourceMgr->CreateBuffer(RBT_Vertex, BU_DynamicDraw, sizeof(float)* 3, maxLineNumber * 8 * 3 * sizeof(float), NULL, 3);

		int* trisStripElements = new int[maxLineNumber * 18];
		int lineID = 0;
		for (int t = 0; t<(maxLineNumber * 18); t += 18, lineID += 8)
		{
			//0
			trisStripElements[t] = lineID;
			trisStripElements[t + 1] = lineID + 1;
			trisStripElements[t + 2] = lineID + 2;

			//1
			trisStripElements[t + 3] = lineID + 1;
			trisStripElements[t + 4] = lineID + 3;
			trisStripElements[t + 5] = lineID + 2;

			//2
			trisStripElements[t + 6] = lineID + 2;
			trisStripElements[t + 7] = lineID + 3;
			trisStripElements[t + 8] = lineID + 4;

			//3
			trisStripElements[t + 9] = lineID + 3;
			trisStripElements[t + 10] = lineID + 5;
			trisStripElements[t + 11] = lineID + 4;

			// 4
			trisStripElements[t + 12] = lineID + 4;
			trisStripElements[t + 13] = lineID + 5;
			trisStripElements[t + 14] = lineID + 7;

			// 5
			trisStripElements[t + 15] = lineID + 4;
			trisStripElements[t + 16] = lineID + 7;
			trisStripElements[t + 17] = lineID + 6;
		}

		RenderBufferPtr indexBuffer = GEngine->RenderResourceMgr->CreateBuffer(RBT_Index, BU_StaticDraw, sizeof(int), maxLineNumber * 18 * sizeof(int), trisStripElements);

		_renderLayout->UnBindVertexBuffer();
		_renderLayout->BindVertexBuffer(positionBuffer);
		_renderLayout->BindVertexBuffer(otherpositionBuffer);
		_renderLayout->BindVertexBuffer(offsetBuffer);
		_renderLayout->BindVertexBuffer(colorBuffer);
		_renderLayout->BindIndexBuffer(indexBuffer);
		_renderLayout->FinishBufferBinding(_renderEffect);

		delete[] offsetDirUv;
		delete[] trisStripElements;
	}

	BatchVolumeLines::BatchVolumeLines(std::string const& name)
		:Renderer(name)
	{
		RenderResourceManagerPtr resourceManager = GEngine->RenderResourceMgr;
		_renderEffect = resourceManager->CreateRenderEffect();
		ShaderObjectPtr vertexShader = resourceManager->CreateShader(ST_VertexShader, "VolumeLines", SM_4_0, "VS");
		ShaderObjectPtr pixelShader = resourceManager->CreateShader(ST_PixelShader, "VolumeLines", SM_4_0, "PS");
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
		_renderEffect->LinkShaders();

		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.SrcBlend = BLEND_SRC_ALPHA;
		bDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		bDesc.BlendOp = BLEND_OP_ADD;

		BlendStatePtr blendState = GEngine->RenderResourceMgr->CreateBlendState(&bDesc, 1);
		_renderEffect->BindBlendState(blendState);

		DepthStencilDesc dDesc;
		dDesc.DepthEnable = false;
		DepthStencilStatePtr depthState = GEngine->RenderResourceMgr->CreateDepthStencilState(&dDesc, 0);
		_renderEffect->BindDepthStencilState(depthState);

		ShaderPropertyManagerPtr globalProperty = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_sLineTextureProperty = globalProperty->CreateProperty(ShaderPropertyManager::sLineTexture, eSP_ShaderResource, 1);
		_sLineSamplerProperty = globalProperty->CreateProperty(ShaderPropertyManager::sLineSampler, eSP_SampleState, 1);

		SamplerDesc sdesc;
		sdesc.Filter = SF_Min_Mag_Mip_Linear;
		SamplerStatePtr samplerState = GEngine->RenderResourceMgr->CreateSamplerState(&sdesc);

		ImagePtr lineImage = GImageManager->Load(GConfig->sResourceTexPath + "LineTexture.jpg");
		RenderTexture2DPtr lineTexture = GEngine->RenderResourceMgr->CreateTexture2D(samplerState, PF_R8G8B8A8_TYPELESS, false, lineImage);
		SurfaceViewPtr lineView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, lineTexture, PF_R8G8B8A8_UNORM);
		_sLineTextureProperty->SetData(lineView);
		_sLineSamplerProperty->SetData(samplerState);
        
		_renderLayout = resourceManager->CreateRenderLayout(_renderEffect, TT_TriangleList, false);

		_vertexs = NULL;
		_positions = NULL;
		_otherPositions = NULL;
		_colors = NULL;
		_savedVertexBufferSize = 1024;

		BuildBuffer(_savedVertexBufferSize / 2);
	}

	BatchVolumeLines::~BatchVolumeLines()
	{
		if (_vertexs)
			delete[] _vertexs;

		if (_positions)
			delete[] _positions;

		if (_otherPositions)
			delete[] _otherPositions;

		if (_colors)
			delete[] _colors;

		_vertexs = 0;
		_vertexNum = 0;
	}

	BatchLineVertex* BatchVolumeLines::PrepareAddVertex()
	{
		if (_vertexNum + 2 >= _savedVertexBufferSize)
		{
			_savedVertexBufferSize *= 2;
			BuildBuffer(_savedVertexBufferSize/2);
		}

		return &(_vertexs[_vertexNum]);
	}

	void BatchVolumeLines::EndAddVertex()
	{
		_vertexNum += 2;
	}

	void BatchVolumeLines::Render(CameraPtr const& camera)
	{
		BOOST_ASSERT(_vertexNum % 2 == 0);

		if (_vertexNum == 0)
			return;

		for (unsigned int l = 0; l < _vertexNum/2; l++)
		{
			const int lA = l * 2;
			const int lB = l * 2 + 1;
			const int l24 = l * 24;
			_positions[l24] = _vertexs[lA].position.x;		    _positions[l24 + 1] = _vertexs[lA].position.y;		_positions[l24 + 2] = _vertexs[lA].position.z;
			_positions[l24 + 3] = _vertexs[lA].position.x;		_positions[l24 + 4] = _vertexs[lA].position.y;		_positions[l24 + 5] = _vertexs[lA].position.z;
			_positions[l24 + 6] = _vertexs[lA].position.x;		_positions[l24 + 7] = _vertexs[lA].position.y;		_positions[l24 + 8] = _vertexs[lA].position.z;
			_positions[l24 + 9] = _vertexs[lA].position.x;		_positions[l24 + 10] = _vertexs[lA].position.y;		_positions[l24 + 11] = _vertexs[lA].position.z;

			_positions[l24 + 12] = _vertexs[lB].position.x;		_positions[l24 + 13] = _vertexs[lB].position.y;		_positions[l24 + 14] = _vertexs[lB].position.z;
			_positions[l24 + 15] = _vertexs[lB].position.x;		_positions[l24 + 16] = _vertexs[lB].position.y;		_positions[l24 + 17] = _vertexs[lB].position.z;
			_positions[l24 + 18] = _vertexs[lB].position.x;		_positions[l24 + 19] = _vertexs[lB].position.y;		_positions[l24 + 20] = _vertexs[lB].position.z;
			_positions[l24 + 21] = _vertexs[lB].position.x;		_positions[l24 + 22] = _vertexs[lB].position.y;		_positions[l24 + 23] = _vertexs[lB].position.z;
		}

		for (unsigned int l = 0; l< _vertexNum / 2; ++l)
		{
			const int lA = l * 2;
			const int lB = l * 2 + 1;
			const int l24 = l * 24;
			_otherPositions[l24] = _vertexs[lB].position.x;		    _otherPositions[l24 + 1] = _vertexs[lB].position.y;		_otherPositions[l24 + 2] = _vertexs[lB].position.z;
			_otherPositions[l24 + 3] = _vertexs[lB].position.x;		_otherPositions[l24 + 4] = _vertexs[lB].position.y;		_otherPositions[l24 + 5] = _vertexs[lB].position.z;
			_otherPositions[l24 + 6] = _vertexs[lB].position.x;		_otherPositions[l24 + 7] = _vertexs[lB].position.y;		_otherPositions[l24 + 8] = _vertexs[lB].position.z;
			_otherPositions[l24 + 9] = _vertexs[lB].position.x;		_otherPositions[l24 + 10] = _vertexs[lB].position.y;	_otherPositions[l24 + 11] = _vertexs[lB].position.z;

			_otherPositions[l24 + 12] = _vertexs[lA].position.x;	_otherPositions[l24 + 13] = _vertexs[lA].position.y;    _otherPositions[l24 + 14] = _vertexs[lA].position.z;
			_otherPositions[l24 + 15] = _vertexs[lA].position.x;	_otherPositions[l24 + 16] = _vertexs[lA].position.y;    _otherPositions[l24 + 17] = _vertexs[lA].position.z;
			_otherPositions[l24 + 18] = _vertexs[lA].position.x;	_otherPositions[l24 + 19] = _vertexs[lA].position.y;	_otherPositions[l24 + 20] = _vertexs[lA].position.z;
			_otherPositions[l24 + 21] = _vertexs[lA].position.x;	_otherPositions[l24 + 22] = _vertexs[lA].position.y;	_otherPositions[l24 + 23] = _vertexs[lA].position.z;
		}

		for (unsigned int l = 0; l < _vertexNum / 2; l++)
		{
			const int lA = l * 2;
			const int lB = l * 2 + 1;
			const int l24 = l * 24;
			_colors[l24] = _vertexs[lA].color.x;		    _colors[l24 + 1] = _vertexs[lA].color.y;		_colors[l24 + 2] = _vertexs[lA].color.z;
			_colors[l24 + 3] = _vertexs[lA].color.x;		_colors[l24 + 4] = _vertexs[lA].color.y;		_colors[l24 + 5] = _vertexs[lA].color.z;
			_colors[l24 + 6] = _vertexs[lA].color.x;		_colors[l24 + 7] = _vertexs[lA].color.y;		_colors[l24 + 8] = _vertexs[lA].color.z;
			_colors[l24 + 9] = _vertexs[lA].color.x;		_colors[l24 + 10] = _vertexs[lA].color.y;		_colors[l24 + 11] = _vertexs[lA].color.z;

			_colors[l24 + 12] = _vertexs[lB].color.x;		_colors[l24 + 13] = _vertexs[lB].color.y;		_colors[l24 + 14] = _vertexs[lB].color.z;
			_colors[l24 + 15] = _vertexs[lB].color.x;		_colors[l24 + 16] = _vertexs[lB].color.y;		_colors[l24 + 17] = _vertexs[lB].color.z;
			_colors[l24 + 18] = _vertexs[lB].color.x;		_colors[l24 + 19] = _vertexs[lB].color.y;		_colors[l24 + 20] = _vertexs[lB].color.z;
			_colors[l24 + 21] = _vertexs[lB].color.x;		_colors[l24 + 22] = _vertexs[lB].color.y;		_colors[l24 + 23] = _vertexs[lB].color.z;
		}

		RenderEnginePtr renderEngine = GEngine->RenderEngine;
		unsigned int bufferSize = _vertexNum / 2 * 8 * 3 * sizeof(float);
		const RenderBufferPtr &positionBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(positionBuffer, BA_Write_Only);
		memcpy(vertexBuffer, _positions, bufferSize);
		renderEngine->UnMap(positionBuffer);

		const RenderBufferPtr &otherPositionBuffer = _renderLayout->GetVertexBuffers()[1];
		vertexBuffer = renderEngine->Map(otherPositionBuffer, BA_Write_Only);
		memcpy(vertexBuffer, _otherPositions, bufferSize);
		renderEngine->UnMap(otherPositionBuffer);

		const RenderBufferPtr &colorBuffer = _renderLayout->GetVertexBuffers()[3];
		vertexBuffer = renderEngine->Map(colorBuffer, BA_Write_Only);
		memcpy(vertexBuffer, _colors, bufferSize);
		renderEngine->UnMap(colorBuffer);
 
		renderEngine->SetRenderLayout(_renderLayout);

		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_vertexNum/2 * 18, 0,0);
		_vertexNum = 0;
	}


	//////////////////////////////////////////////////////////////////////////

	GeometryRenderer::GeometryRenderer(std::string const& name)
		:Renderer(name)
	{
	}

	bool GeometryRenderer::Overlaps(const Frustrum& frustrum)
	{
		return frustrum.Overlaps(_boxBounds);
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
		UpdateBoundingBox();
	}

	void GeometryRenderer::UpdateBoundingBox()
	{
		GameObjectPtr go = GetBase();
		if (go == NULL)
			return;

		const glm::mat4& worldMat = go->GetWorldMatrix();
		_boxBounds = _geometryObject->BoundingBox.GetBox();
		std::vector<glm::vec3> corners;
		_boxBounds.GetCorners(corners);
		for (size_t i = 0; i < corners.size(); i++)
		{
			Math::Transform(worldMat, corners[i]);
		}
	 
		_boxBounds = BoxBounds(corners.data(), corners.size());
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

	 void GeometryRenderer::UpdateShaderProperty()
	 {
		 GetBase()->UpdateShaderProperty();
		 if( _material != NULL )
		 {
			_material->UpdateShaderProperty();
		 }
	 }

	 void GeometryRenderer::DrawBoundingBox(CameraPtr const& camera)
	 {

		/* glm::vec4 boxVec1( _geometryObject->BoundingBox.Origin.x - _geometryObject->BoundingBox.BoxExtent.x,
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
		 boxVec8 = boxVec8 / boxVec8.w;*/
	 

		 //glm::vec3 b1(boxVec1.x, boxVec1.y, boxVec1.z);
		 //glm::vec3 b2(boxVec2.x, boxVec2.y, boxVec2.z);
		 //glm::vec3 b3(boxVec3.x, boxVec3.y, boxVec3.z);
		 //glm::vec3 b4(boxVec4.x, boxVec4.y, boxVec4.z);
		 //glm::vec3 b5(boxVec5.x, boxVec5.y, boxVec5.z);
		 //glm::vec3 b6(boxVec6.x, boxVec6.y, boxVec6.z);
		 //glm::vec3 b7(boxVec7.x, boxVec7.y, boxVec7.z);
		 //glm::vec3 b8(boxVec8.x, boxVec8.y, boxVec8.z);

		 glm::vec3 b1(_boxBounds.BMin.x, _boxBounds.BMin.y, _boxBounds.BMin.z);
		 glm::vec3 b2(_boxBounds.BMax.x, _boxBounds.BMin.y, _boxBounds.BMin.z);
		 glm::vec3 b3(_boxBounds.BMax.x, _boxBounds.BMin.y, _boxBounds.BMax.z);
		 glm::vec3 b4(_boxBounds.BMin.x, _boxBounds.BMin.y, _boxBounds.BMax.z);
		 glm::vec3 b5(_boxBounds.BMin.x, _boxBounds.BMax.y, _boxBounds.BMax.z);
		 glm::vec3 b6(_boxBounds.BMax.x, _boxBounds.BMax.y, _boxBounds.BMax.z);
		 glm::vec3 b7(_boxBounds.BMax.x, _boxBounds.BMax.y, _boxBounds.BMin.z);
		 glm::vec3 b8(_boxBounds.BMin.x, _boxBounds.BMax.y, _boxBounds.BMin.z);

		 glm::vec4 color(1.0f,0,0,1.0f);
		 GEngine->GameCanvas->DrawLine(b1,color,b2,color);
		 GEngine->GameCanvas->DrawLine(b2,color,b3,color);
		 GEngine->GameCanvas->DrawLine(b3,color,b4,color);
		 GEngine->GameCanvas->DrawLine(b4,color,b1,color);

		 GEngine->GameCanvas->DrawLine(b4,color,b5,color);
	     GEngine->GameCanvas->DrawLine(b1,color,b8,color);
		 GEngine->GameCanvas->DrawLine(b2,color,b7,color);
	     GEngine->GameCanvas->DrawLine(b3,color,b6,color);

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
		 glm::vec3 original(0, 0, 0);

		 float AxisLength = _geometryObject->BoundingBox.GetSphere().Radius*1.5f;
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

	void GeometryRenderer::RenderShadow(CameraPtr const& camera, RenderEffectPtr shadowEffect)
	{
		if (_shadowLayout == NULL)
		{
			_shadowLayout = GEngine->RenderResourceMgr->CreateRenderLayout(shadowEffect, TT_TriangleList, true);
			_shadowLayout->BindVertexBuffer(_renderLayout->GetVertexBuffers()[0]);
			_shadowLayout->BindIndexBuffer(_renderLayout->GetIndexBuffer());
			_shadowLayout->FinishBufferBinding(shadowEffect);
		}

		if (_renderEffect == NULL)
			return;

		GameObjectPtr gameObject = _baseObject.lock();
		RenderEnginePtr renderEngine = GEngine->RenderEngine;

		renderEngine->SetRenderLayout(_shadowLayout);
		renderEngine->SetEffect(shadowEffect);
		renderEngine->DrawIndexed(_geometryObject->Indices.size(), 0, 0);
	}



	
}