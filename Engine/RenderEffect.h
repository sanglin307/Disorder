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
		inline ShaderObject* GetVertexShader() const
		{
			return _vertexShader;
		}

		inline ShaderObject* GetPixelShader() const
		{
			return _pixelShader;
		}

		inline ShaderObject* GetGeometryShader() const
		{
			return _geometryShader;
		}

		virtual void BindShader(ShaderObject * shaderObject)
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

		void BindRasterizeState(RasterizeState *rasterizeState)
		{
			_rasterizeState = rasterizeState;
		}

		void BindBlendState(BlendState *blendState)
		{
			_blendState = blendState;
		}

		void BindDepthStencilState(DepthStencilState *depthStencilState)
		{
			_depthStencilState = depthStencilState;
		}

		BlendState* GetBlendState() const
		{
			return _blendState;
		}

		RasterizeState *GetRasterizeState() const
		{
			return _rasterizeState;
		}

		DepthStencilState* GetDepthStencilState() const
		{
			return _depthStencilState;
		}
 
		virtual void UpdateShaderParameter();
 
		virtual void* GetHandle()
		{
			return 0;
		}

		RenderEffect()
		{
			_rasterizeState = GRenderResourceMgr->DefaultRasterizeState;
			_blendState = GRenderResourceMgr->DefaultBlentState;
			_depthStencilState = GRenderResourceMgr->DefaultDepthStencilState;
		}

	protected:
		// shader slot!
		ShaderObject* _vertexShader;
		ShaderObject* _pixelShader;
		ShaderObject* _geometryShader;

		RasterizeState* _rasterizeState;
		BlendState* _blendState;
		DepthStencilState* _depthStencilState;
 
	
	};


}


#endif