#ifndef _DISORDER_DX11RENDERTECHNIQUE_H_
#define _DISORDER_DX11RENDERTECHNIQUE_H_


namespace Disorder
{
	class DX11VertexShader : public VertexShader
	{
	public:
		DX11VertexShader(std::string const& entryPoint)
		{
			_entryPoint = entryPoint;
		};

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
		ID3D11VertexShaderPtr ShaderInterface;
		ID3DBlobPtr DataInterface;
		std::vector<ID3D11Buffer*> ConstBufferArray;
		std::vector<ID3D11SamplerState*> SamplerStateArray;
		std::vector<ID3D11ShaderResourceView*> ShaderResourceViewArray;

	};

	class DX11PixelShader : public PixelShader
	{
	public:
	    DX11PixelShader(std::string const& entryPoint)
		{
			_entryPoint = entryPoint;
		};
		virtual void Load(std::string const& entryPoint);
	    virtual void* GetLowInterface();
		virtual void* GetDataInterface();

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

	public:
		ID3D11PixelShaderPtr ShaderInterface;
		ID3DBlobPtr DataInterface;
		std::vector<ID3D11Buffer*> ConstBufferArray;
		std::vector<ID3D11SamplerState*> SamplerStateArray;
		std::vector<ID3D11ShaderResourceView*> ShaderResourceViewArray;
	};

	class DX11RenderTechnique : public RenderTechnique
	{
	private:
		HRESULT CompileShaderFromFile(std::string const& fileName, std::string const& entryPoint, std::string const& shaderModel, ID3DBlob** ppBlobOut);

		inline std::string GetVertexShaderModelDes()
		{
			if( _shaderModel == SM_4_0 )
				return "vs_4_0";

			return "";
		};

		inline std::string GetPixelShaderModelDes()
		{
			if( _shaderModel == SM_4_0 )
			    return "ps_4_0";

			return "";
		};

	public:

		DX11RenderTechnique(ShaderModel shaderModel):
		  RenderTechnique(shaderModel)
		{
			 
		};

		virtual VertexShaderPtr LoadVertexShaderFromFile(std::string const& fileName,std::string const& entryPoint);
		virtual PixelShaderPtr LoadPixelShaderFromFile(std::string const& fileName,std::string const& entryPoint);


	};
}

#endif