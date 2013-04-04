#ifndef _DISORDER_DX11RENDERTECHNIQUE_H_
#define _DISORDER_DX11RENDERTECHNIQUE_H_


namespace Disorder
{
	struct ConstantBufferDesc
	{
		D3D11_SHADER_BUFFER_DESC				Description;
		std::vector<D3D11_SHADER_VARIABLE_DESC>	Variables;
		std::vector<D3D11_SHADER_TYPE_DESC>		Types;
		std::vector<MaterialParamPtr>		    Parameters;
		MaterialParamPtr					    BufferParamRef;
	};

	struct ShaderInputBindDesc
	{
		ShaderInputBindDesc( D3D11_SHADER_INPUT_BIND_DESC desc )
		{
			Name = std::string(desc.Name);
			Type = desc.Type;
			BindPoint = desc.BindPoint;
			BindCount = desc.BindCount;
			uFlags = desc.uFlags;
			ReturnType = desc.ReturnType;
			Dimension = desc.Dimension;
			NumSamples = desc.NumSamples;
			ParamRef = 0;
		};
		
		ShaderInputBindDesc()
		{
			Name = "";
			Type = D3D_SIT_CBUFFER;
			BindPoint = 0;
			BindCount = 0;
			uFlags = 0;
			ReturnType = D3D11_RETURN_TYPE_UNORM;
			Dimension = D3D_SRV_DIMENSION_UNKNOWN;
			NumSamples = 0;
			ParamRef = 0;
		};

		std::string Name;
		D3D_SHADER_INPUT_TYPE Type;
		UINT BindPoint;
		UINT BindCount;
		UINT uFlags;
		D3D11_RESOURCE_RETURN_TYPE ReturnType;
		D3D_SRV_DIMENSION Dimension;
		UINT NumSamples;
		MaterialParamPtr ParamRef;
	};

	class DX11ShaderReflection : public ShaderReflection
	{
	public:

		/*void InitializeConstantBuffers( IParameterManager* pParamManager );
		void UpdateParameters( PipelineManagerDX11* pPipeline, IParameterManager* pParamManager );
		void BindParameters( ShaderType type, PipelineManagerDX11* pPipeline, IParameterManager* pParamManager );*/

		void PrintShaderDetails();

	public:
		D3D11_SHADER_DESC								ShaderDescription;
		std::vector<D3D11_SIGNATURE_PARAMETER_DESC>		InputSignatureParameters;
		std::vector<D3D11_SIGNATURE_PARAMETER_DESC>		OutputSignatureParameters;
		std::vector<ConstantBufferDesc>				    ConstantBuffers;
		std::vector<ShaderInputBindDesc>				ResourceBindings;
	};

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
	 
		virtual void ShaderReflection(ShaderObjectPtr const& shader);

		// Material Param
		virtual MaterialParamPtr GetConstantBufferParameter(std::string const& name);
		virtual MaterialParamPtr GetVectorParameter(std::string const& name);
		virtual MaterialParamPtr GetMatrixParameter(std::string const& name);
		virtual MaterialParamPtr GetShaderResourceParameter(std::string const& name);
		virtual MaterialParamPtr GetSamplerStateParameter(std::string const& name);
		virtual MaterialParamPtr GetUnorderedAccessParameter(std::string const& name);
	};
}

#endif