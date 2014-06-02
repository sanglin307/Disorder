#ifndef _DISORDER_DX11RENDERRESOURCEMANAGER_H_
#define _DISORDER_DX11RENDERRESOURCEMANAGER_H_

namespace Disorder
{
	class DX11RenderResourceManager : public RenderResourceManager
	{
		typedef boost::unordered_map<std::wstring, ID3D11InputLayoutPtr> RenderLayoutMap;

	public :
		virtual RenderLayoutPtr CreateRenderLayout(RenderEffectPtr const& renderEffect,TopologyType topologyType,bool soloBuffer);
		virtual ShaderObjectPtr CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint);
		virtual RenderBufferPtr CreateBuffer(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint = 0);

		virtual void CreateBufferArray(GeometryPtr const& data, BufferUsage bufferUsage,RenderEffectPtr const& renderEffect,std::vector<RenderBufferPtr> & bufferArray );
		
		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample,unsigned int viewFlag,int arraySize, BufferInitData const* pData);
		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample,ImagePtr image);
		virtual RenderTexture2DPtr CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<ImagePtr>& image);

		virtual RenderEffectPtr CreateRenderEffect();
		virtual SurfaceViewPtr CreateSurfaceView(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat Format, unsigned int Flag = 0);
		virtual RenderSurfacePtr CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap);

		virtual SamplerStatePtr CreateSamplerState(SamplerDesc* pSamplerDesc);
		virtual RasterizeStatePtr CreateRasterizeState(RasterizeDesc *pDesc);
		virtual DepthStencilStatePtr CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef);
		virtual BlendStatePtr CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);

		virtual void RegisterPropertyManager(std::string const& name);
		static DX11RenderResourceManagerPtr Create();

		void UpdateRenderLayout(const std::wstring& hashKey, ID3D11InputLayoutPtr layout);
		ID3D11InputLayoutPtr GetRenderLayout(const std::wstring& hashKey);

	private:
		DX11RenderResourceManager(){}
		RenderLayoutMap _renderLayoutMap;
	
	};
}

#endif