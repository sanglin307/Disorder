#include "DX11RenderInclude.h"
 

namespace Disorder
{
 
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
 
	DX11RenderEngine::DX11RenderEngine(HWND hWnd)
	{
		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&_pDXGIFactory));
		BOOST_ASSERT(SUCCEEDED(hr));

		EnumAdapters();
		CreateDevice();
		CreateViewport(hWnd);
	}

	DX11RenderEngine::~DX11RenderEngine()
	{
		if (_pImmediateContext)
		{
			_pImmediateContext->ClearState();
			_pImmediateContext->Release();
		}

		_pSwapChain->Release();
		_pd3dDevice->Release();
		for (size_t i = 0; i < _vDXGIAdapter.size(); i++)
		{
			_vDXGIAdapter[i]->Release();
		}
		_vDXGIAdapter.clear();

		_pDXGIFactory->Release();
	}


	 void DX11RenderEngine::EnumAdapters()
	 {
		 BOOST_ASSERT(_pDXGIFactory != NULL );
		 BOOST_ASSERT(_vDXGIAdapter.size() == 0);

		 IDXGIAdapter * pAdapter; 
		 UINT i =0;
		 while(_pDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND) 
		 { 
			 _vDXGIAdapter.push_back(pAdapter);
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
		 
 
		 HRESULT Hr = D3D11CreateDevice(_vDXGIAdapter[0], DriverType, NULL, DeviceCreationFlags, FeatureLevels, numFeatureLevels, D3D11_SDK_VERSION, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
		 BOOST_ASSERT( SUCCEEDED(Hr) );
 
	 }

	 void DX11RenderEngine::CreateViewport(HWND hWnd)
	{
		HRESULT hr = S_OK;
	
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );

		DXGI_FORMAT dxFormat = DX11RenderEngine::GetPixelFormat(GConfig->pRenderConfig->ColorFormat);
		while (GConfig->pRenderConfig->MultiSampleCount > 1)
		{
			_pd3dDevice->CheckMultisampleQualityLevels(dxFormat, GConfig->pRenderConfig->MultiSampleCount, &GConfig->pRenderConfig->MultiSampleQuality);
			if (GConfig->pRenderConfig->MultiSampleQuality == 0) // invalid
			{
				GConfig->pRenderConfig->MultiSampleCount--;
			}
			else
			{
				GConfig->pRenderConfig->MultiSampleQuality--;
				break;
			}
		}
 
		sd.BufferCount = 1;
		sd.BufferDesc.Width = GConfig->pRenderConfig->SizeX;
		sd.BufferDesc.Height = GConfig->pRenderConfig->SizeY;
		sd.BufferDesc.Format = dxFormat;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow =  hWnd;
		sd.SampleDesc.Count = GConfig->pRenderConfig->MultiSampleCount;
		sd.SampleDesc.Quality = GConfig->pRenderConfig->MultiSampleQuality;
		sd.Windowed = !GConfig->pRenderConfig->FullScreen;
 
		hr = _pDXGIFactory->CreateSwapChain(_pd3dDevice, &sd, &_pSwapChain);
		BOOST_ASSERT(SUCCEEDED(hr));
		 
		hr = _pDXGIFactory->MakeWindowAssociation(hWnd,DXGI_MWA_NO_WINDOW_CHANGES);
		BOOST_ASSERT(SUCCEEDED(hr));

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = _pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		BOOST_ASSERT(SUCCEEDED(hr));
		DX11RenderTexture2D* BackBufferTex = new DX11RenderTexture2D(GConfig->pRenderConfig->ColorFormat, sd.BufferDesc.Width, sd.BufferDesc.Height, SV_RenderTarget | SV_ShaderResource, false, 
			GConfig->pRenderConfig->MultiSampleCount, GConfig->pRenderConfig->MultiSampleQuality,pBackBuffer);
 
		SurfaceView* rtView = GRenderResourceMgr->CreateSurfaceView(SV_RenderTarget, BackBufferTex,GConfig->pRenderConfig->ColorFormat);
		SurfaceView* srView = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, BackBufferTex, GConfig->pRenderConfig->ColorFormat);

		RenderTexture2D* DepthBufferTex = GRenderResourceMgr->CreateTexture2D("DepthBufferTex",NULL, GConfig->pRenderConfig->DepthStencilFormat, GConfig->pRenderConfig->SizeX, GConfig->pRenderConfig->SizeY, false, false, SV_DepthStencil,1, NULL,0);

		SurfaceView* dsView = GRenderResourceMgr->CreateSurfaceView(SV_DepthStencil, DepthBufferTex, GConfig->pRenderConfig->DepthStencilFormat);

		std::map<ESurfaceLocation, SurfaceView*> viewMap;
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget1, rtView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_DepthStencil, dsView));
		RenderSurface* mainSurface = new DX11RenderSurface(viewMap);
 
		GRenderSurface->MainTarget = new MainRenderTarget(mainSurface, srView, rtView, dsView);

		// Setup the viewport
		SetViewport((float)GConfig->pRenderConfig->SizeX, (float)GConfig->pRenderConfig->SizeY, 0.f, 1.f, 0.f, 0.f);
 
	
	}

	void DX11RenderEngine::SetViewport(float width, float height, float minDepth, float maxDepth, float topX, float topY)
	{
		 D3D11_VIEWPORT vp;
		 vp.Width = width;
		 vp.Height = height;
		 vp.MinDepth = minDepth;
		 vp.MaxDepth = maxDepth;
		 vp.TopLeftX = topX;
		 vp.TopLeftY = topY;
		 _pImmediateContext->RSSetViewports(1, &vp);
	}
 

   	void* DX11RenderEngine::Map(RenderBuffer* buffer,BufferAccess bufferAccess)
	{
		D3D11_MAP flag = D3D11_MAP_WRITE;
		if( bufferAccess == BA_Read_Only )
			flag = D3D11_MAP_READ;
		else if (bufferAccess == BA_Write_Only)
			flag = D3D11_MAP_WRITE_DISCARD;
		else if( bufferAccess == BA_Read_Write)
			flag = D3D11_MAP_READ_WRITE;

		D3D11_MAPPED_SUBRESOURCE pMappedResource;
		HRESULT hr = _pImmediateContext->Map((ID3D11Buffer*)(buffer->GetHandle()),D3D11CalcSubresource(0, 0, 1),flag,0,&pMappedResource);

		return pMappedResource.pData;
	}
	
	void DX11RenderEngine::UnMap(RenderBuffer* buffer)
	{
		_pImmediateContext->Unmap((ID3D11Buffer*)(buffer->GetHandle()),D3D11CalcSubresource(0, 0, 1));
	}

	void DX11RenderEngine::SetBlendState(BlendState* blendState)
	{ 

		if (CachedBlendState != blendState || CachedBlendState->BlendFactor[0] != CachedBlendFactor[0] || CachedBlendState->BlendFactor[1] != CachedBlendFactor[1] || 
			CachedBlendState->BlendFactor[2] != CachedBlendFactor[2] || CachedBlendState->BlendFactor[3] != CachedBlendFactor[3] || 
			CachedBlendState->SampleMask != CachedBlendSampleMask )
		{
			CachedBlendState = blendState;
			CachedBlendFactor[0] = blendState->BlendFactor[0];
			CachedBlendFactor[1] = blendState->BlendFactor[1];
			CachedBlendFactor[2] = blendState->BlendFactor[2];
			CachedBlendFactor[3] = blendState->BlendFactor[3];
			CachedBlendSampleMask = blendState->SampleMask;

		    ID3D11BlendState *pState = (ID3D11BlendState*)(blendState->GetHandle());
 
		    _pImmediateContext->OMSetBlendState(pState,blendState->BlendFactor,blendState->SampleMask);	 
		
		}
	}

    void DX11RenderEngine::SetRasterizeState(RasterizeState* rasterizeState)
	{
		if( CachedRasterizeState != rasterizeState )
		{
			CachedRasterizeState = rasterizeState;
		    ID3D11RasterizerState* pState = (ID3D11RasterizerState*)(rasterizeState->GetHandle());
		    _pImmediateContext->RSSetState(pState);
		}
	 
	}

	D3D11_COMPARISON_FUNC DX11RenderEngine::GetD3DComparisonFunc(ComparisonFunc func)
	{
		switch (func)
		{
		case CF_None:
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

	D3D11_FILTER DX11RenderEngine::GetD3DFilter(SamplerFilter filter, bool bCompareTypeFilter)
	{
		switch (filter)
		{
			case SF_Min_Mag_Mip_Point:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
				else
					return D3D11_FILTER_MIN_MAG_MIP_POINT;
			}
			case SF_Min_Mag_Point_Mip_Linear:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
				else
					return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			}
			case SF_Min_Point_Mag_Linear_Mip_Point:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
				else
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			}
			case SF_Min_Point_Mag_Mip_Linear:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
				else
					return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			}
			case SF_Min_Linear_Mag_Mip_Point:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
				else
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			}
			
			case SF_Min_Linear_Mag_Point_Mip_Linear:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				else
					return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			}	
			case SF_Min_Mag_Linear_Mip_Point:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
				else
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			}
			case SF_Min_Mag_Mip_Linear:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				else
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
			case SF_Anisotropic:
			{
				if (bCompareTypeFilter)
					return D3D11_FILTER_COMPARISON_ANISOTROPIC;
				else
					return D3D11_FILTER_ANISOTROPIC;
			}
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
		CachedTopology = topologyType;
		_pImmediateContext->IASetPrimitiveTopology(DX11RenderEngine::GetPlatformTopology(topologyType));
	}

	void DX11RenderEngine::SetRenderLayout( RenderLayout* renderLayout)
	{
		DX11RenderLayout* dxRenderLayout = (DX11RenderLayout*)renderLayout;

		SetPrimitiveTopology(renderLayout->GetTopology());
		//set layout
		_pImmediateContext->IASetInputLayout((ID3D11InputLayout*)(renderLayout->GetHandle()));
		 
		//set vertex buffer		 
		_pImmediateContext->IASetVertexBuffers(0,dxRenderLayout->VertexArray.size(),&(dxRenderLayout->VertexArray[0]),&(dxRenderLayout->VertexArrayElementSize[0]),&(dxRenderLayout->VertexArrayOffset[0]));
 
		RenderBuffer* indexBuffer = renderLayout->GetIndexBuffer();
		if(indexBuffer != NULL )
		{
			ID3D11Buffer *pBuffer = (ID3D11Buffer *)(indexBuffer->GetHandle());
			unsigned int size = indexBuffer->GetElementSize() ;
			if( size == 2)
			    _pImmediateContext->IASetIndexBuffer(pBuffer,DXGI_FORMAT_R16_UINT,0);
			else if( size == 4)
				 _pImmediateContext->IASetIndexBuffer(pBuffer,DXGI_FORMAT_R32_UINT,0);
		}
 
 
	}

	void DX11RenderEngine::UpdateSubresource(RenderBuffer* buffer,void* pSrcData,unsigned int srcDataSize)
	{
		ID3D11Buffer *pBuffer = (ID3D11Buffer *)(buffer->GetHandle());
		_pImmediateContext->UpdateSubresource(pBuffer, 0, NULL, pSrcData, srcDataSize, 0);
	}


	void DX11RenderEngine::DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation)
	{
		_pImmediateContext->DrawIndexed(indexCount,startIndexLocation,baseVertexLocation);
		GEngine->Stat.DrawTriNumber += GetTriangleCountFromTopology(CachedTopology, indexCount);
	}

	void DX11RenderEngine::Draw(unsigned int vertexCount,unsigned int startVertexLocation)
	{
		_pImmediateContext->Draw(vertexCount,startVertexLocation);

		GEngine->Stat.DrawTriNumber += GetTriangleCountFromTopology(CachedTopology, vertexCount);
	}
 
	void DX11RenderEngine::SetEffect(RenderEffect* effect)
	{
		if( effect == NULL ) // clean up
		{			 
			void* nullArrayInputRes[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ] = { NULL };
			_pImmediateContext->VSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT  ,(ID3D11Buffer**)&nullArrayInputRes);
			_pImmediateContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (ID3D11Buffer**)&nullArrayInputRes);
			_pImmediateContext->PSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ,(ID3D11Buffer**)&nullArrayInputRes);

			void* nullArraySampler[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { NULL };
			_pImmediateContext->VSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,(ID3D11SamplerState**)&nullArraySampler);
			_pImmediateContext->GSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (ID3D11SamplerState**)&nullArraySampler);
			_pImmediateContext->PSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,(ID3D11SamplerState**)&nullArraySampler);

			void* nullArray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { NULL };
			_pImmediateContext->VSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ,(ID3D11ShaderResourceView**)&nullArray);
			_pImmediateContext->GSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (ID3D11ShaderResourceView**)&nullArray);
			_pImmediateContext->PSSetShaderResources(0,D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ,(ID3D11ShaderResourceView**)&nullArray);
		}
		else
		{
			// render State
			SetBlendState(effect->GetBlendState());
			SetRasterizeState(effect->GetRasterizeState());
			SetDepthStencilState(effect->GetDepthStencilState());

			effect->UpdateShaderParameter();

			ShaderObject* vertexShader = effect->GetVertexShader();
			if( vertexShader != NULL )
			{
				ID3D11VertexShader *pShader = (ID3D11VertexShader *)(vertexShader->GetHandle());
				_pImmediateContext->VSSetShader(pShader, NULL,0);

				DX11ShaderObject* dxVertexShader = (DX11ShaderObject*)vertexShader;
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

			ShaderObject* geometryShader = effect->GetGeometryShader();
			if (geometryShader != NULL)
			{
				ID3D11GeometryShader *pShader = (ID3D11GeometryShader *)(geometryShader->GetHandle());
				_pImmediateContext->GSSetShader(pShader, NULL, 0);

				DX11ShaderObject* dxGeometryShader = (DX11ShaderObject*)geometryShader;
				std::size_t cbsize = dxGeometryShader->CachedConstBuffer.size();
				if (cbsize > 0)
				{
					_pImmediateContext->GSSetConstantBuffers(0, cbsize, &(dxGeometryShader->CachedConstBuffer[0]));
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { NULL };
					_pImmediateContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, (ID3D11Buffer**)&nullArray);
				}


				std::size_t sssize = dxGeometryShader->CachedSamplerState.size();
				if (sssize > 0)
				{
					_pImmediateContext->GSSetSamplers(0, sssize, &(dxGeometryShader->CachedSamplerState[0]));
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { NULL };
					_pImmediateContext->GSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, (ID3D11SamplerState**)&nullArray);
				}


				std::size_t srsize = dxGeometryShader->CachedShaderResourceView.size();
				if (srsize > 0)
				{
					_pImmediateContext->GSSetShaderResources(0, srsize, &(dxGeometryShader->CachedShaderResourceView[0]));
				}
				else
				{
					void* nullArray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { NULL };
					_pImmediateContext->GSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, (ID3D11ShaderResourceView**)&nullArray);
				}

			}
			else
			{
				_pImmediateContext->GSSetShader(NULL, NULL, 0);
			}

			ShaderObject* pixelShader = effect->GetPixelShader();
			if( pixelShader != NULL )
			{
				ID3D11PixelShader *pShader = (ID3D11PixelShader *)(pixelShader->GetHandle());
				_pImmediateContext->PSSetShader(pShader,NULL,0);

				DX11ShaderObject* dxPixelShader = (DX11ShaderObject*)pixelShader;

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
			else
			{
				_pImmediateContext->PSSetShader(NULL, NULL, 0);
			}
		}
	}

	void DX11RenderEngine::SetDepthStencilState(DepthStencilState* depthStencilState)
	{
		if( CachedDepthStencilState != depthStencilState || depthStencilState->StencilRef != CachedStencilRef )
		{
			CachedDepthStencilState = depthStencilState;
			CachedStencilRef = depthStencilState->StencilRef;

			ID3D11DepthStencilState* pState = (ID3D11DepthStencilState*)(depthStencilState->GetHandle());
			_pImmediateContext->OMSetDepthStencilState(pState,depthStencilState->StencilRef);
		}
	}
 
	void DX11RenderEngine::SaveSurfaceView(SurfaceView* surface, std::string const& fileName)
	{
		//create stage texture
		D3D11_TEXTURE2D_DESC desc;
		RenderTexture2D* tex = (RenderTexture2D*)surface->Resource;
		ID3D11Texture2D *pTex = (ID3D11Texture2D*)tex->GetHandle();
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
		
		int dataSize = desc.Width * desc.Height * RenderEngine::ComputePixelSizeBits(tex->Format) / 8;
		Image* image = new Image(fileName,eIT_PNG,desc.Width,desc.Height,tex->Format,(BYTE*)MappedResource.pData,dataSize);
		GImageManager->Save(fileName,image);

		_pImmediateContext->Unmap(pStageTex2D,D3D11CalcSubresource(0, 0, 1));
		pStageTex2D->Release();
		
	}

	void DX11RenderEngine::CopyTexture2D(RenderTexture2D* srcTexture, RenderTexture2D* dstTexture)
	{
		BOOST_ASSERT(srcTexture->Height == dstTexture->Height);
		BOOST_ASSERT(srcTexture->Width == dstTexture->Width);
		BOOST_ASSERT(srcTexture->Format == dstTexture->Format);

		_pImmediateContext->CopyResource((ID3D11Resource *)dstTexture->GetHandle(), (ID3D11Resource *)srcTexture->GetHandle());
	}
	 
	void DX11RenderEngine::SetRenderTarget(RenderSurface* renderTarget, int sliceIndex,bool useReadOnlyDepthStencil)
	{
		std::vector<ID3D11RenderTargetView*> vRenderTarget;
		ID3D11DepthStencilView* pDepthView = NULL;
		
		DX11RenderSurface* dxSurface = (DX11RenderSurface*)renderTarget;

		for (size_t i = 0; i < SL_SurfaceLoactionMax; i++)
		{
			if (dxSurface->_surfacesViewArray[i] == NULL)
				continue;

			if (i == SL_DepthStencil && pDepthView == NULL)
			{
				DX11SurfaceView* dxDepthStencil = (DX11SurfaceView*)dxSurface->_surfacesViewArray[i];
				if (sliceIndex < 0 || dxDepthStencil->DepthStencilHandleArray.size() == 0 )
				{
					if (_featureLevel >= D3D_FEATURE_LEVEL_11_0 && useReadOnlyDepthStencil)
						pDepthView = dxDepthStencil->ReadonlyDepthStencil;
					else
						pDepthView = dxDepthStencil->DepthStencilHandle;
				}
				else
				{
					BOOST_ASSERT((size_t)sliceIndex < dxDepthStencil->DepthStencilHandleArray.size());
					pDepthView = dxDepthStencil->DepthStencilHandleArray[sliceIndex];
				}
			}
			else if (i >= SL_RenderTarget1 && i <= SL_RenderTarget8)
			{
				DX11SurfaceView* dxRenderTarget = (DX11SurfaceView*)dxSurface->_surfacesViewArray[i];
				if (sliceIndex < 0 || dxRenderTarget->RenderTargetHandleArray.size() == 0)
				{
					vRenderTarget.push_back(dxRenderTarget->RenderTargetHandle);
				}
				else
				{
					vRenderTarget.push_back(dxRenderTarget->RenderTargetHandleArray[sliceIndex]);
				}
			}
		}
	
		_pImmediateContext->OMSetRenderTargets(vRenderTarget.size(), vRenderTarget.data(), pDepthView);
	 
	}

	void DX11RenderEngine::ClearRenderTarget(SurfaceView* renderTarget, const glm::vec4& color, int sliceIndex)
	{
		DX11SurfaceView* target = (DX11SurfaceView*)renderTarget;
		if (sliceIndex < 0 || target->RenderTargetHandleArray.size() == 0)
	        _pImmediateContext->ClearRenderTargetView(target->RenderTargetHandle, glm::value_ptr(color));
		else
			_pImmediateContext->ClearRenderTargetView(target->RenderTargetHandleArray[sliceIndex], glm::value_ptr(color));
	}

	 
	void DX11RenderEngine::ClearRenderSurface(RenderSurface* renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil, int sliceIndex)
	{
		DX11RenderSurface* dxSurface = (DX11RenderSurface*)renderSurface;
		for (size_t i = 0; i < SL_SurfaceLoactionMax; i++)
		{
			if (dxSurface->_surfacesViewArray[i] != NULL)
			{
				if (i == SL_DepthStencil)
				{
					ClearDepthStencil(dxSurface->_surfacesViewArray[i], bClearDepth, depth, bClearStencil, stencil,sliceIndex);
				}
				else
				{
					ClearRenderTarget(dxSurface->_surfacesViewArray[i], color,sliceIndex);
				}
			}
		}
	}

	void DX11RenderEngine::ClearDepthStencil(SurfaceView* depthBuffer, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil, int sliceIndex)
	{
		unsigned int flag = 0;
		if( bClearDepth )
			flag |= D3D11_CLEAR_DEPTH;

		if( bClearStencil )
			flag |= D3D11_CLEAR_STENCIL;

		DX11SurfaceView* dxDepthStencil = (DX11SurfaceView*)depthBuffer; 
		if (sliceIndex < 0 || dxDepthStencil->DepthStencilHandleArray.size() == 0)
		   _pImmediateContext->ClearDepthStencilView(dxDepthStencil->DepthStencilHandle, flag, depth, stencil);
		else
		{
			_pImmediateContext->ClearDepthStencilView(dxDepthStencil->DepthStencilHandleArray[sliceIndex], flag, depth, stencil);
		}
			 
	}

	void DX11RenderEngine::OnFrameBegin()
	{	
		GEngine->Stat.OnFrameBegin();
	}

	void DX11RenderEngine::OnFrameEnd()
	{
		_pSwapChain->Present( GConfig->pRenderConfig->SyncInterval, 0 );

		GEngine->Stat.OnFrameEnd();
	}


	
 

}