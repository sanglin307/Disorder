#ifndef _DISORDER_GLRENDERRESOURCEMANAGER_H_
#define _DISORDER_GLRENDERRESOURCEMANAGER_H_


namespace Disorder
{
	class GLRenderResourceManager : public RenderResourceManager
	{
	public:
		virtual RenderLayoutPtr CreateRenderLayout(ShaderObjectPtr const& vertexShader, TopologyType topologyType, bool soloBuffer);
		virtual ShaderObjectPtr CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel, std::string const& entryPoint);
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type, unsigned int accessHint, GeometryPtr const& data, std::string const& sematic, ShaderObjectPtr const& vertexShader);
		virtual RenderBufferPtr CreateRenderBuffer(RenderBufferType type, unsigned int accessHint, unsigned int elementSize, unsigned int size, void *pData);

		virtual void CreateRenderBufferArray(GeometryPtr const& data, unsigned int accessHint, ShaderObjectPtr const& vertexShader, std::vector<RenderBufferPtr> & bufferArray);

		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, unsigned int usage, BufferInitData const* pData);
		virtual RenderTexture2DPtr CreateRenderTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, ImagePtr image);

		virtual RenderSurfacePtr CreateRenderSurface(RenderTexture2DPtr const& texture, unsigned int usage, PixelFormat RenderTargetFormat, PixelFormat DepthFormat, PixelFormat ShaderResFormat, bool ReadOnlyDepth = false, bool ReadOnlyStencil = false);

		virtual SamplerStatePtr CreateSamplerState(SamplerFilter filter, TextureAddressMode addressUVW, UINT maxAnisotropy = 16);
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc);
		virtual DepthStencilStatePtr CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef);
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable = false, bool IndependentBlendEnable = false);

		virtual void RegisterPropertyManager(std::string const& name);
		static GLRenderResourceManagerPtr Create();

	private:
		GLRenderResourceManager(){}


	};
}




#endif