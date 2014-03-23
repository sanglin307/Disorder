#pragma once


// opengl header
#include <GL/glew.h>
#include <GL/wglew.h>



// disorder related
#include "../Engine/EngineInclude.h"



namespace Disorder
{
	class GLSamplerState;
	typedef boost::shared_ptr<GLSamplerState> GLSamplerStatePtr;
	class GLRenderBuffer;
	typedef boost::shared_ptr<GLRenderBuffer> GLRenderBufferPtr;
	class GLRenderTexture2D;
	typedef boost::shared_ptr<GLRenderTexture2D> GLRenderTexture2DPtr;
	class GLRenderEngine;
	typedef boost::shared_ptr<GLRenderEngine> GLRenderEnginePtr;
	class GLRenderResourceManager;
	typedef boost::shared_ptr<GLRenderResourceManager> GLRenderResourceManagerPtr;
	class GLShaderObject;
	typedef boost::shared_ptr<GLShaderObject> GLShaderObjectPtr;
	class GLRenderEffect;
	typedef boost::shared_ptr<GLRenderEffect> GLRenderEffectPtr;
	class GLRenderLayout;
	typedef boost::shared_ptr<GLRenderLayout> GLRenderLayoutPtr;
	class GLShaderPropertyManager;
	typedef boost::shared_ptr<GLShaderPropertyManager> GLShaderPropertyManagerPtr;
	class GLProgramReflection;
	typedef boost::shared_ptr<GLProgramReflection> GLProgramReflectionPtr;
	class GLRenderSurface;
	typedef boost::shared_ptr<GLRenderSurface> GLRenderSurfacePtr;
	class GLRenderGBuffer;
	typedef boost::shared_ptr<GLRenderGBuffer> GLRenderGBufferPtr;
	class GLRenderSurfaceCache;
	typedef boost::shared_ptr<GLRenderSurfaceCache> GLRenderSurfaceCachePtr;

}

#include "GLRenderState.h"
#include "GLRenderResource.h"
#include "GLRenderEffect.h"
#include "GLRenderEngine.h"
#include "GLRenderLayout.h"
#include "GLRenderSurface.h"
#include "GLRenderResourceManager.h"
