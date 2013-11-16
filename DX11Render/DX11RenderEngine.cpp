#include "DX11RenderInclude.h"
#include "../Engine/Logger.h"

namespace Disorder
{

	 DX11RenderEngine::DX11RenderEngine()
	 {
		GPixelFormats[ PF_Unknown		].PlatformFormat	= DXGI_FORMAT_UNKNOWN;
		GPixelFormats[ PF_R32G32B32A32F	].PlatformFormat	= DXGI_FORMAT_R32G32B32A32_FLOAT;
		GPixelFormats[ PF_R32G32B32F	].PlatformFormat	= DXGI_FORMAT_R32G32B32_FLOAT;
		GPixelFormats[ PF_R8G8B8A8		].PlatformFormat	= DXGI_FORMAT_R8G8B8A8_UNORM;
		GPixelFormats[ PF_R32G32F       ].PlatformFormat    = DXGI_FORMAT_R32G32_FLOAT;
		GPixelFormats[ PF_R8G8          ].PlatformFormat    = DXGI_FORMAT_R8G8_UNORM;
		GPixelFormats[ PF_R10G10B10A2   ].PlatformFormat    = DXGI_FORMAT_R10G10B10A2_UNORM; 
		GPixelFormats[ PF_D24S8 ].PlatformFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;

		ResourceManager = boost::make_shared<DX11RenderResourceManager>();
		ParameterManager = boost::make_shared<MaterialParameterManager>();

		IDXGIFactory * pFactory;
        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory) );
		BOOST_ASSERT(SUCCEEDED(hr));

		_pDXGIFactory = MakeComPtr<IDXGIFactory>(pFactory); 

		EnumAdapters();

		CreateDevice();
	 }

	 void DX11RenderEngine::EnumAdapters()
	 {
		 BOOST_ASSERT(_pDXGIFactory != NULL );

		 _vDXGIAdapter.clear();
		 IDXGIAdapter * pAdapter; 
		 
		 UINT i =0;
		 while(_pDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND) 
		 { 
			 _vDXGIAdapter.push_back(MakeComPtr<IDXGIAdapter>(pAdapter));
			 i++;
		 } 
	 }
 
	 void DX11RenderEngine::CreateDevice()
	 {
		 BOOST_ASSERT(_vDXGIAdapter.size() > 0);
		 UINT DeviceCreationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		 D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_UNKNOWN;//D3D_DRIVER_TYPE_HARDWARE;
 
	 #ifdef _DEBUG
		 DeviceCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	 #endif
  
		 D3D_FEATURE_LEVEL FeatureLevels[] =
		 {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		 };

		 UINT numFeatureLevels = ARRAYSIZE( FeatureLevels );
		 ID3D11Device*         pd3dDevice;
		 ID3D11DeviceContext*  pImmediateContext;
 
		 HRESULT Hr = D3D11CreateDevice( _vDXGIAdapter[0].get(), DriverType, NULL, DeviceCreationFlags, FeatureLevels, numFeatureLevels, D3D11_SDK_VERSION, &pd3dDevice, &_featureLevel, &pImmediateContext );
		 BOOST_ASSERT( SUCCEEDED(Hr) );

 
		 _pd3dDevice = MakeComPtr<ID3D11Device>(pd3dDevice);
		 _pImmediateContext = MakeComPtr<ID3D11DeviceContext>(pImmediateContext);
		 
	 }

	 void DX11RenderEngine::CreateViewport(void *hWnd)
	{
		HRESULT hr = S_OK;
	
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );

		sd.BufferCount = 1;
		sd.BufferDesc.Width = GConfig->pRenderConfig->SizeX;
		sd.BufferDesc.Height = GConfig->pRenderConfig->SizeY;
		sd.BufferDesc.Format = (DXGI_FORMAT)GPixelFormats[GConfig->pRenderConfig->ColorFormat].PlatformFormat; 
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = (HWND)hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !GConfig->pRenderConfig->FullScreen;

		
		IDXGISwapChain*       pSwapChain;
		hr = _pDXGIFactory->CreateSwapChain(_pd3dDevice.get(),&sd,&pSwapChain);
		BOOST_ASSERT(SUCCEEDED(hr));
		_pSwapChain = MakeComPtr<IDXGISwapChain>(pSwapChain);

		hr = _pDXGIFactory->MakeWindowAssociation((HWND)hWnd,DXGI_MWA_NO_WINDOW_CHANGES);
		BOOST_ASSERT(SUCCEEDED(hr));

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = _pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		BOOST_ASSERT(SUCCEEDED(hr));
		DX11RenderTexture2DPtr BackBufferTex = boost::make_shared<DX11RenderTexture2D>(GConfig->pRenderConfig->ColorFormat,sd.BufferDesc.Width,sd.BufferDesc.Height,MakeComPtr<ID3D11Texture2D>(pBackBuffer));
 
		ID3D11RenderTargetView* pRenderTargetView = NULL;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	    RTVDesc.Format = (DXGI_FORMAT)GPixelFormats[GConfig->pRenderConfig->ColorFormat].PlatformFormat;
	    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	    RTVDesc.Texture2D.MipSlice = 0;
		hr = _pd3dDevice->CreateRenderTargetView( pBackBuffer, &RTVDesc, &pRenderTargetView );
		BOOST_ASSERT(SUCCEEDED(hr));

		ID3D11ShaderResourceView* pShaderResourceView = NULL;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = (DXGI_FORMAT)GPixelFormats[GConfig->pRenderConfig->ColorFormat].PlatformFormat;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;
		hr = _pd3dDevice->CreateShaderResourceView( pBackBuffer,&SRVDesc,&pShaderResourceView );
		BOOST_ASSERT(SUCCEEDED(hr));

		DX11RenderSurfacePtr RenderTarget = boost::make_shared<DX11RenderSurface>();
		RenderTarget->Tex2DResource = BackBufferTex;
		RenderTarget->RenderTargetView = MakeComPtr<ID3D11RenderTargetView>(pRenderTargetView);
		RenderTarget->ShaderResourceView = MakeComPtr<ID3D11ShaderResourceView>(pShaderResourceView);
		GRenderSurface.RenderTarget = RenderTarget;

		//Create a stencil & depth buffer.
		ID3D11Texture2D* pDepthStencil = NULL;
		ID3D11DepthStencilView* pDepthStencilView = NULL;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = GConfig->pRenderConfig->SizeX;
		descDepth.Height = GConfig->pRenderConfig->SizeY;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = (DXGI_FORMAT)GPixelFormats[GConfig->pRenderConfig->DepthStencilFormat].PlatformFormat;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = _pd3dDevice->CreateTexture2D( &descDepth, NULL, &pDepthStencil );
		DX11RenderTexture2DPtr DepthBufferTex = boost::make_shared<DX11RenderTexture2D>(GConfig->pRenderConfig->DepthStencilFormat,descDepth.Width,descDepth.Height,MakeComPtr<ID3D11Texture2D>(pDepthStencil));
 
		BOOST_ASSERT(SUCCEEDED(hr));

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = _pd3dDevice->CreateDepthStencilView( pDepthStencil, &descDSV, &pDepthStencilView );
		BOOST_ASSERT(SUCCEEDED(hr));

		DX11RenderSurfacePtr DepthStencilBuffer = boost::make_shared<DX11RenderSurface>();
		DepthStencilBuffer->Tex2DResource = DepthBufferTex;
		DepthStencilBuffer->DepthStencilView = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);
		GRenderSurface.DepthStencilBuffer = DepthStencilBuffer;
 
		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)GConfig->pRenderConfig->SizeX;
		vp.Height = (FLOAT)GConfig->pRenderConfig->SizeY;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_pImmediateContext->RSSetViewports( 1, &vp );

	}



	void DX11RenderEngine::Init()
	{
	}

	void DX11RenderEngine::Exit()
	{
		 if( _pImmediateContext ) 
		 {
			 _pImmediateContext->ClearState();
		 }

		 _pImmediateContext.reset();
		 _pSwapChain.reset();
		 _pd3dDevice.reset();
	}

   	void* DX11RenderEngine::Map(RenderBufferPtr const& buffer,BufferAccess bufferAccess)
	{
		D3D11_MAP flag = D3D11_MAP_WRITE;
		if( bufferAccess == BA_Read_Only )
			flag = D3D11_MAP_READ;
		else if( bufferAccess == BA_Write_Only)
			flag = D3D11_MAP_WRITE_DISCARD;
		else if( bufferAccess == BA_Read_Write)
			flag = D3D11_MAP_READ_WRITE;

		D3D11_MAPPED_SUBRESOURCE pMappedResource;
		HRESULT hr = _pImmediateContext->Map((ID3D11Buffer*)(buffer->GetLowInterface()),D3D11CalcSubresource(0, 0, 1),flag,0,&pMappedResource);

		return pMappedResource.pData;
	}
	
	void DX11RenderEngine::UnMap(RenderBufferPtr const& buffer)
	{
		_pImmediateContext->Unmap((ID3D11Buffer*)(buffer->GetLowInterface()),D3D11CalcSubresource(0, 0, 1));
	}

	void DX11RenderEngine::SetBlendState(BlendStatePtr const& blendState)
	{ 
		if( CachedBlendState != blendState )
		{
			CachedBlendState = blendState;
		    ID3D11BlendState *pState = (ID3D11BlendState*)(blendState->GetLowInterface());

			D3D11_BLEND_DESC desc;
			pState->GetDesc(&desc);

		    _pImmediateContext->OMSetBlendState(pState,blendState->BlendFactor,blendState->SampleMask);	 
		
		}
	}

    void DX11RenderEngine::SetRasterizeState(RasterizeStatePtr const& rasterizeState)
	{
		if( CachedRasterizeState != rasterizeState )
		{
			CachedRasterizeState = rasterizeState;
		    ID3D11RasterizerState* pState = (ID3D11RasterizerState*)(rasterizeState->GetLowInterface());
		    _pImmediateContext->RSSetState(pState);
		}
	 
	}

	void DX11RenderEngine::SetRenderLayout( RenderLayoutPtr const& renderLayout)
	{

		DX11RenderLayoutPtr dxRenderLayout = boost::dynamic_pointer_cast<DX11RenderLayout>(renderLayout);

		TopologyType topology = renderLayout->GetTopology();
		if(topology == TT_TriangleList )
			_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		else
			BOOST_ASSERT(0);

		//set layout
		_pImmediateContext->IASetInputLayout((ID3D11InputLayout*)(renderLayout->GetLowInterface()));
		 
		//set vertex buffer		 
		_pImmediateContext->IASetVertexBuffers(0,dxRenderLayout->VertexArray.size(),&(dxRenderLayout->VertexArray[0]),&(dxRenderLayout->VertexArrayElementSize[0]),&(dxRenderLayout->VertexArrayOffset[0]));
 
		RenderBufferPtr indexBuffer = renderLayout->GetIndexBuffer();
		if(indexBuffer != NULL )
		{
			ID3D11Buffer *pBuffer = (ID3D11Buffer *)(indexBuffer->GetLowInterface());
			unsigned int size = indexBuffer->GetElementSize() ;
			if( size == 2)
			    _pImmediateContext->IASetIndexBuffer(pBuffer,DXGI_FORMAT_R16_UINT,0);
			else if( size == 4)
				 _pImmediateContext->IASetIndexBuffer(pBuffer,DXGI_FORMAT_R32_UINT,0);
		}
 
 
	}

	void DX11RenderEngine::UpdateSubresource(RenderBufferPtr const& buffer,void* pSrcData,unsigned int srcDataSize)
	{
		ID3D11Buffer *pBuffer = (ID3D11Buffer *)(buffer->GetLowInterface());
		_pImmediateContext->UpdateSubresource(pBuffer, 0, NULL, pSrcData, srcDataSize, 0);
	}


	void DX11RenderEngine::DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation)
	{
		GDrawTriNumber += indexCount / 3;

		_pImmediateContext->DrawIndexed(indexCount,startIndexLocation,baseVertexLocation);
	}
 
	void DX11RenderEngine::SetEffect(RenderEffectPtr const& effect)
	{

		// render State
        SetBlendState(effect->GetBlendState());
		SetRasterizeState(effect->GetRasterizeState());
		SetDepthStencilState(effect->GetDepthStencilState());

		effect->UpdateRenderParam();

		ShaderObjectPtr vertexShader = effect->GetVertexShader();
		if( vertexShader != NULL )
		{
			ID3D11VertexShader *pShader = (ID3D11VertexShader *)(vertexShader->GetLowInterface());
			_pImmediateContext->VSSetShader(pShader, NULL,0);

			DX11ShaderObjectPtr dxVertexShader = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader);
			std::size_t cbsize = dxVertexShader->CachedConstBuffer.size();
			if( cbsize > 0 )
			{
				_pImmediateContext->VSSetConstantBuffers(0,cbsize,&(dxVertexShader->CachedConstBuffer[0]));
			}
			else
			{
				_pImmediateContext->VSSetConstantBuffers(0,0,0);
			}


			std::size_t sssize = dxVertexShader->CachedSamplerState.size();
			if( sssize > 0 )
			{
				_pImmediateContext->VSSetSamplers(0,sssize,&(dxVertexShader->CachedSamplerState[0]));	 
			}
			else
			{
				_pImmediateContext->VSSetSamplers(0,0,0);
			}


			std::size_t srsize = dxVertexShader->CachedShaderResourceView.size();
			if( srsize > 0 )
			{
				_pImmediateContext->VSSetShaderResources(0,srsize,&(dxVertexShader->CachedShaderResourceView[0]));	 
			}
			else
			{
				_pImmediateContext->VSSetShaderResources(0,0,0);
			}

		}

		ShaderObjectPtr pixelShader = effect->GetPixelShader();
		if( pixelShader != NULL )
		{
			ID3D11PixelShader *pShader = (ID3D11PixelShader *)(pixelShader->GetLowInterface());
			_pImmediateContext->PSSetShader(pShader,NULL,0);

			DX11ShaderObjectPtr dxPixelShader = boost::dynamic_pointer_cast<DX11ShaderObject>(pixelShader);

			std::size_t csize = dxPixelShader->CachedConstBuffer.size();
			if( csize > 0 )
			{
				_pImmediateContext->PSSetConstantBuffers(0,csize,&(dxPixelShader->CachedConstBuffer[0]));			 
			}
			else
			{		 
				_pImmediateContext->PSSetConstantBuffers(0,0,0);
			}

			std::size_t sssize = dxPixelShader->CachedSamplerState.size();
			if( sssize > 0 )
			{
				_pImmediateContext->PSSetSamplers(0,sssize,&(dxPixelShader->CachedSamplerState[0]));		 
			}
			else
			{
				_pImmediateContext->PSSetSamplers(0,0,0);
			}

			std::size_t srsize = dxPixelShader->CachedShaderResourceView.size();
			if( srsize > 0 )
			{
				_pImmediateContext->PSSetShaderResources(0,srsize,&(dxPixelShader->CachedShaderResourceView[0]));				 
			}
			else
			{
				_pImmediateContext->PSSetShaderResources(0,0,0);
			}
		}
	}

	void DX11RenderEngine::SetDepthStencilState(DepthStencilStatePtr const& depthStencilState)
	{
		if( CachedDepthStencilState != depthStencilState )
		{
			CachedDepthStencilState = depthStencilState;
			ID3D11DepthStencilState* pState = (ID3D11DepthStencilState*)(depthStencilState->GetLowInterface());
			_pImmediateContext->OMSetDepthStencilState(pState,depthStencilState->StencilRef);
		}
	}

	void DX11RenderEngine::SetRenderTarget(const RenderSurfacePtr& renderTarget,const RenderSurfacePtr& depthStencil)
	{
		DX11RenderSurfacePtr dxRenderTarget = renderTarget == NULL ? NULL : boost::dynamic_pointer_cast<DX11RenderSurface>(renderTarget);
		DX11RenderSurfacePtr dxDepthStencil = depthStencil == NULL ? NULL : boost::dynamic_pointer_cast<DX11RenderSurface>(depthStencil);

		ID3D11RenderTargetView* pView = dxRenderTarget->RenderTargetView.get();
		_pImmediateContext->OMSetRenderTargets(1,&pView,dxDepthStencil->DepthStencilView.get());
	}

	void DX11RenderEngine::ClearRenderTarget(const RenderSurfacePtr& renderTarget,const Vector4& color )
	{
		DX11RenderSurfacePtr dxRenderTarget = boost::dynamic_pointer_cast<DX11RenderSurface>(renderTarget);
		_pImmediateContext->ClearRenderTargetView(dxRenderTarget->RenderTargetView.get(), color.Ptr() );
	}

	void DX11RenderEngine::ClearDepthStencil(const RenderSurfacePtr& depthBuffer,bool bClearDepth,float depth,bool bClearStencil,unsigned char stencil)
	{
		DX11RenderSurfacePtr dxDepthBuffer = boost::dynamic_pointer_cast<DX11RenderSurface>(depthBuffer);
		unsigned int flag = 0;
		if( bClearDepth )
			flag |= D3D11_CLEAR_DEPTH;

		if( bClearStencil )
			flag |= D3D11_CLEAR_STENCIL;

		_pImmediateContext->ClearDepthStencilView(dxDepthBuffer->DepthStencilView.get(), flag,depth, stencil );
	}

	void DX11RenderEngine::OnDrawBegin()
	{
		GDrawTriNumber = 0;

		// Clear the back buffer 
 
		
	}

    void DX11RenderEngine::OnDrawEnd()
	{
		_pSwapChain->Present( 0, 0 );
	}


	

 

}