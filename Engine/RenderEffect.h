#ifndef _DISORDER_RENDERTECHNIQUE_H_
#define _DISORDER_RENDERTECHNIQUE_H_


namespace Disorder
{
	enum ShaderModel
	{
		SM_4_0
	};

	enum ShaderType
	{
		ST_VertexShader,
		ST_PixelShader,
		ST_GeometryShader,
		ST_ComputeShader,
		ST_HullShader,
		ST_DomainShader,

		ST_NumShaderTypes
	};

	class ShaderReflection
	{
	};


	class ShaderObject
	{
	public:
		virtual void Load(std::string const& entryPoint) = 0;

		

	    ShaderType const& GetType() const
		{
			return _type;
		}

		virtual void BindConstBuffer(RenderBufferPtr const& constBuffer)
		{
			_constBuffers.push_back(constBuffer);
		}

		virtual void BindShaderResource(RenderViewPtr const& shaderView)
		{
			_shaderResources.push_back(shaderView);
		}

		virtual void BindSamplerState(SamplerStatePtr const& samplerState)
		{
			_samplerStates.push_back(samplerState);
		}

		std::vector<RenderBufferPtr> const& GetConstBuffers()
		{
			return _constBuffers;
		}

		std::vector<RenderViewPtr> const& GetShaderResources()
		{
			return _shaderResources;
		}

		std::vector<SamplerStatePtr> const& GetSamplerStates()
		{
			return _samplerStates;
		}

		virtual void* GetLowInterface(){ return 0;}
		virtual void* GetDataInterface() { return 0;}

	public :
		ShaderReflectionPtr ShaderReflect;

	protected:
		std::string _entryPoint;
		ShaderType _type;
		std::vector<RenderBufferPtr> _constBuffers;
		std::vector<RenderViewPtr> _shaderResources;
		std::vector<SamplerStatePtr> _samplerStates;
		
	};
 
	class RenderEffect
	{

	typedef boost::unordered_map<std::string,MaterialParamPtr> MaterialParamMap;
	public: 
		RenderEffect(ShaderModel shaderModel)
		{
			_shaderModel = shaderModel;
		}

		virtual ShaderObjectPtr LoadShaderFromFile(std::string const& fileName,std::string const& entryPoint,ShaderType shaderType) = 0;
	 
		inline ShaderObjectPtr const& GetVertexShader() const
		{
			return _vertexShader;
		}

		inline ShaderObjectPtr const& GetPixelShader() const
		{
			return _pixelShader;
		}

			// Material Param
		virtual MaterialParamPtr GetConstantBufferParameter(std::string const& name) = 0;
		virtual MaterialParamPtr GetVectorParameter(std::string const& name) = 0;
		virtual MaterialParamPtr GetMatrixParameter(std::string const& name) = 0;
		virtual MaterialParamPtr GetShaderResourceParameter(std::string const& name) =0;
		virtual MaterialParamPtr GetSamplerStateParameter(std::string const& name) = 0;
		virtual MaterialParamPtr GetUnorderedAccessParameter(std::string const& name) = 0;

	protected:
		ShaderModel _shaderModel;
		MaterialParamMap _materialParamMap;
		// shader slot!
		ShaderObjectPtr _vertexShader;
		ShaderObjectPtr _pixelShader;
 
	};


}


#endif