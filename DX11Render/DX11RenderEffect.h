#ifndef _DISORDER_DX11RENDERTECHNIQUE_H_
#define _DISORDER_DX11RENDERTECHNIQUE_H_


namespace Disorder
{
	class DX11ShaderObject : public ShaderObject
	{
	public:
 
		explicit DX11ShaderObject(std::string const& entryPoint,ShaderType shaderType)
		{
			_entryPoint = entryPoint;
			_type = shaderType;

		}

		virtual void BindConstBuffer(RenderBufferPtr const& constBuffer)
		{
			ShaderObject::BindConstBuffer(constBuffer);
			ConstBufferArray.push_back((ID3D11Buffer*)(constBuffer->GetLowInterface()));
		}

		virtual void BindShaderResource(RenderViewPtr const& shaderView)
		{
			ShaderObject::BindShaderResource(shaderView);
			ShaderResourceViewArray.push_back((ID3D11ShaderResourceView*)(shaderView->GetLowInterface()));
		}

		virtual void BindSamplerState(SamplerStatePtr const& samplerState)
		{
			ShaderObject::BindSamplerState(samplerState);
			SamplerStateArray.push_back((ID3D11SamplerState*)(samplerState->GetLowInterface()));
		}

		virtual void Load(std::string const& entryPoint);
		virtual void* GetLowInterface();
		virtual void* GetDataInterface();
		
	public:
		ID3D11VertexShaderPtr VertexShaderInterface;
		ID3D11PixelShaderPtr  PixelShaderInterface;
		ID3DBlobPtr DataInterface;
		std::vector<ID3D11Buffer*> ConstBufferArray;
		std::vector<ID3D11SamplerState*> SamplerStateArray;
		std::vector<ID3D11ShaderResourceView*> ShaderResourceViewArray;

	};
 
	class DX11RenderEffect : public RenderEffect
	{
	private:
		HRESULT CompileShaderFromFile(std::string const& fileName, std::string const& entryPoint, std::string const& shaderModel, ID3DBlob** ppBlobOut);

		inline std::string GetShaderModelDes(ShaderType type)
		{
			if( _shaderModel == SM_4_0 )
			{
				if( type == ST_VertexShader )
				    return "vs_4_0";
				else if( type == ST_PixelShader )
					return "ps_4_0";
			}

			BOOST_ASSERT(0);

			return "";
		};

		 

	public:

		DX11RenderEffect(ShaderModel shaderModel):
		  RenderEffect(shaderModel)
		{
			 
		};

		virtual ShaderObjectPtr LoadShaderFromFile(std::string const& fileName,std::string const& entryPoint,ShaderType shaderType);
	 

	};
}

#endif