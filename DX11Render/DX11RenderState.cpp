#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11SamplerState::GetHandle()
	{
		return D3DInterface.get();
	}

	DX11SamplerStatePtr DX11SamplerState::Create(SamplerDesc* pSamplerDesc)
	{
		DX11SamplerState *pSampler = new DX11SamplerState;

		pSampler->Desc = *pSamplerDesc;

		D3D11_SAMPLER_DESC samDesc;
        ZeroMemory( &samDesc, sizeof(samDesc) );

		samDesc.MaxAnisotropy = pSamplerDesc->MaxAnisotropy;
		samDesc.Filter = DX11RenderEngine::GetD3DFilter(pSamplerDesc->Filter);
		samDesc.AddressU = DX11RenderEngine::GetD3DAddressMode(pSamplerDesc->AddressU);
		samDesc.AddressV = DX11RenderEngine::GetD3DAddressMode(pSamplerDesc->AddressV);
		samDesc.AddressW = DX11RenderEngine::GetD3DAddressMode(pSamplerDesc->AddressW);
		samDesc.BorderColor[0] = pSamplerDesc->BorderColor[0];
		samDesc.BorderColor[1] = pSamplerDesc->BorderColor[1];
		samDesc.BorderColor[2] = pSamplerDesc->BorderColor[2];
		samDesc.BorderColor[3] = pSamplerDesc->BorderColor[3];
		samDesc.ComparisonFunc = DX11RenderEngine::GetD3DComparisonFunc(pSamplerDesc->CompareFunc);
        samDesc.MaxLOD = pSamplerDesc->MaxLOD;
		samDesc.MinLOD = pSamplerDesc->MinLOD;
		samDesc.MipLODBias = pSamplerDesc->MipLODBias;

        DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 

		ID3D11SamplerState *pSamplerState;

		HRESULT hr = renderEngine->D3DDevice()->CreateSamplerState( &samDesc, &pSamplerState );
		BOOST_ASSERT(hr==S_OK);

		pSampler->D3DInterface = MakeComPtr<ID3D11SamplerState>(pSamplerState);

		return DX11SamplerStatePtr(pSampler);

	}

	//======================================================================

	void* DX11RasterizeState::GetHandle()
	{
		return D3DInterface.get();
	}

	DX11RasterizeStatePtr DX11RasterizeState::Create(RasterizeDesc *pDesc)
	{
		DX11RasterizeState *pRasterizeState = new DX11RasterizeState;

		pRasterizeState->Desc = *pDesc;

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		
		if( pDesc->CullMode == RCM_None)
			desc.CullMode = D3D11_CULL_NONE;
		else if( pDesc->CullMode == RCM_CullFront)
			desc.CullMode = D3D11_CULL_FRONT;
		else if( pDesc->CullMode == RCM_CullBack)
			desc.CullMode = D3D11_CULL_BACK;

		desc.DepthBias = pDesc->DepthBias;
		desc.DepthBiasClamp = pDesc->DepthBiasClamp;
		desc.DepthClipEnable = pDesc->DepthClipEnable;

		if( pDesc->FillMode == RFM_Wireframe )
			desc.FillMode = D3D11_FILL_WIREFRAME;
		else if( pDesc->FillMode == RFM_Solid)
			desc.FillMode = D3D11_FILL_SOLID;

		desc.FrontCounterClockwise = pDesc->FrontCounterClockwise;
		desc.MultisampleEnable = pDesc->MultisampleEnable;
		if (GConfig->pRenderConfig->MultiSampleCount > 1)
			desc.MultisampleEnable = true;
		desc.AntialiasedLineEnable = pDesc->AntialiasedLineEnable;


		desc.ScissorEnable = pDesc->ScissorEnable;
		desc.SlopeScaledDepthBias = pDesc->SlopeScaledDepthBias;

		ID3D11RasterizerState *pState;
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		HRESULT hr = renderEngine->D3DDevice()->CreateRasterizerState(&desc,&pState);
		BOOST_ASSERT(hr==S_OK);

		pRasterizeState->D3DInterface = MakeComPtr<ID3D11RasterizerState>(pState);


		return DX11RasterizeStatePtr(pRasterizeState);

	}
	//==================================================================

	void* DX11DepthStencilState::GetHandle()
	{
		return D3DInterface.get();
	}

	DX11DepthStencilStatePtr DX11DepthStencilState::Create(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		DX11DepthStencilState *pDepthStencilState = new DX11DepthStencilState;

		pDepthStencilState->Desc = *pDepthStencilDesc;

		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc,sizeof(desc));

		desc.DepthEnable = pDepthStencilDesc->DepthEnable;
		if(pDepthStencilDesc->DepthWrite)
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = DX11RenderEngine::GetD3DComparisonFunc(pDepthStencilDesc->DepthFunc);

		desc.StencilEnable = pDepthStencilDesc->StencilEnable;
		desc.StencilReadMask = pDepthStencilDesc->StencilReadMask;
		desc.StencilWriteMask = pDepthStencilDesc->StencilWriteMask;

		desc.FrontFace.StencilDepthFailOp = DX11RenderEngine::GetD3DStencilOp(pDepthStencilDesc->FrontFaceStencilDepthFailOp);
		desc.FrontFace.StencilFailOp = DX11RenderEngine::GetD3DStencilOp(pDepthStencilDesc->FrontFaceStencilFailOp);
		desc.FrontFace.StencilPassOp = DX11RenderEngine::GetD3DStencilOp(pDepthStencilDesc->FrontFaceStencilPassOp);
		desc.FrontFace.StencilFunc = DX11RenderEngine::GetD3DComparisonFunc(pDepthStencilDesc->FrontFaceStencilFunc);

		desc.BackFace.StencilDepthFailOp = DX11RenderEngine::GetD3DStencilOp(pDepthStencilDesc->BackFaceStencilDepthFailOp);
		desc.BackFace.StencilFailOp = DX11RenderEngine::GetD3DStencilOp(pDepthStencilDesc->BackFaceStencilFailOp);
		desc.BackFace.StencilPassOp = DX11RenderEngine::GetD3DStencilOp(pDepthStencilDesc->BackFaceStencilPassOp);
		desc.BackFace.StencilFunc = DX11RenderEngine::GetD3DComparisonFunc(pDepthStencilDesc->BackFaceStencilFunc);

		ID3D11DepthStencilState *pState;
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		HRESULT hr = renderEngine->D3DDevice()->CreateDepthStencilState(&desc,&pState);
		BOOST_ASSERT(hr==S_OK);

		pDepthStencilState->StencilRef = stencilRef;
		pDepthStencilState->D3DInterface = MakeComPtr<ID3D11DepthStencilState>(pState);

		return DX11DepthStencilStatePtr(pDepthStencilState);

	}


	// ===================================================================
	DX11BlendStatePtr DX11BlendState::Create(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		BOOST_ASSERT(BlendArraySize <= 8 );

		DX11BlendState *pState = new DX11BlendState;

		pState->Desc = *pBlendDescArray;

		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
		desc.IndependentBlendEnable = IndependentBlendEnable;

		for( int index = 0;index< BlendArraySize;index ++ )
		{
			unsigned int TargetIndex = pBlendDescArray[index].TargetIndex ;
			BOOST_ASSERT( TargetIndex < 8 );
			desc.RenderTarget[TargetIndex].BlendEnable = pBlendDescArray[index].BlendEnable;
			desc.RenderTarget[TargetIndex].DestBlend = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].DestBlend);
			desc.RenderTarget[TargetIndex].DestBlendAlpha = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].DestBlendAlpha);
			desc.RenderTarget[TargetIndex].SrcBlend = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].SrcBlend);
			desc.RenderTarget[TargetIndex].SrcBlendAlpha = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].SrcBlendAlpha);
			desc.RenderTarget[TargetIndex].RenderTargetWriteMask = pBlendDescArray[index].RenderTargetWriteMask;
			desc.RenderTarget[TargetIndex].BlendOp = DX11RenderEngine::GetD3DBlendOp(pBlendDescArray[index].BlendOp);
			desc.RenderTarget[TargetIndex].BlendOpAlpha = DX11RenderEngine::GetD3DBlendOp(pBlendDescArray[index].BlendOpAlpha);
		}

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine);
		ID3D11BlendState* pBlendState;
		HRESULT hr = renderEngine->D3DDevice()->CreateBlendState(&desc,&pBlendState);

		BOOST_ASSERT(hr==S_OK);

		pState->D3DInterface = MakeComPtr<ID3D11BlendState>(pBlendState);

		return DX11BlendStatePtr(pState);

	}

	void* DX11BlendState::GetHandle()
	{
		return D3DInterface.get();
	}
}