#include "DX11RenderInclude.h"
 

namespace Disorder
{

	DX11RenderEnginePtr DX11RenderEngine::Create()
	{
		DX11RenderEngine *pEngine = new DX11RenderEngine;
		return DX11RenderEnginePtr(pEngine);
	}

	DXGI_FORMAT DX11RenderEngine::GetPixelFormat(PixelFormat format)
	{
		return (DXGI_FORMAT)format;
	}

	PixelFormat DX11RenderEngine::GetBasePixelFormat(DXGI_FORMAT format)
	{
		return (PixelFormat)format;
	}
 
	DXGI_FORMAT DX11RenderEngine::GetDepthTextureFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
		  case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: 
			  return DXGI_FORMAT_R32G8X24_TYPELESS;
		  case DXGI_FORMAT_D32_FLOAT:            
			  return DXGI_FORMAT_R32_TYPELESS;
		  case DXGI_FORMAT_D24_UNORM_S8_UINT:    
			  return DXGI_FORMAT_R24G8_TYPELESS;
		  case DXGI_FORMAT_D16_UNORM:            
			  return DXGI_FORMAT_R16_TYPELESS;
		  default:
			  BOOST_ASSERT(0);
			  return DXGI_FORMAT_UNKNOWN;
		}
	}

	DXGI_FORMAT DX11RenderEngine::GetDepthShaderResourceFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
		  case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: 
			  return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		  case DXGI_FORMAT_D32_FLOAT:            
			  return DXGI_FORMAT_R32_UINT;
		  case DXGI_FORMAT_D24_UNORM_S8_UINT:    
			  return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		  case DXGI_FORMAT_D16_UNORM:            
			  return DXGI_FORMAT_R16_UNORM;
		  default:
			   BOOST_ASSERT(0);
			  return DXGI_FORMAT_UNKNOWN;
		}
	}
 
	 DX11RenderEngine::DX11RenderEngine()
	 {
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
		 D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_UNKNOWN;
 
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
		sd.BufferDesc.Format = DX11RenderEngine::GetPixelFormat(GConfig->pRenderConfig->ColorFormat); 
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
		DX11RenderTexture2DPtr BackBufferTex = DX11RenderTexture2D::Create(GConfig->pRenderConfig->ColorFormat,sd.BufferDesc.Width,sd.BufferDesc.Height,MakeComPtr<ID3D11Texture2D>(pBackBuffer));
 
		ID3D11RenderTargetView* pRenderTargetView = NULL;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	    RTVDesc.Format = DX11RenderEngine::GetPixelFormat(GConfig->pRenderConfig->ColorFormat);
	    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	    RTVDesc.Texture2D.MipSlice = 0;
		hr = _pd3dDevice->CreateRenderTargetView( pBackBuffer, &RTVDesc, &pRenderTargetView );
		BOOST_ASSERT(SUCCEEDED(hr));

		ID3D11ShaderResourceView* pShaderResourceView = NULL;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = DX11RenderEngine::GetPixelFormat(GConfig->pRenderConfig->ColorFormat);
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;
		hr = _pd3dDevice->CreateShaderResourceView( pBackBuffer,&SRVDesc,&pShaderResourceView );
		BOOST_ASSERT(SUCCEEDED(hr));

		GEngine->RenderSurfaceCache->RenderTarget = DX11RenderSurface::Create(BackBufferTex,MakeComPtr<ID3D11RenderTargetView>(pRenderTargetView),MakeComPtr<ID3D11ShaderResourceView>(pShaderResourceView),NULL);

		//Create a stencil & depth buffer.
		ID3D11Texture2D* pDepthStencil = NULL;
		ID3D11DepthStencilView* pDepthStencilView = NULL;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = GConfig->pRenderConfig->SizeX;
		descDepth.Height = GConfig->pRenderConfig->SizeY;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DX11RenderEngine::GetPixelFormat(GConfig->pRenderConfig->DepthStencilFormat);
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = _pd3dDevice->CreateTexture2D( &descDepth, NULL, &pDepthStencil );
		DX11RenderTexture2DPtr DepthBufferTex = DX11RenderTexture2D::Create(GConfig->pRenderConfig->DepthStencilFormat,descDepth.Width,descDepth.Height,MakeComPtr<ID3D11Texture2D>(pDepthStencil));
 
		BOOST_ASSERT(SUCCEEDED(hr));

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = _pd3dDevice->CreateDepthStencilView( pDepthStencil, &descDSV, &pDepthStencilView );
		BOOST_ASSERT(SUCCEEDED(hr));

		GEngine->RenderSurfaceCache->DepthStencilBuffer = DX11RenderSurface::Create(DepthBufferTex,NULL,NULL,MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView));
 
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
		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;

		IDXGIFactory * pFactory;
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
		BOOST_ASSERT(SUCCEEDED(hr));

		_pDXGIFactory = MakeComPtr<IDXGIFactory>(pFactory);

		EnumAdapters();

		CreateDevice();
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

	D3D11_COMPARISON_FUNC DX11RenderEngine::GetD3DComparisonFunc(ComparisonFunc func)
	{
		switch (func)
		{
		case CF_Never:
			return D3D11_COMPARISON_NEVER;
		case CF_Less:
			return D3D11_COMPARISON_LESS;
		case CF_Equal:
			return D3D11_COMPARISON_EQUAL;
		case CF_Less_Equal:
			return D3D11_COMPARISON_LESS_EQUAL;
		case CF_Greater:
			return D3D11_COMPARISON_GREATER;
		case CF_Not_Equal:
			return D3D11_COMPARISON_NOT_EQUAL;
		case CF_Greater_Equal:
			return D3D11_COMPARISON_GREATER_EQUAL;
		case CF_Always:
			return D3D11_COMPARISON_ALWAYS;
		default:
			BOOST_ASSERT(0);
		}

		return D3D11_COMPARISON_NEVER;
	}

	D3D11_TEXTURE_ADDRESS_MODE DX11RenderEngine::GetD3DAddressMode(TextureAddressMode addrMode)
	{
		switch (addrMode)
		{
		case TAM_Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case TAM_Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case TAM_Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case TAM_Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		default:
			BOOST_ASSERT(0);
		}

		return D3D11_TEXTURE_ADDRESS_CLAMP;
	}

	D3D11_FILTER DX11RenderEngine::GetD3DFilter(SamplerFilter filter)
	{
		switch (filter)
		{
		case SF_Min_Mag_Mip_Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case SF_Min_Mag_Point_Mip_Linear:
			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		case SF_Min_Point_Mag_Linear_Mip_Point:
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		case SF_Min_Point_Mag_Mip_Linear:
			return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		case SF_Min_Linear_Mag_Mip_Point:
			return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		case SF_Min_Linear_Mag_Point_Mip_Linear:
			return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		case SF_Min_Mag_Linear_Mip_Point:
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case SF_Min_Mag_Mip_Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case SF_Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;
		default:
			BOOST_ASSERT(0);
		}

		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	D3D11_STENCIL_OP DX11RenderEngine::GetD3DStencilOp(StencilOperation op)
	{
		switch (op)
		{
		case STENCIL_OP_KEEP:
			return D3D11_STENCIL_OP_KEEP;
		case STENCIL_OP_ZERO:
			return D3D11_STENCIL_OP_ZERO;
		case STENCIL_OP_REPLACE:
			return D3D11_STENCIL_OP_REPLACE;
		case STENCIL_OP_INCR_SAT:
			return D3D11_STENCIL_OP_INCR_SAT;
		case STENCIL_OP_DECR_SAT:
			return D3D11_STENCIL_OP_DECR_SAT;
		case STENCIL_OP_INVERT:
			return D3D11_STENCIL_OP_INVERT;
		case STENCIL_OP_INCR:
			return D3D11_STENCIL_OP_INCR;
		case STENCIL_OP_DECR:
			return D3D11_STENCIL_OP_DECR;
		default:
			BOOST_ASSERT(0);
		}

		return D3D11_STENCIL_OP_KEEP;
	}

	D3D11_BLEND_OP DX11RenderEngine::GetD3DBlendOp(BlendOperation blendOp)
	{
		switch (blendOp)
		{
		case BLEND_OP_ADD:
			return D3D11_BLEND_OP_ADD;
		case BLEND_OP_SUBTRACT:
			return D3D11_BLEND_OP_SUBTRACT;
		case BLEND_OP_REV_SUBTRACT:
			return D3D11_BLEND_OP_REV_SUBTRACT;
		case BLEND_OP_MIN:
			return D3D11_BLEND_OP_MIN;
		case BLEND_OP_MAX:
			return D3D11_BLEND_OP_MAX;
		default:
			BOOST_ASSERT(0);
		}

		return D3D11_BLEND_OP_ADD;
	}

	D3D11_BLEND DX11RenderEngine::GetD3DBlendDesc(BlendOptions blendOptions)
	{
		switch (blendOptions)
		{
		case BLEND_ZERO:
			return D3D11_BLEND_ZERO;
		case BLEND_ONE:
			return D3D11_BLEND_ONE;
		case BLEND_SRC_COLOR:
			return D3D11_BLEND_SRC_COLOR;
		case BLEND_INV_SRC_COLOR:
			return D3D11_BLEND_INV_SRC_COLOR;
		case BLEND_SRC_ALPHA:
			return D3D11_BLEND_SRC_ALPHA;
		case BLEND_INV_SRC_ALPHA:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case BLEND_DEST_ALPHA:
			return D3D11_BLEND_DEST_ALPHA;
		case BLEND_INV_DEST_ALPHA:
			return D3D11_BLEND_INV_DEST_ALPHA;
		case BLEND_DEST_COLOR:
			return D3D11_BLEND_DEST_COLOR;
		case BLEND_INV_DEST_COLOR:
			return D3D11_BLEND_INV_DEST_COLOR;
		case BLEND_SRC_ALPHA_SAT:
			return D3D11_BLEND_SRC_ALPHA_SAT;
		case BLEND_BLEND_FACTOR:
			return D3D11_BLEND_BLEND_FACTOR;
		case BLEND_INV_BLEND_FACTOR:
			return D3D11_BLEND_INV_BLEND_FACTOR;
		case BLEND_SRC1_COLOR:
			return D3D11_BLEND_SRC1_COLOR;
		case BLEND_INV_SRC1_COLOR:
			return D3D11_BLEND_INV_SRC1_COLOR;
		case BLEND_SRC1_ALPHA:
			return D3D11_BLEND_SRC1_ALPHA;
		case BLEND_INV_SRC1_ALPHA:
			return D3D11_BLEND_INV_SRC1_ALPHA;
		default:
			BOOST_ASSERT(0);
		}

		return D3D11_BLEND_ZERO;
	}

	D3D_PRIMITIVE_TOPOLOGY DX11RenderEngine::GetPlatformTopology(TopologyType tType)
	{
		switch (tType)
		{
		case TT_PointList:
			return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case TT_LineList:
			return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case TT_LineStrip:
			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case TT_TriangleList:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case TT_TriangleStrip:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case TT_LineList_Adj:
			return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
		case TT_LineStrip_Adj:
			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
		case TT_TriangleList_Adj:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
		case TT_TriangleStrip_Adj:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
		case TT_1_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
		case TT_2_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
		case TT_3_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
		case TT_4_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
		case TT_5_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
		case TT_6_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
		case TT_7_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
		case TT_8_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
		case TT_9_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
		case TT_10_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
		case TT_11_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
		case TT_12_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
		case TT_13_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
		case TT_14_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
		case TT_15_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
		case TT_16_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
		case TT_17_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
		case TT_18_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
		case TT_19_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
		case TT_20_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
		case TT_21_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
		case TT_22_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
		case TT_23_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
		case TT_24_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
		case TT_25_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
		case TT_26_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
		case TT_27_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
		case TT_28_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
		case TT_29_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
		case TT_30_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
		case TT_31_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
		case TT_32_Ctrl_Pt_PatchList:
			return D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
		default:
			BOOST_ASSERT(0);
			return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}

	void DX11RenderEngine::SetPrimitiveTopology(TopologyType topologyType)
	{
		_pImmediateContext->IASetInputLayout(NULL);
		_pImmediateContext->IASetPrimitiveTopology(DX11RenderEngine::GetPlatformTopology(topologyType));
		_pImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	}

	void DX11RenderEngine::SetRenderLayout( RenderLayoutPtr const& renderLayout)
	{
		DX11RenderLayoutPtr dxRenderLayout = boost::dynamic_pointer_cast<DX11RenderLayout>(renderLayout);

		TopologyType topology = renderLayout->GetTopology();
		_pImmediateContext->IASetPrimitiveTopology(DX11RenderEngine::GetPlatformTopology(topology));

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

	void DX11RenderEngine::Draw(unsigned int vertexCount,unsigned int startVertexLocation)
	{
		_pImmediateContext->Draw(vertexCount,startVertexLocation);
	}
 
	void DX11RenderEngine::SetEffect(RenderEffectPtr const& effect)
	{
		if( effect == NULL ) // clean up
		{			 
			void* nullArrayInputRes[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ] = { NULL };
			_pImmediateContext->VSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ,(ID3D11Buffer**)&nullArrayInputRes);
			_pImmediateContext->PSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ,(ID3D11Buffer**)&nullArrayInputRes);

			void* nullArraySampler[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { NULL };
			_pImmediateContext->VSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,(ID3D11SamplerState**)&nullArraySampler);
			_pImmediateContext->PSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,(ID3D11SamplerState**)&nullArraySampler);

			void* nullArray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { NULL };
			_pImmediateContext->VSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ,(ID3D11ShaderResourceView**)&nullArray);
			_pImmediateContext->PSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ,(ID3D11ShaderResourceView**)&nullArray);
		}
		else
		{
			// render State
			SetBlendState(effect->GetBlendState());
			SetRasterizeState(effect->GetRasterizeState());
			SetDepthStencilState(effect->GetDepthStencilState());

			effect->UpdateShaderParameter();

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
					void* nullArray[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ] = { NULL };
					_pImmediateContext->VSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ,(ID3D11Buffer**)&nullArray);
				}


				std::size_t sssize = dxVertexShader->CachedSamplerState.size();
				if( sssize > 0 )
				{
					_pImmediateContext->VSSetSamplers(0,sssize,&(dxVertexShader->CachedSamplerState[0]));	 
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { NULL };
					_pImmediateContext->VSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,(ID3D11SamplerState**)&nullArray);
				}


				std::size_t srsize = dxVertexShader->CachedShaderResourceView.size();
				if( srsize > 0 )
				{
					_pImmediateContext->VSSetShaderResources(0,srsize,&(dxVertexShader->CachedShaderResourceView[0]));	 
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { NULL };
					_pImmediateContext->VSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ,(ID3D11ShaderResourceView**)&nullArray);
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
					void* nullArray[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ] = { NULL };
					_pImmediateContext->PSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ,(ID3D11Buffer**)&nullArray);
				}

				std::size_t sssize = dxPixelShader->CachedSamplerState.size();
				if( sssize > 0 )
				{
					_pImmediateContext->PSSetSamplers(0,sssize,&(dxPixelShader->CachedSamplerState[0]));		 
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { NULL };
					_pImmediateContext->PSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,(ID3D11SamplerState**)&nullArray);
				}

				std::size_t srsize = dxPixelShader->CachedShaderResourceView.size();
				if( srsize > 0 )
				{
					_pImmediateContext->PSSetShaderResources(0,srsize,&(dxPixelShader->CachedShaderResourceView[0]));				 
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { NULL };
					_pImmediateContext->PSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ,(ID3D11ShaderResourceView**)&nullArray);
				}
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

	void DX11RenderEngine::SetRenderTarget(const std::vector<RenderSurfacePtr>& renderTarget,const RenderSurfacePtr& depthStencil,bool useReadOnlyDepthStencil)
	{
		std::vector<ID3D11RenderTargetView*> vRenderTarget;
		for(size_t i=0;i<renderTarget.size();i++ )
		{
			DX11RenderSurfacePtr dxRenderTarget = renderTarget[i] == NULL ? NULL : boost::dynamic_pointer_cast<DX11RenderSurface>(renderTarget[i]);
			vRenderTarget.push_back(dxRenderTarget->RenderTargetView.get());
		}

		DX11RenderSurfacePtr dxDepthStencil = depthStencil == NULL ? NULL : boost::dynamic_pointer_cast<DX11RenderSurface>(depthStencil);
		if( _featureLevel >= D3D_FEATURE_LEVEL_11_0 && useReadOnlyDepthStencil && dxDepthStencil->DepthStencilReadOnlyView == NULL )
		{
			BOOST_ASSERT(0);
			return;
		}
		if( _featureLevel >= D3D_FEATURE_LEVEL_11_0 && useReadOnlyDepthStencil )
			_pImmediateContext->OMSetRenderTargets(vRenderTarget.size(),&(vRenderTarget[0]),dxDepthStencil->DepthStencilReadOnlyView.get());
		else
		    _pImmediateContext->OMSetRenderTargets(vRenderTarget.size(),&(vRenderTarget[0]),dxDepthStencil->DepthStencilView.get());

	}

	void DX11RenderEngine::SaveRenderSurface(RenderSurfacePtr const& surface,std::string const& fileName)
	{
		//create stage texture
		D3D11_TEXTURE2D_DESC desc;
		ID3D11Texture2D *pTex = (ID3D11Texture2D*)surface->Tex2DResource->GetLowInterface();
		pTex->GetDesc(&desc);
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.MiscFlags = 0;

		ID3D11Texture2D* pStageTex2D = NULL; 
		HRESULT hr = _pd3dDevice->CreateTexture2D( &desc, NULL, &pStageTex2D );
		BOOST_ASSERT(hr==S_OK);

		_pImmediateContext->CopyResource(pStageTex2D,pTex);

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		_pImmediateContext->Map(pStageTex2D,D3D11CalcSubresource(0, 0, 1),D3D11_MAP_READ,0,&MappedResource);
		
		ImagePtr image = Image::Create(desc.Width,desc.Height,surface->Tex2DResource->Format,MappedResource.pData);
		GImageManager->Save(fileName,image);

		_pImmediateContext->Unmap(pStageTex2D,D3D11CalcSubresource(0, 0, 1));
		pStageTex2D->Release();
		
	}

	void DX11RenderEngine::SetRenderTarget(const RenderSurfacePtr& renderTarget,const RenderSurfacePtr& depthStencil,bool useReadOnlyDepthStencil)
	{
		DX11RenderSurfacePtr dxRenderTarget = renderTarget == NULL ? NULL : boost::dynamic_pointer_cast<DX11RenderSurface>(renderTarget);
		DX11RenderSurfacePtr dxDepthStencil = depthStencil == NULL ? NULL : boost::dynamic_pointer_cast<DX11RenderSurface>(depthStencil);

		ID3D11RenderTargetView* pView = dxRenderTarget->RenderTargetView.get();
		if( _featureLevel >= D3D_FEATURE_LEVEL_11_0 && useReadOnlyDepthStencil && dxDepthStencil->DepthStencilReadOnlyView == NULL )
		{
			BOOST_ASSERT(0);
			return;
		}

		if( _featureLevel >= D3D_FEATURE_LEVEL_11_0 && useReadOnlyDepthStencil )
			_pImmediateContext->OMSetRenderTargets(1,&pView,dxDepthStencil->DepthStencilReadOnlyView.get());
		else
		    _pImmediateContext->OMSetRenderTargets(1,&pView,dxDepthStencil->DepthStencilView.get());
	}

	void DX11RenderEngine::ClearRenderTarget(const RenderSurfacePtr& renderTarget,const glm::vec4& color )
	{
		DX11RenderSurfacePtr dxRenderTarget = boost::dynamic_pointer_cast<DX11RenderSurface>(renderTarget);
		_pImmediateContext->ClearRenderTargetView(dxRenderTarget->RenderTargetView.get(), glm::value_ptr(color));
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
 
		
	}

    void DX11RenderEngine::OnDrawEnd()
	{
		_pSwapChain->Present( 0, 0 );
	}


	

 

}