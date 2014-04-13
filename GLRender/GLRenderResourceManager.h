#ifndef _DISORDER_GLRENDERRESOURCEMANAGER_H_
#define _DISORDER_GLRENDERRESOURCEMANAGER_H_


namespace Disorder
{
	class GLRenderResourceManager : public RenderResourceManager
	{
	public:
		virtual RenderLayoutPtr CreateRenderLayout(RenderEffectPtr const& renderEffect, TopologyType topologyType, bool soloBuffer);
		virtual ShaderObjectPtr CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel, std::string const& entryPoint);
		virtual RenderBufferPtr CreateBuffer(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData);

		virtual void CreateBufferArray(GeometryPtr const& data, BufferUsage bufferUsage, RenderEffectPtr const& renderEffect, std::vector<RenderBufferPtr> & bufferArray);

		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample, const std::vector<ESurfaceLocation>& location, BufferInitData const* pData);
		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample, ImagePtr image);

		virtual RenderSurfacePtr CreateShaderResource(RenderTexturePtr const& texture, PixelFormat format);
		virtual RenderSurfacePtr CreateRenderSurface(const std::vector<sRenderSurfaceDes>& surfaceDes);
		virtual RenderEffectPtr CreateRenderEffect();
		virtual SamplerStatePtr CreateSamplerState(SamplerDesc* pSamplerDesc);
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