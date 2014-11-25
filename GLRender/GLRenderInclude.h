#pragma once


// opengl header
#include <GL/glew.h>
#include <GL/wglew.h>



// disorder related
#include "../Engine/EngineInclude.h"



namespace Disorder
{
	class GLSamplerState;
	class GLRasterizeState;
	class GLDepthStencilState;
	class GLBlendState;
	class GLRenderBuffer;
	class GLRenderTexture2D;
	class GLRenderEngine;
	class GLRenderResourceManager;
	class GLShaderObject;
	class GLRenderEffect;
	class GLRenderLayout;
	class GLShaderPropertyManager;
	class GLProgramReflection;
	class GLSurfaceView;
	class GLRenderSurface;
	class GLDebugLayer;
}

#include "GLRenderState.h"
#include "GLRenderResource.h"
#include "GLRenderEffect.h"
#include "GLRenderEngine.h"
#include "GLRenderLayout.h"
#include "GLRenderSurface.h"
#include "GLRenderResourceManager.h"
