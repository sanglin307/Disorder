#ifndef _DISORDER_RENDERRESOURCEMANAGER_H_
#define _DISORDER_RENDERRESOURCEMANAGER_H_


namespace Disorder
{
	class RenderResourceManager 
	{
		typedef boost::unordered_map<std::string,RenderEffectPtr>  EffectMap;
		typedef boost::unordered_map<std::string,RenderLayoutPtr>  RenderLayoutMap;
      
	public:

		virtual RenderEffectPtr CreateRenderEffect(std::string const& fileName, ShaderModel shaderModel,std::string const& entryPointVS,std::string const& entryPointPS) = 0;
		virtual RenderLayoutPtr CreateRenderLayout(ShaderObjectPtr const& vertexShader,const VertexInputDes* pVertexInputDes,unsigned int vertexInputDesLength,TopologyType topologyType) = 0;
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int elementSize,unsigned int bufferSize,unsigned int accessHint,BufferInitData const* pData) = 0;
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int hight,bool bMipmap,BufferInitData const* pData) = 0;
		virtual SamplerStatePtr CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16) = 0;
		virtual RenderViewPtr CreateTexture2DView(RenderTexture2DPtr const& textureResource) = 0;
		virtual RenderViewPtr CreateTexture2DViewFromFile(std::string const& fileName) = 0;
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc) = 0;
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false) = 0;
	protected:
		EffectMap _effectMap;
		RenderLayoutMap _renderLayoutMap;
       

	};
}


#endif