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
		virtual RenderLayoutPtr CreateRenderLayout(RenderEffectPtr const& renderEffect,TopologyType topologyType,bool soloBuffer) = 0;
		virtual RenderBufferPtr CreateBuffer(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData) = 0;
		virtual void CreateBufferArray(GeometryPtr const& data, BufferUsage bufferUsage,RenderEffectPtr const& renderEffect,std::vector<RenderBufferPtr> & bufferArray ) = 0;
		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample,const std::vector<ESurfaceLocation>& location, BufferInitData const* pData) = 0;
		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,bool bMultiSample,ImagePtr image) = 0;
		virtual SamplerStatePtr CreateSamplerState(SamplerDesc* pSamplerDesc) = 0;
	
		virtual RenderSurfacePtr CreateShaderResource(RenderTexturePtr const& texture,PixelFormat format) = 0;
		virtual RenderSurfacePtr CreateRenderSurface(const std::vector<sRenderSurfaceDes>& surfaceDes) = 0;
		virtual RenderEffectPtr CreateRenderEffect() = 0;
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc) = 0;
		virtual DepthStencilStatePtr CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef) = 0;
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false) = 0;

		virtual void RegisterPropertyManager(std::string const& name) = 0;
		ShaderPropertyManagerPtr GetPropertyManager(std::string const& name);

		void DumpContent();
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