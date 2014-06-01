#ifndef _DISORDER_RENDERTECHNIQUE_H_
#define _DISORDER_RENDERTECHNIQUE_H_


namespace Disorder
{

	class ShaderObject
	{
	public:
	    ShaderType const& GetType() const
		{
			return _type;
		}
 
		std::string GetShaderName()
		{
			std::string result = _fileName + ":" + _entryPoint;
		    return result;
		}
 
		virtual void  UpdateShaderParameter() {;}
		virtual void* GetHandle(){ return 0;}
		virtual void* GetDataInterface() { return 0;}
 
		virtual bool LoadShaderFromFile(std::string const& fileName,std::string const& entryPoint,ShaderType shaderType) = 0;
		virtual void ShaderReflection() = 0;

		static void SetShaderMacro(std::string const& name,std::string const& value);
		static void RemoveShaderMacro(std::string const& name);

		static std::map<std::string,std::string> sMapIncludeFiles; // key: filename, value: content. 

		virtual void DumpContent();

	protected:
		std::string _fileName;
		std::string _entryPoint;
		ShaderType _type;
		ShaderModel _shaderModel;
		static std::map<std::string,std::string> _mapShaderMacro;       // key: name, value : definition.
		
	};
 
	class RenderEffect
	{
	public: 
		inline ShaderObjectPtr const& GetVertexShader() const
		{
			return _vertexShader;
		}

		inline ShaderObjectPtr const& GetPixelShader() const
		{
			return _pixelShader;
		}

		inline ShaderObjectPtr const& GetGeometryShader() const
		{
			return _geometryShader;
		}

		virtual void BindShader(ShaderObjectPtr const& shaderObject)
		{
			if( shaderObject->GetType() == ST_VertexShader )
				_vertexShader = shaderObject;
			else if (shaderObject->GetType() == ST_PixelShader)
				_pixelShader = shaderObject;
			else if (shaderObject->GetType() == ST_GeometryShader)
				_geometryShader = shaderObject;
			else
			   BOOST_ASSERT(0);

		}

		virtual void LinkShaders()
		{
		}

		void BindRasterizeState(RasterizeStatePtr const& rasterizeState)
		{
			_rasterizeState = rasterizeState;
		}

		void BindBlendState(BlendStatePtr const& blendState)
		{
			_blendState = blendState;
		}

		void BindDepthStencilState(DepthStencilStatePtr const& depthStencilState)
		{
			_depthStencilState = depthStencilState;
		}

		BlendStatePtr const& GetBlendState()
		{
			return _blendState;
		}

		RasterizeStatePtr const& GetRasterizeState()
		{
			return _rasterizeState;
		}

		DepthStencilStatePtr const& GetDepthStencilState()
		{
			return _depthStencilState;
		}
 
		virtual void UpdateShaderParameter();

		static RenderEffectPtr Create(); 

		virtual void* GetHandle()
		{
			return 0;
		}

	protected:
		// shader slot!
		ShaderObjectPtr _vertexShader;
		ShaderObjectPtr _pixelShader;
		ShaderObjectPtr _geometryShader;

		RasterizeStatePtr _rasterizeState;
		BlendStatePtr _blendState;
		DepthStencilStatePtr _depthStencilState;
 
		RenderEffect()
		{
			_rasterizeState = RenderResourceManager::DefaultRasterizeState;
			_blendState = RenderResourceManager::DefaultBlentState;
			_depthStencilState = RenderResourceManager::DefaultDepthStencilState;
		}
	};


}


#endif