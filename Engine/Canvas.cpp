#include "EngineInclude.h"

namespace Disorder
{
	void CanvasBatchElement::SetTexture(RenderTexture2DPtr const& texture)
	{
		_texture = texture;
		
		ShaderObjectPtr pixelShader = _renderEffect->GetPixelShader();
		RenderViewPtr renderView = GEngine->RenderEngine->ResourceManager->CreateTexture2DView(texture);
 
		MaterialParamShaderResPtr shaderres = _renderEffect->GetShaderResourceParameter("DiffuseTexture");
		shaderres->SetValue(renderView);
	  
		MaterialParamSamplerStatePtr msampler = _renderEffect->GetSamplerStateParameter("LinearSampler");
		msampler->SetValue(texture->Sampler);

		_renderEffect->PrepareRenderParam();
	}

	void CanvasBatchElement::Draw(MaterialViewType view)
	{
		BOOST_ASSERT(_vertexs.size()%4==0);

		if( _vertexs.size() == 0 )
			return;

		RenderEnginePtr renderEngine = GEngine->RenderEngine;
	

		const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
		void* vertexBuffer = renderEngine->Map(vertexRenderBuffer,BA_Write_Only);
		memcpy(vertexBuffer,_vertexs.data(),_vertexs.size()*sizeof(SimpleVertex));
		renderEngine->UnMap(vertexRenderBuffer);

		const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
		void* indexBuffer = renderEngine->Map(indexRenderBuffer,BA_Write_Only);
		memcpy(indexBuffer,_indexs.data(),_indexs.size()*sizeof(WORD));
		renderEngine->UnMap(indexRenderBuffer);

		renderEngine->SetRenderLayout(_renderLayout);
		renderEngine->SetBlendState(_renderEffect->GetBlendState());
		renderEngine->SetRasterizeState(_renderEffect->GetRasterizeState());
		renderEngine->SetEffect(_renderEffect);
		renderEngine->DrawIndexed(_indexs.size(),0,0);

		_indexs.clear();
		_vertexs.clear();
	}

	unsigned int CanvasBatchElement::GetCurrentDrawTriNumber()
	{
		return _indexs.size() /3;
	}

	void CanvasBatchElement::AddVertex(Vector3 const& position,Vector4 const& color,Vector2 const& texcoord)
	{
		if( _vertexs.size() >= _savedVertexBufferSize )
		{
			_savedVertexBufferSize *= 2;
			const RenderBufferPtr & vertexRenderBuffer = _renderLayout->GetVertexBuffers()[0];
			vertexRenderBuffer->Resize(sizeof(SimpleVertex) * _savedVertexBufferSize);
		}

		if( _indexs.size() >= _savedIndexBufferSize )
		{
			_savedIndexBufferSize *= 2;
			const RenderBufferPtr & indexRenderBuffer = _renderLayout->GetIndexBuffer();
			indexRenderBuffer->Resize(sizeof(WORD) * _savedIndexBufferSize);
		}

	

		SimpleVertex vertex;
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

	CanvasBatchElement::CanvasBatchElement()
	{
		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		_renderEffect =  resourceManager->CreateRenderEffect("2DFX.fx",SM_4_0,"VS","PS");

		ShaderObjectPtr vertexShader = _renderEffect->GetVertexShader();
		//ShaderObjectPtr pixelShader = _renderEffect->GetPixelShader();
 
		BlendDesc blendDesc;
		blendDesc.BlendEnable = true;
		blendDesc.SrcBlend = BLEND_SRC_ALPHA;
		blendDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = BLEND_OP_ADD;
		 
		BlendStatePtr blendState = resourceManager->CreateBlendState(&blendDesc,1);
		_renderEffect->BindBlendState(blendState);

	    _renderLayout = resourceManager->CreateRenderLayout(vertexShader,TT_TriangleList);
	 
		_savedVertexBufferSize = 2048;
		_savedIndexBufferSize = (UINT)(_savedVertexBufferSize * 1.5f);
 
		RenderBufferPtr vertexBuffer = resourceManager->CreateRenderBuffer(RBT_Vertex,BAH_GPU_Read | BAH_CPU_Write,sizeof(SimpleVertex),sizeof(SimpleVertex)*_savedVertexBufferSize,NULL);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		//Index buffer
		RenderBufferPtr indexBuffer = resourceManager->CreateRenderBuffer(RBT_Index,BAH_GPU_Read | BAH_CPU_Write,sizeof(WORD),sizeof(WORD)*_savedIndexBufferSize,NULL);
		_renderLayout->BindIndexBuffer(indexBuffer);

	
 
	}


	Canvas::Canvas(unsigned int width,unsigned int height)
		:_width(width),_height(height)
	{
		_font = GFontManager->CreateFontFromFile("calibri.ttf",20,96);
		_stringElement.SetTexture(_font->GetGlyphTexture());
	}

	void Canvas::Draw()
	{
		_stringElement.Draw(MVT_Perspective);
	}

	unsigned int Canvas::GetCurrentDrawTriNumber()
	{
		return _stringElement.GetCurrentDrawTriNumber();
	}

	void Canvas::DrawString(float xPos,float yPos,float size,Vector4 const& color,std::string const& str)
	{
		 //draw string x[-1.0,1.0] and y[-1.0,1.0] z = 0.0
		float xbegin = (xPos - _width / 2 )*2/_width;
		float ybegin = (yPos - _height / 2 )*(-2)/_height;
		float z = 0.0f;
		float charSize = 2*size / _height;

		// we draw char one by one
		for(int index=0;index<str.length();index++)
		{
			UINT c = str[index];

			if( c== 0x20) // Space
			{
				xbegin += charSize/4;
				continue;
			}

			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			 
			float drawSizeX = charSize * rect.aspectRatio ;
			_stringElement.AddVertex(Vector3(xbegin,ybegin,z),color,Vector2(rect.uvRect.left,rect.uvRect.top));
			_stringElement.AddVertex(Vector3(xbegin+drawSizeX,ybegin,z),color,Vector2(rect.uvRect.right,rect.uvRect.top));
			_stringElement.AddVertex(Vector3(xbegin,ybegin-charSize,z),color,Vector2(rect.uvRect.left,rect.uvRect.bottom));
			_stringElement.AddVertex(Vector3(xbegin+drawSizeX,ybegin - charSize,z),color,Vector2(rect.uvRect.right,rect.uvRect.bottom));

			xbegin += drawSizeX;

		}




	}
}