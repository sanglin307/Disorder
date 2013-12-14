// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


//system related
#include <D3D11.h>
#include <D3Dcompiler.h>



#include "../Engine/EngineInclude.h"

 

namespace Disorder
{
	class DX11SamplerState;
	typedef boost::shared_ptr<DX11SamplerState> DX11SamplerStatePtr;
	class DX11RenderEngine;
	typedef boost::shared_ptr<DX11RenderEngine> DX11RenderEnginePtr;
	class DX11RenderLayout;
	typedef boost::shared_ptr<DX11RenderLayout> DX11RenderLayoutPtr;
	class DX11RenderBuffer;
	typedef boost::shared_ptr<DX11RenderBuffer> DX11RenderBufferPtr;
	class DX11RenderTexture2D;
	typedef boost::shared_ptr<DX11RenderTexture2D> DX11RenderTexture2DPtr;
	class DX11RenderResourceManager;
	typedef boost::shared_ptr<DX11RenderResourceManager> DX11RenderResourceManagerPtr;
	class DX11RenderSurface;
	typedef boost::shared_ptr<DX11RenderSurface> DX11RenderSurfacePtr;
	class DX11ShaderObject;
	typedef boost::shared_ptr<DX11ShaderObject> DX11ShaderObjectPtr;
	class DX11BlendState;
	typedef boost::shared_ptr<DX11BlendState> DX11BlendStatePtr;
	class DX11RasterizeState;
	typedef boost::shared_ptr<DX11RasterizeState> DX11RasterizeStatePtr;
	class DX11DepthStencilState;
	typedef boost::shared_ptr<DX11DepthStencilState> DX11DepthStencilStatePtr;
	class DX11ShaderReflection;
	typedef boost::shared_ptr<DX11ShaderReflection> DX11ShaderReflectionPtr;
	class DX11RenderEffect;
	typedef boost::shared_ptr<DX11RenderEffect> DX11RenderEffectPtr;
	class DX11ShaderPropertyManager;
	typedef boost::shared_ptr<DX11ShaderPropertyManager> DX11ShaderPropertyManagerPtr;

	typedef boost::shared_ptr<IDXGIFactory> IDXGIFactoryPtr;
	typedef boost::shared_ptr<IDXGIAdapter> IDXGIAdapterPtr;
	typedef boost::shared_ptr<ID3D11Device> ID3D11DevicePtr;
	typedef boost::shared_ptr<ID3D11DeviceContext> ID3D11DeviceContextPtr;
	typedef boost::shared_ptr<IDXGISwapChain>  IDXGISwapChainPtr;
	typedef boost::shared_ptr<ID3D11RenderTargetView> ID3D11RenderTargetViewPtr;
	typedef boost::shared_ptr<ID3D11InputLayout> ID3D11InputLayoutPtr;
	typedef boost::shared_ptr<ID3D11Buffer> ID3D11BufferPtr;
	typedef boost::shared_ptr<ID3D11VertexShader> ID3D11VertexShaderPtr;
	typedef boost::shared_ptr<ID3D11PixelShader>  ID3D11PixelShaderPtr;
	typedef boost::shared_ptr<ID3DBlob> ID3DBlobPtr;
	typedef boost::shared_ptr<ID3D11DepthStencilView> ID3D11DepthStencilViewPtr;
	typedef boost::shared_ptr<ID3D11ShaderResourceView> ID3D11ShaderResourceViewPtr;
	typedef boost::shared_ptr<ID3D11SamplerState> ID3D11SamplerStatePtr;
	typedef boost::shared_ptr<ID3D11Texture2D> ID3D11Texture2DPtr;
	typedef boost::shared_ptr<ID3D11RasterizerState> ID3D11RasterizerStatePtr;
	typedef boost::shared_ptr<ID3D11BlendState> ID3D11BlendStatePtr;
	typedef boost::shared_ptr<ID3D11DepthStencilState> ID3D11DepthStencilStatePtr;



}


//project related
#include "DX11RenderState.h"
#include "DX11RenderResource.h"
#include "DX11RenderEffect.h"
#include "DX11RenderSurface.h"
#include "DX11RenderLayout.h"
#include "DX11RenderEngine.h"
#include "DX11RenderResourceManager.h"


// TODO: reference additional headers your program requires here
