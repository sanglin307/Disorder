#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11SamplerState::GetHandle()
	{
		return D3DInterface;
	}

	DX11SamplerState::DX11SamplerState(SamplerDesc* pSamplerDesc)
	{
		Desc = *pSamplerDesc;

		D3D11_SAMPLER_DESC samDesc;
        ZeroMemory( &samDesc, sizeof(samDesc) );

		samDesc.MaxAnisotropy = pSamplerDesc->MaxAnisotropy;
		samDesc.Filter = DX11RenderEngine::GetD3DFilter(pSamplerDesc->Filter, pSamplerDesc->CompareTypeSampler);
		samDesc.AddressU =  DX11RenderEngine::GetD3DAddressMode(pSamplerDesc->AddressU);
		samDesc.AddressV =  DX11RenderEngine::GetD3DAddressMode(pSamplerDesc->AddressV);
		samDesc.AddressW =  DX11RenderEngine::GetD3DAddressMode(pSamplerDesc->AddressW);
		samDesc.BorderColor[0] = pSamplerDesc->BorderColor[0];
		samDesc.BorderColor[1] = pSamplerDesc->BorderColor[1];
		samDesc.BorderColor[2] = pSamplerDesc->BorderColor[2];
		samDesc.BorderColor[3] = pSamplerDesc->BorderColor[3];
		samDesc.ComparisonFunc =  DX11RenderEngine::GetD3DComparisonFunc(pSamplerDesc->CompareFunc);
		samDesc.MaxLOD = pSamplerDesc->MaxLOD;
		samDesc.MinLOD = pSamplerDesc->MinLOD;
		samDesc.MipLODBias = pSamplerDesc->MipLODBias;

        DX11RenderEngine* renderEngine = (DX11RenderEngine*)GRenderEngine; 

		ID3D11SamplerState *pSamplerState;

		HRESULT hr = renderEngine->D3DDevice()->CreateSamplerState( &samDesc, &pSamplerState );
		BOOST_ASSERT(hr == S_OK);

		D3DInterface = pSamplerState;

	}

	//======================================================================

	void* DX11RasterizeState::GetHandle()
	{
		return D3DInterface;
	}

	DX11RasterizeState::DX11RasterizeState(RasterizeDesc *pDesc)
	{
		Desc = *pDesc;

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
		desc.SlopeScaledDepthBias = pDesc->SlopeScaledDepthBias;

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

		ID3D11RasterizerState *pState;
		DX11RenderEngine* renderEngine = (DX11RenderEngine*)GRenderEngine; 
		HRESULT hr = renderEngine->D3DDevice()->CreateRasterizerState(&desc,&pState);
		BOOST_ASSERT(hr==S_OK);

		D3DInterface = pState;
	}
	//==================================================================

	void* DX11DepthStencilState::GetHandle()
	{
		return D3DInterface;
	}

	DX11DepthStencilState::DX11DepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		Desc = *pDepthStencilDesc;

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
		DX11RenderEngine* renderEngine = (DX11RenderEngine*)GRenderEngine; 
		HRESULT hr = renderEngine->D3DDevice()->CreateDepthStencilState(&desc,&pState);
		BOOST_ASSERT(hr==S_OK);

		StencilRef = stencilRef;
		D3DInterface = pState;
	}


	// ===================================================================
	DX11BlendState::DX11BlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		BOOST_ASSERT(BlendArraySize <= 8 );
 
		AlphaToCoverageEnable = AlphaToCoverageEnable;
		IndependentBlendEnable = IndependentBlendEnable;

		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc,sizeof(desc));

		desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
		desc.IndependentBlendEnable = IndependentBlendEnable;

		for( int index = 0; index < BlendArraySize; index ++ )
		{
			Desc[index] = pBlendDescArray[index];
			desc.RenderTarget[index].BlendEnable = pBlendDescArray[index].BlendEnable;
			desc.RenderTarget[index].DestBlend = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].DestBlend);
			desc.RenderTarget[index].DestBlendAlpha = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].DestBlendAlpha);
			desc.RenderTarget[index].SrcBlend = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].SrcBlend);
			desc.RenderTarget[index].SrcBlendAlpha = DX11RenderEngine::GetD3DBlendDesc(pBlendDescArray[index].SrcBlendAlpha);
			desc.RenderTarget[index].RenderTargetWriteMask = pBlendDescArray[index].RenderTargetWriteMask;
			desc.RenderTarget[index].BlendOp = DX11RenderEngine::GetD3DBlendOp(pBlendDescArray[index].BlendOp);
			desc.RenderTarget[index].BlendOpAlpha = DX11RenderEngine::GetD3DBlendOp(pBlendDescArray[index].BlendOpAlpha);
		}

		DX11RenderEngine* renderEngine = (DX11RenderEngine*)GRenderEngine;
		ID3D11BlendState* pBlendState;
		HRESULT hr = renderEngine->D3DDevice()->CreateBlendState(&desc,&pBlendState);

		BOOST_ASSERT(hr==S_OK);

		D3DInterface = pBlendState;
	}

	void* DX11BlendState::GetHandle()
	{
		return D3DInterface;
	}
}