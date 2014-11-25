#ifndef _DISORDER_DX11RENDERENGINE_H_
#define _DISORDER_DX11RENDERENGINE_H_


namespace Disorder
{
	class DX11RenderEngine : public RenderEngine
	{
	public:
		DX11RenderEngine();
		virtual void Init();
		virtual void Exit();
		
		virtual void OnFrameBegin();
		virtual void OnFrameEnd();

		virtual void ClearRenderSurface(RenderSurface* renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil, int sliceIndex = -1);
		

		virtual void SetViewport(float width, float height, float minDepth, float maxDepth, float topX, float topY);

		virtual void SetRenderLayout(RenderLayout* renderLayout);
	
		virtual void SetRenderTarget(RenderSurface* renderTarget, int sliceIndex = -1, bool useReadOnlyDepthStencil = false);
	 
		virtual void SetPrimitiveTopology(TopologyType topologyType);

		virtual void SetEffect(RenderEffect* technique);
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation);
		virtual void Draw(unsigned int vertexCount,unsigned int startVertexLocation);

		virtual void* Map(RenderBuffer* buffer,BufferAccess bufferAccess);
		virtual void UnMap(RenderBuffer* buffer);
		virtual void UpdateSubresource(RenderBuffer* buffer,void* pSrcData,unsigned int srcDataSize);
		virtual void CopyTexture2D(RenderTexture2D* srcTexture, RenderTexture2D* dstTexture);
		virtual void CreateViewport(void *hWnd);

		ID3D11Device* DX11RenderEngine::D3DDevice() const
		{
			return _pd3dDevice;
		};

		ID3D11DeviceContext* DX11RenderEngine::D3DImmediateContext() const
		{
			return _pImmediateContext;
		};

		D3D_FEATURE_LEVEL GetFeatureLevel()
		{
			return _featureLevel;
		}

		virtual void SaveSurfaceView(SurfaceView* surface, std::string const& fileName);

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
		static D3D11_FILTER GetD3DFilter(SamplerFilter filter,bool bCompareTypeFilter = false);
		static D3D11_TEXTURE_ADDRESS_MODE GetD3DAddressMode(TextureAddressMode addrMode);

	protected:

		void EnumAdapters();
		void CreateDevice();

		virtual void SetBlendState(BlendState* blendState);
		virtual void SetRasterizeState(RasterizeState* rasterizeState);
		virtual void SetDepthStencilState(DepthStencilState* depthStencilState);
		
		void ClearRenderTarget(SurfaceView* renderTarget, const glm::vec4& color, int sliceIndex);
		void ClearDepthStencil(SurfaceView* depthBuffer, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil, int sliceIndex);

	private:
		D3D_DRIVER_TYPE                         _driverType;
		D3D_FEATURE_LEVEL                       _featureLevel;

		IDXGIFactory*                         _pDXGIFactory;
		std::vector<IDXGIAdapter*>            _vDXGIAdapter;

		ID3D11Device*                         _pd3dDevice;
		ID3D11DeviceContext*                  _pImmediateContext;
		IDXGISwapChain*                       _pSwapChain;
 
	 

	};
}






#endif