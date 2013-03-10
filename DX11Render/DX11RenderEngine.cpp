#include "DX11RenderInclude.h"

namespace Disorder
{
 
	 RenderEnginePtr CreateDX11RenderEngine()
	 {
		 return boost::make_shared<DX11RenderEngine>();
	 }

	 DX11RenderEngine::DX11RenderEngine()
	 {
		GPixelFormats[ PF_Unknown		].PlatformFormat	= DXGI_FORMAT_UNKNOWN;
		GPixelFormats[ PF_R32G32B32A32F	].PlatformFormat	= DXGI_FORMAT_R32G32B32A32_FLOAT;
		GPixelFormats[ PF_R32G32B32F	].PlatformFormat	= DXGI_FORMAT_R32G32B32_FLOAT;
		GPixelFormats[ PF_R8G8B8A8		].PlatformFormat	= DXGI_FORMAT_R8G8B8A8_UNORM;
		GPixelFormats[ PF_R32G32F       ].PlatformFormat    = DXGI_FORMAT_R32G32_FLOAT;
		GPixelFormats[ PF_R8G8          ].PlatformFormat    =  DXGI_FORMAT_R8G8_UNORM;
		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;

		ResourceManager = boost::make_shared<DX11RenderResourceManager>();
		 
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

		 _pRenderTarget.reset();
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
		_pImmediateContext->Map((ID3D11Buffer*)(buffer->GetLowInterface()),D3D11CalcSubresource(0, 0, 1),flag,0,&pMappedResource);

		return pMappedResource.pData;
	}
	
	void DX11RenderEngine::UnMap(RenderBufferPtr const& buffer)
	{
		_pImmediateContext->Unmap((ID3D11Buffer*)(buffer->GetLowInterface()),D3D11CalcSubresource(0, 0, 1));
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
			_pImmediateContext->IASetIndexBuffer(pBuffer,DXGI_FORMAT_R16_UINT,0);
		}

		RasterizeStatePtr rasterizeState = renderLayout->GetRasterizeState();
		if( rasterizeState != NULL )
		{
			ID3D11RasterizerState* pState = (ID3D11RasterizerState*)(rasterizeState->GetLowInterface());
			_pImmediateContext->RSSetState(pState);
		}

