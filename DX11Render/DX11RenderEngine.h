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
		virtual void Draw(std::list<RendererPtr> const& renderList);

		virtual void OnDrawBegin();
		virtual void OnDrawEnd();

		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout);
		virtual void SetBlendState(BlendStatePtr const& blendState);
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState);

		//virtual void UpdateMVPMatrix(RenderEffectPtr const& technique, Matrix4 const& worldMatrix,Matrix4 const& viewMatrix,Matrix4 const& projMatrix);
		virtual void SetEffect(RenderEffectPtr const& technique);
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation);

		virtual void* Map(RenderBufferPtr const& buffer,BufferAccess bufferAccess);
		virtual void UnMap(RenderBufferPtr const& buffer);
		virtual void UpdateSubresource(RenderBufferPtr const& buffer,void* pSrcData,unsigned int srcDataSize);

		void PrepareRenderParam(RenderEffectPtr const& technique);

		virtual RenderTargetPtr CreateRenderTarget(RenderConfig const& renderConfig,void *hWnd);

		ID3D11DevicePtr const & DX11RenderEngine::D3DDevice() const
		{
			return _pd3dDevice;
		};

		ID3D11DeviceContextPtr const & DX11RenderEngine::D3DImmediateContext() const
		{
			return _pImmediateContext;
		};

	private:

		D3D_DRIVER_TYPE                         _driverType;
		D3D_FEATURE_LEVEL                       _featureLevel;
		ID3D11DevicePtr                         _pd3dDevice;
		ID3D11DeviceContextPtr                  _pImmediateContext;
		IDXGISwapChainPtr                       _pSwapChain;

		DX11RenderTargetPtr                     _pRenderTarget;

	 

	};
}






#endif