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

}

#include "GLRenderEffect.h"
#include "GLRenderEngine.h"
#include "GLRenderResourceManager.h"
