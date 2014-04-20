#ifndef _DISORDER_DX11RENDERENGINE_H_
#define _DISORDER_DX11RENDERENGINE_H_


namespace Disorder
{
	class DX11RenderEngine : public RenderEngine
	{
	public:
		
		static DX11RenderEnginePtr Create();

		virtual void Init();
		virtual void Exit();
		
		virtual void OnDrawBegin();
		virtual void OnDrawEnd();

		virtual void ClearRenderSurface(const RenderSurfacePtr& renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil);
 
		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout);
	
		virtual void SetRenderTarget(const RenderSurfacePtr& renderTarget,bool useReadOnlyDepthStencil = false);
		virtual void SetPrimitiveTopology(TopologyType topologyType);

		virtual void SetEffect(RenderEffectPtr const& technique);
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation);
		virtual void Draw(unsigned int vertexCount,unsigned int startVertexLocation);

		virtual void* Map(RenderBufferPtr const& buffer,BufferAccess bufferAccess);
		virtual void UnMap(RenderBufferPtr const& buffer);
		virtual void UpdateSubresource(RenderBufferPtr const& buffer,void* pSrcData,unsigned int srcDataSize);
 
		virtual void CreateViewport(void *hWnd);

		ID3D11DevicePtr const & DX11RenderEngine::D3DDevice() const
		{
			return _pd3dDevice;
		};

		ID3D11DeviceContextPtr const & DX11RenderEngine::D3DImmediateContext() const
		{
			return _pImmediateContext;
		};

		D3D_FEATURE_LEVEL GetFeatureLevel()
		{
			return _featureLevel;
		}

		virtual void SaveSurfaceView(SurfaceViewPtr const& surface, std::string const& fileName);

		// pixel format related things.
		static DXGI_FORMAT GetPixelFormat(PixelFormat format);
		static PixelFormat GetBasePixelFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetDepthShaderResourceFormat(DXGI_FORMAT format);
        static DXGI_FORMAT GetDepthTextureFormat(DXGI_FORMAT format);
		
		static D3D_PRIMITIVE_TOPOLOGY GetPlatformTopology(TopologyType tType);
		static D3D11_BLEND_OP GetD3DBlendOp(BlendOperation blendOp);
		static D3D11_BLEND GetD3DBlendDesc(BlendOptions blendOptions);
		static D3D11_COMPARISON_FUNC GetD3DComparisonFunc(ComparisonFunc func);
		static D3D11_STENCIL_OP GetD3DStencilOp(StencilOperation op);
		static D3D11_FILTER GetD3DFilter(SamplerFilter filter);
		static D3D11_TEXTURE_ADDRESS_MODE GetD3DAddressMode(TextureAddressMode addrMode);

	protected:

		void EnumAdapters();
		void CreateDevice();

		virtual void SetBlendState(BlendStatePtr const& blendState);
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState);
		virtual void SetDepthStencilState(DepthStencilStatePtr const& depthStencilState);
		virtual void ClearRenderTarget(const SurfaceViewPtr& renderTarget, const glm::vec4& color);
		virtual void ClearDepthStencil(const SurfaceViewPtr& depthBuffer, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil);

	private:
		DX11RenderEngine();

		D3D_DRIVER_TYPE                         _driverType;
		D3D_FEATURE_LEVEL                       _featureLevel;

		IDXGIFactoryPtr                         _pDXGIFactory;
		std::vector<IDXGIAdapterPtr>            _vDXGIAdapter;

		ID3D11DevicePtr                         _pd3dDevice;
		ID3D11DeviceContextPtr                  _pImmediateContext;
		IDXGISwapChainPtr                       _pSwapChain;
 
	 

	};
}






#endif