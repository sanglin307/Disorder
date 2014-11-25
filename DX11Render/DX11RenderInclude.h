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
	class DX11RenderEngine;
	class DX11RenderLayout;
	class DX11RenderBuffer;
	class DX11RenderTexture2D;
	class DX11RenderResourceManager;
	class DX11SurfaceView;
	class DX11RenderSurface;
	class DX11ShaderObject;
	class DX11BlendState;
	class DX11RasterizeState;
	class DX11DepthStencilState;
	class DX11ShaderReflection;	 
	class DX11RenderEffect;	 
	class DX11ShaderPropertyManager;
	 
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
