#ifndef _DISORDER_DX11RENDERRESOURCEMANAGER_H_
#define _DISORDER_DX11RENDERRESOURCEMANAGER_H_

namespace Disorder
{
	class DX11RenderResourceManager : public RenderResourceManager
	{
	public :
		virtual RenderEffectPtr CreateRenderEffect(std::string const& fileName, ShaderModel shaderModel,std::string const& entryPointVS,std::string const& entryPointPS);
		virtual RenderLayoutPtr CreateRenderLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType);
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,GeometryPtr const& data,ShaderObjectPtr const& vertexShader = 0);
		virtual RenderBufferPtr CreateConstBuffer(unsigned int size, unsigned int accessHint);
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int hight,bool bMipmap,BufferInitData const* pData);
		virtual RenderViewPtr CreateTexture2DViewFromFile(std::string const& fileName);
		virtual SamplerStatePtr CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16);
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc);
		virtual RenderViewPtr CreateTexture2DView(RenderTexture2DPtr const& textureResource);
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);

	
	};
}

#endif