		BlendStatePtr blendState = renderLayout->GetBlendState();
		if( blendState != NULL )
		{
			ID3D11BlendState *pState = (ID3D11BlendState*)(blendState->GetLowInterface());
			_pImmediateContext->OMSetBlendState(pState,blendState->BlendFactor,blendState->SampleMask);
		}
 
	}

	void DX11RenderEngine::UpdateMVPMatrix(RenderTechniquePtr const& technique, Matrix4 const& worldMatrix,Matrix4 const& viewMatrix,Matrix4 const& projMatrix)
	{
		VertexShaderPtr shader = technique->GetVertexShader();
		std::vector<RenderBufferPtr> constBuffers = shader->GetConstBuffers();
		if( constBuffers.size() > 0 )
		{
			ID3D11Buffer *pBuffer = (ID3D11Buffer *)(constBuffers[0]->GetLowInterface());
			typedef struct _sMatrixs
			{
				Matrix4 worldMatrix;
				Matrix4 viewMatrix;
				Matrix4 projMatrix;
			} sMatrix;
			sMatrix matrixBuffer;
			matrixBuffer.worldMatrix = worldMatrix.transpose();
			matrixBuffer.viewMatrix = viewMatrix;
			matrixBuffer.projMatrix = projMatrix;
			_pImmediateContext->UpdateSubresource(pBuffer, 0, NULL, &matrixBuffer, 0, 0);
			 
		}
	}

	void DX11RenderEngine::DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation)
	{
		GDrawTriNumber += indexCount / 3;

		_pImmediateContext->DrawIndexed(indexCount,startIndexLocation,baseVertexLocation);
	}

	void DX11RenderEngine::SetFX(RenderTechniquePtr const& technique)
	{
		VertexShaderPtr vertexShader = technique->GetVertexShader();
		if( vertexShader != NULL )
		{
			ID3D11VertexShader *pShader = (ID3D11VertexShader *)(vertexShader->GetLowInterface());
			_pImmediateContext->VSSetShader(pShader, NULL,0);

			DX11VertexShaderPtr dxVertexShader = boost::dynamic_pointer_cast<DX11VertexShader>(vertexShader);
            std::size_t cbsize = dxVertexShader->ConstBufferArray.size();
			if( cbsize > 0 )
			{
				_pImmediateContext->VSSetConstantBuffers(0,cbsize,&(dxVertexShader->ConstBufferArray[0]));
			}

			std::size_t sssize = dxVertexShader->SamplerStateArray.size();
			if( sssize > 0 )
			{
				_pImmediateContext->VSSetSamplers(0,sssize,&(dxVertexShader->SamplerStateArray[0]));	 
			}

			std::size_t srsize = dxVertexShader->ShaderResourceViewArray.size();
			if( srsize > 0 )
			{
				_pImmediateContext->VSSetShaderResources(0,srsize,&(dxVertexShader->ShaderResourceViewArray[0]));	 
			}
		}

		PixelShaderPtr pixelShader = technique->GetPixelShader();
		if( pixelShader != NULL )
		{
			ID3D11PixelShader *pShader = (ID3D11PixelShader *)(pixelShader->GetLowInterface());
			_pImmediateContext->PSSetShader(pShader,NULL,0);

			DX11PixelShaderPtr dxPixelShader = boost::dynamic_pointer_cast<DX11PixelShader>(pixelShader);

			std::size_t csize = dxPixelShader->ConstBufferArray.size();
			if( csize > 0 )
			{
			    _pImmediateContext->PSSetConstantBuffers(0,csize,&(dxPixelShader->ConstBufferArray[0]));
			 
			}

			std::size_t sssize = dxPixelShader->SamplerStateArray.size();
			if( sssize > 0 )
			{
				_pImmediateContext->PSSetSamplers(0,sssize,&(dxPixelShader->SamplerStateArray[0]));		 
			}

			std::size_t srsize = dxPixelShader->ShaderResourceViewArray.size();
			if( srsize > 0 )
			{
				_pImmediateContext->PSSetShaderResources(0,srsize,&(dxPixelShader->ShaderResourceViewArray[0]));
				 
			}
		}
	}

	void DX11RenderEngine::OnDrawBegin()
	{
		GDrawTriNumber = 0;

		// Clear the back buffer 
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		_pImmediateContext->ClearRenderTargetView( (ID3D11RenderTargetView*)(_pRenderTarget->GetRenderTargetView(0)->GetLowInterface()), ClearColor );
		_pImmediateContext->ClearDepthStencilView( (ID3D11DepthStencilView*)(_pRenderTarget->GetDepthStencilView()->GetLowInterface()), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}

    void DX11RenderEngine::OnDrawEnd()
	{
		// Present the information rendered to the back buffer to the front buffer (the screen)
		_pSwapChain->Present( 0, 0 );
	}

	void DX11RenderEngine::Draw(std::list<RendererPtr> const& renderList)
	{
		std::list<RendererPtr>::const_iterator iter = renderList.begin();
		while( iter != renderList.end() )
		{
			(*iter)->Draw();
			iter++;
		}
	}

	RenderTargetPtr DX11RenderEngine::CreateRenderTarget(RenderConfig const& renderConfig,void *hWnd)
	{

		HRESULT hr = S_OK;
		UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};

		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = renderConfig.SizeX;
		sd.BufferDesc.Height = renderConfig.SizeY;
		sd.BufferDesc.Format = (DXGI_FORMAT)GPixelFormats[ renderConfig.ColorFormat ].PlatformFormat;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = (HWND)hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !renderConfig.FullScreen;

		ID3D11Device*         pd3dDevice;
		ID3D11DeviceContext*  pImmediateContext;
		IDXGISwapChain*       pSwapChain;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, _driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
												D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &_featureLevel, &pImmediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}

		BOOST_ASSERT(hr>=0);

		_pd3dDevice = MakeComPtr<ID3D11Device>(pd3dDevice);
		_pImmediateContext = MakeComPtr<ID3D11DeviceContext>(pImmediateContext);
		_pSwapChain = MakeComPtr<IDXGISwapChain>(pSwapChain);

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = _pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		BOOST_ASSERT(hr>= 0);

		ID3D11RenderTargetView* pRenderTargetView = NULL;
		hr = _pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &pRenderTargetView );
		pBackBuffer->Release();
		BOOST_ASSERT(hr>=0);

		//Create a stencil & depth buffer.
		ID3D11Texture2D* pDepthStencil = NULL;
		ID3D11DepthStencilView* pDepthStencilView = NULL;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = renderConfig.SizeX;
		descDepth.Height = renderConfig.SizeY;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = _pd3dDevice->CreateTexture2D( &descDepth, NULL, &pDepthStencil );
		BOOST_ASSERT(hr>=0);

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = _pd3dDevice->CreateDepthStencilView( pDepthStencil, &descDSV, &pDepthStencilView );
		BOOST_ASSERT(hr>=0);
		pDepthStencil->Release();
		_pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

		
		_pRenderTarget = boost::make_shared<DX11RenderTarget>();

		DX11RenderTargetViewPtr pRenderView = boost::make_shared<DX11RenderTargetView>(); 
		pRenderView->D3DInterface = MakeComPtr<ID3D11RenderTargetView>(pRenderTargetView);
		DX11DepthStencilViewPtr pDepthView = boost::make_shared<DX11DepthStencilView>();
		pDepthView->D3DInterface = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);
		_pRenderTarget->SetRenderTargetView(pRenderView);
		_pRenderTarget->SetDepthStencilView(pDepthView);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)renderConfig.SizeX;
		vp.Height = (FLOAT)renderConfig.SizeY;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_pImmediateContext->RSSetViewports( 1, &vp );
 
		return _pRenderTarget;
	}

 

}