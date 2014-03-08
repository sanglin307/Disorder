#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


// opengl header
#include <GL/glew.h>
#include <GL/wglew.h>



// disorder related
#include "../Engine/EngineInclude.h"



namespace Disorder
{

	class GLRenderEngine;
	typedef boost::shared_ptr<GLRenderEngine> GLRenderEnginePtr;



}

#include "GLRenderEngine.h"
