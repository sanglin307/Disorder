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

	protected:
		std::string _entryPoint;
		ShaderType _type;
		std::vector<RenderBufferPtr> _constBuffers;
		std::vector<RenderViewPtr> _shaderResources;
		std::vector<SamplerStatePtr> _samplerStates;

	};

	class VertexShader : public ShaderObject
	{
	public:
		VertexShader()
		{
			_type = ST_VertexShader;
		}
 
		virtual void Load(std::string const& entryPoint) = 0;

	protected:
		
	};

	class PixelShader : public ShaderObject
	{
	public:
		PixelShader()
		{
			_type = ST_PixelShader;
		}

		virtual void Load(std::string const& entryPoint) = 0;
	};

	class RenderTechnique
	{

	public: 
		RenderTechnique(ShaderModel shaderModel)
		{
			_shaderModel = shaderModel;
		}

		virtual VertexShaderPtr LoadVertexShaderFromFile(std::string const& fileName,std::string const& entryPoint) = 0;
		virtual PixelShaderPtr LoadPixelShaderFromFile(std::string const& fileName,std::string const& entryPoint) = 0;


		inline VertexShaderPtr const& GetVertexShader() const
		{
			return _vertexShader;
		}

		inline PixelShaderPtr const& GetPixelShader() const
		{
			return _pixelShader;
		}

	protected:
		ShaderModel _shaderModel;
		VertexShaderPtr _vertexShader;
		PixelShaderPtr _pixelShader;



	};


}


#endif