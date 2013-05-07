#ifndef _DISORDER_RENDERTECHNIQUE_H_
#define _DISORDER_RENDERTECHNIQUE_H_


namespace Disorder
{
	
 
	class ShaderObject
	{
	public:
		//virtual void Load(std::string const& entryPoint) = 0;
 
	    ShaderType const& GetType() const
		{
			return _type;
		}
 
		std::string GetShaderName()
		{
			std::string result = _effectName + ":" + _entryPoint;
		    return result;
		}
 
		virtual void  PrepareRenderParam() {;}
		virtual void  UpdateRenderParam() {;}
		virtual void* GetLowInterface(){ return 0;}
		virtual void* GetDataInterface() { return 0;}
 
		
	protected:
		std::string _effectName;
		std::string _entryPoint;
		ShaderType _type;
	};
 
	class RenderEffect
	{

	public: 
		RenderEffect(ShaderModel shaderModel)
		{
			_shaderModel = shaderModel;
			_rasterizeState = RenderResourceManager::DefaultRasterizeState;
			_blendState = RenderResourceManager::DefaultBlentState;

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

		void BindRasterizeState(RasterizeStatePtr const& rasterizeState)
		{
			_rasterizeState = rasterizeState;
		}

		void BindBlendState(BlendStatePtr const& blendState)
		{
			_blendState = blendState;
		}

		BlendStatePtr const& GetBlendState()
		{
			return _blendState;
		}

		RasterizeStatePtr const& GetRasterizeState()
		{
			return _rasterizeState;
		}

		virtual void PrepareRenderParam() = 0;
		virtual void UpdateRenderParam() = 0;

        virtual void ShaderReflection(ShaderObjectPtr const& shader) = 0;

	protected:
		ShaderModel _shaderModel;	
		// shader slot!
		ShaderObjectPtr _vertexShader;
		ShaderObjectPtr _pixelShader;
		RasterizeStatePtr _rasterizeState;
		BlendStatePtr _blendState;
 
	};


}


#endif