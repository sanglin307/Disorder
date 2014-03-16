#pragma once


// opengl header
#include <GL/glew.h>
#include <GL/wglew.h>



// disorder related
#include "../Engine/EngineInclude.h"



namespace Disorder
{

	class GLRenderEngine;
	typedef boost::shared_ptr<GLRenderEngine> GLRenderEnginePtr;
	class GLRenderResourceManager;
	typedef boost::shared_ptr<GLRenderResourceManager> GLRenderResourceManagerPtr;
	class GLShaderObject;
	typedef boost::shared_ptr<GLShaderObject> GLShaderObjectPtr;
	class GLRenderEffect;
	typedef boost::shared_ptr<GLRenderEffect> GLRenderEffectPtr;
	class GLShaderPropertyManager;
	typedef boost::shared_ptr<GLShaderPropertyManager> GLShaderPropertyManagerPtr;
	class GLProgramReflection;
	typedef boost::shared_ptr<GLProgramReflection> GLProgramReflectionPtr;

}

#include "GLRenderEffect.h"
#include "GLRenderEngine.h"
#include "GLRenderResourceManager.h"
