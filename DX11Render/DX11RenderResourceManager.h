#ifndef _DISORDER_DX11RENDERRESOURCEMANAGER_H_
#define _DISORDER_DX11RENDERRESOURCEMANAGER_H_

namespace Disorder
{
	class DX11RenderResourceManager : public RenderResourceManager
	{
	public :
		virtual RenderEffectPtr CreateRenderEffect(std::string const& fileName, ShaderModel shaderModel,std::string const& entryPointVS,std::string const& entryPointPS );
		virtual RenderLayoutPtr CreateRenderLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer);
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,GeometryPtr const& data,ShaderObjectPtr const& vertexShader);
		virtual void CreateVertexBufferArray(GeometryPtr const& data, unsigned int accessHint,ShaderObjectPtr const& vertexShader,std::vector<RenderBufferPtr> & bufferArray );
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData);
		virtual RenderBufferPtr CreateConstBuffer(unsigned int size, unsigned int accessHint);
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int hight,bool bMipmap,BufferInitData const* pData);
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,ImagePtr image);
       
		virtual RenderSurfacePtr CreateRenderSurface(RenderTexture2DPtr const& texture,unsigned int usage);

		virtual SamplerStatePtr CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16);
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc);
		virtual DepthStencilStatePtr CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef);
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);

	
	};
}

#endif