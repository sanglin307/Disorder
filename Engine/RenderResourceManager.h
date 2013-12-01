#ifndef _DISORDER_RENDERRESOURCEMANAGER_H_
#define _DISORDER_RENDERRESOURCEMANAGER_H_


namespace Disorder
{
	class RenderResourceManager 
	{
		typedef boost::unordered_map<std::string,ShaderObjectPtr>  ShaderMap;
		typedef boost::unordered_map<std::string,ShaderPropertyManagerPtr> ShaderPropertyManagerMap;
		 
	public:
		virtual void Init();
		virtual ShaderObjectPtr CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint) = 0;
		virtual RenderLayoutPtr CreateRenderLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer) = 0;
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,GeometryPtr const& data,std::string const& sematic,ShaderObjectPtr const& vertexShader) = 0;
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData) = 0;
		virtual void CreateRenderBufferArray(GeometryPtr const& data, unsigned int accessHint,ShaderObjectPtr const& vertexShader,std::vector<RenderBufferPtr> & bufferArray ) = 0;
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int hight,bool bMipmap,BufferInitData const* pData) = 0;
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,ImagePtr image) = 0;
		virtual SamplerStatePtr CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16) = 0;
	
		virtual RenderSurfacePtr CreateRenderSurface(RenderTexture2DPtr const& texture,unsigned int usage) = 0;

		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc) = 0;
		virtual DepthStencilStatePtr CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef) = 0;
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false) = 0;

		void RegisterPropertyManager(std::string const& name);
		ShaderPropertyManagerPtr GetPropertyManager(std::string const& name);

	public:
		static RasterizeStatePtr DefaultRasterizeState;
		static BlendStatePtr DefaultBlentState;
		static DepthStencilStatePtr DefaultDepthStencilState;


	protected:
		ShaderMap _shaderMap;
		ShaderPropertyManagerMap _propertyManagerMap;
 
	};
}


#endif