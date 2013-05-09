#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11SamplerState::GetLowInterface()
	{
		return D3DInterface.get();
	}

	bool DX11SamplerState::Create(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy)
	{
		_filter = filter;
		_AddressU = _AddressV = _AddressW = addressUVW;
		_maxAnisotropy = maxAnisotropy;

		D3D11_SAMPLER_DESC samDesc;
        ZeroMemory( &samDesc, sizeof(samDesc) );

		samDesc.MaxAnisotropy = maxAnisotropy;
		if( _filter == SF_Point )
			samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		else if( _filter == SF_Linear )
			samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		else if( _filter == SF_Anisotropic )
		{
			samDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samDesc.MaxAnisotropy = maxAnisotropy;
		}
 
		if( addressUVW == TAM_Wrap )
		{
			samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		}
		else if( addressUVW == TAM_Mirror )
		{
			samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		else if( addressUVW == TAM_Clamp )
		{
			samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else if( addressUVW == TAM_Border )
		{
			samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		}

        samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samDesc.MaxLOD = D3D11_FLOAT32_MAX;
        DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 

		ID3D11SamplerState *pSamplerState;

		HRESULT hr = renderEngine->D3DDevice()->CreateSamplerState( &samDesc, &pSamplerState );
		BOOST_ASSERT(hr==S_OK);

		D3DInterface = MakeComPtr<ID3D11SamplerState>(pSamplerState);

		return true;

	}

	//======================================================================

	void* DX11RasterizeState::GetLowInterface()
	{
		return D3DInterface.get();
	}

	bool DX11RasterizeState::Create(RasterizeDesc *pDesc)
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.AntialiasedLineEnable = pDesc->AntialiasedLineEnable;
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
		desc.ScissorEnable = pDesc->ScissorEnable;
		desc.SlopeScaledDepthBias = pDesc->SlopeScaledDepthBias;

		ID3D11RasterizerState *pState;
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		HRESULT hr = renderEngine->D3DDevice()->CreateRasterizerState(&desc,&pState);
		BOOST_ASSERT(hr==S_OK);

		D3DInterface = MakeComPtr<ID3D11RasterizerState>(pState);


		return true;

	}
	//==================================================================

	void* DX11DepthStencilState::GetLowInterface()
	{
		return D3DInterface.get();
	}

	bool DX11DepthStencilState::Create(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc,sizeof(desc));

		desc.DepthEnable = pDepthStencilDesc->DepthEnable;
		if(pDepthStencilDesc->DepthWrite)
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = GetD3DComparisonFunc(pDepthStencilDesc->DepthFunc);

		desc.StencilEnable = pDepthStencilDesc->StencilEnable;
		desc.StencilReadMask = pDepthStencilDesc->StencilReadMask;
		desc.StencilWriteMask = pDepthStencilDesc->StencilWriteMask;

		desc.FrontFace.StencilDepthFailOp = GetD3DStencilOp(pDepthStencilDesc->FrontFaceStencilDepthFailOp);
		desc.FrontFace.StencilFailOp = GetD3DStencilOp(pDepthStencilDesc->FrontFaceStencilFailOp);
		desc.FrontFace.StencilPassOp = GetD3DStencilOp(pDepthStencilDesc->FrontFaceStencilPassOp);
		desc.FrontFace.StencilFunc = GetD3DComparisonFunc(pDepthStencilDesc->FrontFaceStencilFunc);

		desc.BackFace.StencilDepthFailOp = GetD3DStencilOp(pDepthStencilDesc->BackFaceStencilDepthFailOp);
		desc.BackFace.StencilFailOp = GetD3DStencilOp(pDepthStencilDesc->BackFaceStencilFailOp);
		desc.BackFace.StencilPassOp = GetD3DStencilOp(pDepthStencilDesc->BackFaceStencilPassOp);
		desc.BackFace.StencilFunc = GetD3DComparisonFunc(pDepthStencilDesc->BackFaceStencilFunc);

		ID3D11DepthStencilState *pState;
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		HRESULT hr = renderEngine->D3DDevice()->CreateDepthStencilState(&desc,&pState);
		BOOST_ASSERT(hr==S_OK);

		StencilRef = stencilRef;
		D3DInterface = MakeComPtr<ID3D11DepthStencilState>(pState);

		return true;

	}


	// ===================================================================
	bool DX11BlendState::Create(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		BOOST_ASSERT(BlendArraySize <= 8 );

		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
		desc.IndependentBlendEnable = IndependentBlendEnable;

		for( int index = 0;index< BlendArraySize;index ++ )
		{
			unsigned int TargetIndex = pBlendDescArray[index].TargetIndex ;
			BOOST_ASSERT( TargetIndex < 8 );
			desc.RenderTarget[TargetIndex].BlendEnable = pBlendDescArray[index].BlendEnable;
			desc.RenderTarget[TargetIndex].DestBlend = GetD3DBlendDesc(pBlendDescArray[index].DestBlend);
			desc.RenderTarget[TargetIndex].DestBlendAlpha = GetD3DBlendDesc(pBlendDescArray[index].DestBlendAlpha);
			desc.RenderTarget[TargetIndex].SrcBlend = GetD3DBlendDesc(pBlendDescArray[index].SrcBlend);
			desc.RenderTarget[TargetIndex].SrcBlendAlpha = GetD3DBlendDesc(pBlendDescArray[index].SrcBlendAlpha);
			desc.RenderTarget[TargetIndex].RenderTargetWriteMask = pBlendDescArray[index].RenderTargetWriteMask;
			desc.RenderTarget[TargetIndex].BlendOp = GetD3DBlendOp(pBlendDescArray[index].BlendOp);
			desc.RenderTarget[TargetIndex].BlendOpAlpha = GetD3DBlendOp(pBlendDescArray[index].BlendOpAlpha);
		}

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine);
		ID3D11BlendState* pState;
		HRESULT hr = renderEngine->D3DDevice()->CreateBlendState(&desc,&pState);

		BOOST_ASSERT(hr==S_OK);

		D3DInterface = MakeComPtr<ID3D11BlendState>(pState);

		return true;

	}

	void* DX11BlendState::GetLowInterface()
	{
		return D3DInterface.get();
	}
}