#ifndef _DISORDER_DX11RENDERRESOURCEMANAGER_H_
#define _DISORDER_DX11RENDERRESOURCEMANAGER_H_

namespace Disorder
{
	class DX11RenderResourceManager : public RenderResourceManager
	{
	public :
		virtual RenderLayout* CreateRenderLayout(RenderEffect* renderEffect, TopologyType topologyType, bool soloBuffer);
		virtual ShaderObject* CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint);
		virtual RenderBuffer* CreateBuffer(const std::string& bufferName, RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint = 0);

		virtual void CreateBufferArray(const std::string& bufferName, Geometry* data, BufferUsage bufferUsage, RenderEffect* renderEffect, std::vector<RenderBuffer*> & bufferArray);
		
		virtual RenderTexture2D* CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);
		virtual RenderTexture2D* CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, Image* image);
		virtual RenderTexture2D* CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag);

		virtual RenderEffect* CreateRenderEffect();
		virtual SurfaceView* CreateSurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat Format, unsigned int Flag = 0);
		virtual RenderSurface* CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap);

		virtual SamplerState* CreateSamplerState(SamplerDesc* pSamplerDesc);
		virtual RasterizeState* CreateRasterizeState(RasterizeDesc *pDesc);
		virtual DepthStencilState* CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef);
		virtual BlendState* CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);

		virtual void RegisterPropertyManager(std::string const& name);
	
	};
}

#endif