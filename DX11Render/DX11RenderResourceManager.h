#ifndef _DISORDER_DX11RENDERRESOURCEMANAGER_H_
#define _DISORDER_DX11RENDERRESOURCEMANAGER_H_

namespace Disorder
{
	class DX11RenderResourceManager : public RenderResourceManager
	{
	public :
		virtual RenderLayoutPtr CreateRenderLayout(RenderEffectPtr const& renderEffect,TopologyType topologyType,bool soloBuffer);
		virtual ShaderObjectPtr CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint);
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData);

		virtual void CreateRenderBufferArray(GeometryPtr const& data, BufferUsage bufferUsage,RenderEffectPtr const& renderEffect,std::vector<RenderBufferPtr> & bufferArray );
		
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int hight,bool bMipmap,unsigned int usage,BufferInitData const* pData);
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,ImagePtr image);
       
		virtual RenderEffectPtr CreateRenderEffect();
		virtual RenderSurfacePtr CreateRenderSurface(RenderTexture2DPtr const& texture,unsigned int usage,PixelFormat RenderTargetFormat,PixelFormat DepthFormat,PixelFormat ShaderResFormat,bool ReadOnlyDepth = false,bool ReadOnlyStencil = false);

		virtual SamplerStatePtr CreateSamplerState(SamplerDesc* pSamplerDesc);
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc);
		virtual DepthStencilStatePtr CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef);
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);

		virtual void RegisterPropertyManager(std::string const& name);
		static DX11RenderResourceManagerPtr Create();

	private:
		DX11RenderResourceManager(){}

	
	};
}

#endif