#ifndef _DISORDER_RENDERRESOURCEMANAGER_H_
#define _DISORDER_RENDERRESOURCEMANAGER_H_


namespace Disorder
{
	class RenderResourceManager 
	{
		typedef boost::unordered_map<std::string,RenderEffectPtr>  EffectMap;
		 
	public:
		virtual void Init();
		virtual RenderEffectPtr CreateRenderEffect(std::string const& fileName, ShaderModel shaderModel,std::string const& entryPointVS,std::string const& entryPointPS ) = 0;
		virtual RenderLayoutPtr CreateRenderLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer) = 0;
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,GeometryPtr const& data,ShaderObjectPtr const& vertexShader) = 0;
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData) = 0;
		virtual void CreateVertexBufferArray(GeometryPtr const& data, unsigned int accessHint,ShaderObjectPtr const& vertexShader,std::vector<RenderBufferPtr> & bufferArray ) = 0;
		virtual RenderBufferPtr CreateConstBuffer(unsigned int size, unsigned int accessHint) = 0;
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int hight,bool bMipmap,BufferInitData const* pData) = 0;
		virtual SamplerStatePtr CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16) = 0;
		virtual RenderViewPtr CreateTexture2DView(RenderTexture2DPtr const& textureResource) = 0;
		virtual RenderViewPtr CreateTexture2DViewFromFile(std::string const& fileName) = 0;
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc) = 0;
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false) = 0;

	public:
		static RasterizeStatePtr DefaultRasterizeState;
		static BlendStatePtr DefaultBlentState;


	protected:
		EffectMap _effectMap;
 
	};
}


#endif