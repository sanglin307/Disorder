#include "GLRenderInclude.h"


namespace Disorder
{
	GLDebugLayerPtr GLDebugLayer::Create()
	{
		GLDebugLayer *pLayer = new GLDebugLayer;
		return GLDebugLayerPtr(pLayer);
	}

	void GLDebugLayer::Init()
	{
		GLRenderEnginePtr engine = boost::dynamic_pointer_cast<GLRenderEngine>(GEngine->RenderEngine);
		if (engine->IsVersionSupported(4, 3))
		{
			glDebugMessageCallback(DebugCallback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_API, "API"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_APPLICATION, "Application"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_OTHER, "Other"));

			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_ERROR, "Error"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behavior"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behavior"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_PORTABILITY, "Portability"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_PERFORMANCE, "Performance"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_OTHER, "Other"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_MARKER, "Maker"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_PUSH_GROUP, "Push Group"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_POP_GROUP, "Pop Group"));

			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_LOW, "Low"));
			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_MEDIUM, "Medium"));
			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_HIGH, "High"));
			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_NOTIFICATION, "Notification"));
		}
		else if (engine->IsExtensionSupported("GL_ARB_debug_output"))
		{
			glDebugMessageCallbackARB(DebugCallback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_API_ARB, "API"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB, "Window System"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_SHADER_COMPILER_ARB, "Shader Compiler"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, "Third Party"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_APPLICATION_ARB, "Application"));
			_mapSourceInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SOURCE_OTHER_ARB, "Other"));

			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_ERROR_ARB, "Error"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB, "Deprecated Behavior"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB, "Undefined Behavior"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_PORTABILITY_ARB, "Portability"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_PERFORMANCE_ARB, "Performance"));
			_mapTypeInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_TYPE_OTHER_ARB, "Other"));

			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_LOW_ARB, "Low"));
			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_MEDIUM_ARB, "Medium"));
			_mapSeverityInfo.insert(std::make_pair<GLenum, std::string>(GL_DEBUG_SEVERITY_HIGH_ARB, "High"));
		
		}
	}

	void GLDebugLayer::OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message)
	{
		std::stringstream stream;

		stream << "OpenGL Debug Message -- Source: " << _mapSourceInfo[source] << "  Type: " << _mapTypeInfo[type] << "  Severity: " << _mapSeverityInfo[severity];
		stream << " ID: " << id;
		stream << " Message: " << message;

		if (type == GL_DEBUG_TYPE_ERROR)
		{
			GLogger->Error(stream.str());
		}
		else
		{
			GLogger->Warning(stream.str());
		}

	}

	void GLDebugLayer::Exit()
	{

	}

	GLRenderEnginePtr GLRenderEngine::Create()
	{
		GLRenderEngine *pEngine = new GLRenderEngine;
		return GLRenderEnginePtr(pEngine);
	}

	GLenum GLRenderEngine::GetPixelFormat(PixelFormat format,GLenum &glFormat,GLenum &glType)
	{
		//glFormat include : GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT, and GL_STENCIL_INDEX.
		//glType include : GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT, 
		//GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, 
		//GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, 
		//GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV

		// the following is not all right, please check it when occure error!

		switch (format)
		{
		case PF_R32G32B32A32_TYPELESS:
		case PF_R32G32B32A32_FLOAT:
			glFormat = GL_RGBA;
			glType = GL_FLOAT;
			return GL_RGBA32F;
		case PF_R32G32B32A32_UINT:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_INT;
			return GL_RGBA32UI;
		case PF_R32G32B32A32_SINT:
			glFormat = GL_RGBA;
			glType = GL_INT;
			return GL_RGBA32I;
		case PF_R32G32B32_TYPELESS:
		case PF_R32G32B32_FLOAT:
			glFormat = GL_RGB;
			glType = GL_FLOAT;
			return GL_RGB32F;
		case PF_R32G32B32_UINT:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_INT;
			return GL_RGB16UI;
		case PF_R32G32B32_SINT:
			glFormat = GL_RGB;
			glType = GL_INT;
			return GL_RGB16I;
		case PF_R16G16B16A16_TYPELESS:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGBA16;
		case PF_R16G16B16A16_FLOAT:
		case PF_R16G16B16A16_UNORM:
		case PF_R16G16B16A16_SNORM:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGBA16F;
		case PF_R16G16B16A16_UINT:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGBA16UI;
		case PF_R16G16B16A16_SINT:
			glFormat = GL_RGBA;
			glType = GL_SHORT;
			return GL_RGBA16I;
		case PF_R32G32_TYPELESS:
		case PF_R32G32_FLOAT:
			glFormat = GL_RG;
			glType = GL_FLOAT;
			return GL_RG32F;
		case PF_R32G32_UINT:
			return GL_RG32UI;
			glFormat = GL_RG;
			glType = GL_UNSIGNED_INT;
		case PF_R32G32_SINT:
			glFormat = GL_RG;
			glType = GL_INT;
			return GL_RG32I;
		case PF_R32G8X24_TYPELESS:
		case PF_D32_FLOAT_S8X24_UINT:
		case PF_R32_FLOAT_X8X24_TYPELESS:
		case PF_X32_TYPELESS_G8X24_UINT:
			glFormat = GL_DEPTH_COMPONENT;
			glType = GL_FLOAT;
			return GL_DEPTH32F_STENCIL8;
		case PF_R10G10B10A2_TYPELESS:
		case PF_R10G10B10A2_UNORM:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_INT_10_10_10_2;
			return GL_RGB10_A2;
		case PF_R10G10B10A2_UINT:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_INT_10_10_10_2;
			return GL_RGB10_A2UI;
		case PF_R11G11B10_FLOAT:
			glFormat = GL_RGB;
			glType = GL_FLOAT;
			return GL_R11F_G11F_B10F;
		case PF_R8G8B8A8_TYPELESS:
		case PF_R8G8B8A8_UNORM:
		case PF_R8G8B8A8_UNORM_SRGB:
		case PF_R8G8B8A8_SNORM:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_INT_8_8_8_8;
			return GL_RGBA8;
		case PF_R8G8B8A8_UINT:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_INT_8_8_8_8;
			return GL_RGBA8UI;
		case PF_R8G8B8A8_SINT:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_INT_8_8_8_8;
			return GL_RGBA8I;
		case PF_R16G16_TYPELESS:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_INT_8_8_8_8;
			return GL_RG16;
		case PF_R16G16_FLOAT:
		case PF_R16G16_UNORM:
		case PF_R16G16_SNORM:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_SHORT;		
			return GL_RGB16F;
		case PF_R16G16_UINT:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_SHORT;
			return GL_RG16UI;
		case PF_R16G16_SINT:
			glFormat = GL_RG;
			glType = GL_SHORT;
			return GL_RG16I;
		case PF_D32_FLOAT:
			glFormat = GL_DEPTH_COMPONENT;
			glType = GL_FLOAT;
			return GL_R32F;
		case PF_R32_TYPELESS:
		case PF_R32_FLOAT:
			glFormat = GL_R;
			glType = GL_FLOAT;
			return GL_R32F;	
		case PF_R32_UINT:
			glFormat = GL_R;
			glType = GL_UNSIGNED_INT;
			return GL_R32UI;
		case PF_R32_SINT :
			glFormat = GL_R;
			glType = GL_INT;
			return GL_R32I;
		case PF_R24G8_TYPELESS:		
		case PF_D24_UNORM_S8_UINT:
		case PF_R24_UNORM_X8_TYPELESS:
		case PF_X24_TYPELESS_G8_UINT:
			glFormat = GL_DEPTH_COMPONENT;
			glType = GL_UNSIGNED_INT;
			return GL_DEPTH24_STENCIL8;
		case PF_R8G8_TYPELESS:
		case PF_R8G8_UNORM:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_BYTE;
			return GL_RG8;
		case PF_R8G8_UINT:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_BYTE;
			return GL_RG8UI;
		case PF_R8G8_SNORM:
			glFormat = GL_RG;
			glType = GL_BYTE;
			return GL_RG8_SNORM;
		case PF_R8G8_SINT:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_BYTE;
			return GL_RG8I;
		case PF_R16_TYPELESS:
			glFormat = GL_R;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16;
		case PF_R16_FLOAT:
		case PF_R16_UNORM:
			glFormat = GL_R;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16F;
		case PF_D16_UNORM:
			glFormat = GL_DEPTH_COMPONENT;
			glType = GL_UNSIGNED_SHORT;
			return GL_DEPTH_COMPONENT16;
		case PF_R16_UINT:
			glFormat = GL_R;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16UI;
		case PF_R16_SNORM:
			glFormat = GL_R;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16_SNORM;
		case PF_R16_SINT:
			glFormat = GL_R;
			glType = GL_SHORT;
			return GL_R16I;
		case PF_R8_TYPELESS:
			glFormat = GL_R;
			glType = GL_UNSIGNED_BYTE;
			return GL_R8;
		case PF_R8_UNORM:
		case PF_R8_UINT:
			glFormat = GL_R;
			glType = GL_UNSIGNED_BYTE;
			return GL_R8UI;
		case PF_R8_SNORM:
			glFormat = GL_R;
			glType = GL_BYTE;
			return GL_R8_SNORM;
		case PF_R8_SINT:
			glFormat = GL_R;
			glType = GL_BYTE;
			return GL_R8I;
		case PF_R9G9B9E5_SHAREDEXP:
			glFormat = GL_R;
			glType = GL_UNSIGNED_INT;
			return GL_RGB9_E5;  
		default:
			BOOST_ASSERT(0);
			return 0;
		}
	}

	GLint GLRenderEngine::GetGLAddressMode(TextureAddressMode addrMode)
	{
		switch (addrMode)
		{
		case TAM_Wrap:
			return GL_REPEAT;
		case TAM_Mirror:
			return GL_MIRRORED_REPEAT;
		case TAM_Clamp:
			return GL_CLAMP_TO_EDGE;
		case TAM_Border:
			return GL_CLAMP_TO_BORDER;
		default:
			BOOST_ASSERT(0);
		}

		return 0;
	}

	GLint GLRenderEngine::GetGLComparisonFunc(ComparisonFunc func)
	{
		switch (func)
		{
		case CF_None:
		case CF_Never:
			return GL_NEVER;
		case CF_Less:
			return GL_LESS;
		case CF_Equal:
			return GL_EQUAL;
		case CF_Less_Equal:
			return GL_LEQUAL;
		case CF_Greater:
			return GL_GREATER;
		case CF_Not_Equal:
			return GL_NOTEQUAL;
		case CF_Greater_Equal:
			return GL_GEQUAL;
		case CF_Always:
			return GL_ALWAYS;
		default:
			BOOST_ASSERT(0);
		}

		return GL_NEVER;
	}

	GLRenderEngine::GLRenderEngine()
	{
		_hRC = 0;	
	}

	void GLRenderEngine::CreateViewport(void *hWnd)
	{
		if (_hRC)
			return;

		if (!CreateGLContext((HWND)hWnd))
		{
			GLogger->Error("Can't create windows OpenGL Context!");
			return;
		}

		LoadShaderIncludeFiles();
		LoadGLExtensions();

		_debugLayer = GLDebugLayer::Create();
		_debugLayer->Init();
	}

	void GLRenderEngine::LoadGLExtensions()
	{
		GLint numExtensions;
		GLint i;

		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		for (i = 0; i < numExtensions; i++)
		{
			const char* e = (const char*)glGetStringi(GL_EXTENSIONS, i);
			_vGLExtensions.push_back(e);
		}
	}

	bool GLRenderEngine::IsExtensionSupported(std::string const& extension)
	{
		if (_vGLExtensions.size() == 0)
			return false;

		for (size_t i = 0; i < _vGLExtensions.size(); i++)
		{
			if (!stricmp(extension.c_str(),_vGLExtensions[i].c_str()))
				return true;
		}

		return false;
	}

	void GLRenderEngine::LoadShaderIncludeFiles()
	{
		std::string shaderPath = GConfig->sResourceFXPath + "GLSL\\";
		boost::filesystem::path p(shaderPath);
		if (!boost::filesystem::exists(p) || !boost::filesystem::is_directory(p) )
		{
			GLogger->Error("shader's path not exist!");
			return;
		}

		boost::filesystem::directory_iterator enditer;
		for (boost::filesystem::directory_iterator fileiter(p); fileiter != enditer; ++fileiter)
		{
			if (!boost::filesystem::is_directory(*fileiter) && (boost::filesystem::extension(*fileiter) == ".gls"))
			{
				std::string path = fileiter->path().string();
				std::string filekey = "/glsl/" + fileiter->path().leaf().string();
				FileObjectPtr file = GEngine->FileManager->OpenFile(path,"rt"); 
				std::string content = GEngine->FileManager->ReadFile(file);
				if (ShaderObject::sMapIncludeFiles.find(filekey) == ShaderObject::sMapIncludeFiles.end())
				{
					ShaderObject::sMapIncludeFiles.insert(std::pair<std::string, std::string>(filekey, content));
				}
			}
		}

		if (ShaderObject::sMapIncludeFiles.size() > 0)
		{
			std::map<std::string, std::string>::const_iterator iter = ShaderObject::sMapIncludeFiles.begin();
			while (iter != ShaderObject::sMapIncludeFiles.end())
			{
				glNamedStringARB(GL_SHADER_INCLUDE_ARB, iter->first.size(), iter->first.c_str(), iter->second.size(), iter->second.c_str());				 
				++iter;
				
			}
		}
	}

	 
	bool GLRenderEngine::CreateGLContext(HWND window)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		
		TCHAR winClassName[] = L"tempWin";
		WNDCLASSEXW Cls;
		memset(&Cls, 0, sizeof(Cls));
		Cls.cbSize = sizeof(Cls);
		Cls.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		Cls.lpfnWndProc = DefWindowProc;
		Cls.hInstance = GAppInstance;
		Cls.lpszClassName = winClassName;
		Cls.lpszMenuName = NULL;

		// temp windows
		if (!RegisterClassExW(&Cls))
		{
			DWORD result = GetLastError();
			std::stringstream str;
			str << "Window Class registe Error and code is:" << result;
			GLogger->Error(str.str());
			return false;
		}

		DWORD WindowStyle;
		WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
 
		// Create the window
		HWND htempWin = CreateWindowW(winClassName, winClassName, WindowStyle, 0, 0, 640, 480, NULL, NULL, GAppInstance, NULL);
		DWORD errr = GetLastError();
		_hDC = GetDC(htempWin);
		int nPixelFormat = ChoosePixelFormat(_hDC, &pfd);

		if (nPixelFormat == 0) 
			return false;

		BOOL bResult = SetPixelFormat(_hDC, nPixelFormat, &pfd);

		if (!bResult) 
			return false;

		HGLRC tempContext = wglCreateContext(_hDC);
		wglMakeCurrent(_hDC, tempContext);
 
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			MessageBox(NULL, L"GLEW is not initialized!", L"GLRenderInit", 0);
			return false;
		}

		glGetIntegerv(GL_MAJOR_VERSION, &_mainVersion);
		glGetIntegerv(GL_MINOR_VERSION, &_subVersion);

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		DestroyWindow(htempWin);
		UnregisterClass(winClassName, GAppInstance);

		_hDC = GetDC(window);
		unsigned int nPixCount = 0;
		int colorBit = 24;
		int alphaBit = 8;
		int depthBit = 24;
		int stencilBit = 8;
		RenderEngine::ComputePixelColorAlphaSize(GConfig->pRenderConfig->ColorFormat, colorBit, alphaBit);
		RenderEngine::ComputeDepthStencilSize(GConfig->pRenderConfig->DepthStencilFormat,depthBit,stencilBit);

		float fPixAttribs[] = { 0, 0 };
		std::vector<int> pixAttribs;
		pixAttribs.push_back(WGL_SUPPORT_OPENGL_ARB); pixAttribs.push_back(1);
		pixAttribs.push_back(WGL_DRAW_TO_WINDOW_ARB); pixAttribs.push_back(1);
		pixAttribs.push_back(WGL_ACCELERATION_ARB); pixAttribs.push_back(WGL_FULL_ACCELERATION_ARB);
		pixAttribs.push_back(WGL_COLOR_BITS_ARB);  pixAttribs.push_back(colorBit);
		pixAttribs.push_back(WGL_ALPHA_BITS_ARB);  pixAttribs.push_back(alphaBit);
		pixAttribs.push_back(WGL_DEPTH_BITS_ARB);  pixAttribs.push_back(depthBit);
		pixAttribs.push_back(WGL_STENCIL_BITS_ARB); pixAttribs.push_back(stencilBit);
		pixAttribs.push_back(WGL_DOUBLE_BUFFER_ARB); pixAttribs.push_back(GL_TRUE);
		pixAttribs.push_back(WGL_PIXEL_TYPE_ARB); pixAttribs.push_back(WGL_TYPE_RGBA_ARB);
		if (GConfig->pRenderConfig->MultiSampleCount > 1)
		{
			pixAttribs.push_back(WGL_SAMPLE_BUFFERS_ARB); pixAttribs.push_back(GL_TRUE);
			pixAttribs.push_back(WGL_SAMPLES_ARB); pixAttribs.push_back(GConfig->pRenderConfig->MultiSampleCount);
		}
		pixAttribs.push_back(0);

		int result = wglChoosePixelFormatARB(_hDC, pixAttribs.data(), fPixAttribs, 1, &nPixelFormat, &nPixCount);
		if (!result || nPixCount <= 0)
		{
			while ((!result || nPixCount <= 0) && GConfig->pRenderConfig->MultiSampleCount > 1)
			{
				GConfig->pRenderConfig->MultiSampleCount--;
				for (size_t t = 0; t < pixAttribs.size(); t++)
				{
					if (pixAttribs[t] == WGL_SAMPLES_ARB)
					{
						pixAttribs[t + 1] = GConfig->pRenderConfig->MultiSampleCount;
						result = wglChoosePixelFormatARB(_hDC, pixAttribs.data(), fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
						break;
					}
				}
			}

			if (!result || nPixCount <= 0)
			{
			   GLogger->Error("Can't find right pixel format for openGL,please check render setting!");
			   return false;
		    }
		}

		// Got a format, now set it as the current one  
		if (!SetPixelFormat(_hDC, nPixelFormat, &pfd))
		{
			return false;
		}

		

		std::vector<int> attrib;
		attrib.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB); attrib.push_back(_mainVersion);
		attrib.push_back(WGL_CONTEXT_MINOR_VERSION_ARB); attrib.push_back(_subVersion);
		attrib.push_back(WGL_CONTEXT_PROFILE_MASK_ARB); attrib.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
#ifdef _DEBUG
		attrib.push_back(WGL_CONTEXT_FLAGS_ARB); attrib.push_back(WGL_CONTEXT_DEBUG_BIT_ARB | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);
#else
		attrib.push_back(WGL_CONTEXT_FLAGS_ARB); attrib.push_back( WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);
#endif
		attrib.push_back(0);

		if (wglewIsSupported("WGL_ARB_create_context") == 1)
		{
			_hRC = wglCreateContextAttribsARB(_hDC, 0, attrib.data());
			if (!_hRC)
				return false;
			wglMakeCurrent(_hDC, _hRC);
		}
		else
		{	
			BOOST_ASSERT(0);
			return false;
		}
		
		//v-syn
		wglSwapIntervalEXT(GConfig->pRenderConfig->SyncInterval);

	
		//get the version number
		glGetIntegerv(GL_MAJOR_VERSION, &_mainVersion);
		glGetIntegerv(GL_MINOR_VERSION, &_subVersion);

		glViewport(0, 0, GConfig->pRenderConfig->SizeX, GConfig->pRenderConfig->SizeY);
	

		return true;
	}


	void GLRenderEngine::Init()
	{
		
	}
	
	void GLRenderEngine::Exit()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_hRC);
	}

	void GLRenderEngine::OnDrawBegin()
	{
		GDrawTriNumber = 0;
	}
	void GLRenderEngine::OnDrawEnd()
	{
		SwapBuffers(_hDC);
	}

	void GLRenderEngine::AdjustProjMatrix(const glm::mat4 &matrix)
	{

	}
 
	void GLRenderEngine::ClearRenderSurface(const RenderSurfacePtr& renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil)
	{

	}

	void GLRenderEngine::ClearRenderTarget(const SurfaceViewPtr& renderTarget, const glm::vec4& color)
	{
		/*GLuint fbo = (GLuint)renderTarget->GetHandle(SL_RenderTarget1);
		_renderCache.CacheFrameBufferObject(fbo);

		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));*/

	}

	void GLRenderEngine::ClearDepthStencil(const SurfaceViewPtr& depthBuffer, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil)
	{
		/*if (!bClearDepth && bClearStencil)
			return;

		GLuint fbo = (GLuint)depthBuffer->GetHandle(SL_DepthStencil);
		_renderCache.CacheFrameBufferObject(fbo);

		if ( bClearDepth)
			glClearBufferfv(GL_DEPTH, 0, &depth);

		if ( bClearStencil )
			glClearStencil(stencil);*/
	}

	void GLRenderEngine::SetRenderTarget(const SurfaceViewPtr& renderTarget, const SurfaceViewPtr& depthStencil, bool useReadOnlyDepthStencil)
	{
		BOOST_ASSERT(0);
	}

	void GLRenderEngine::SetRenderTarget(const RenderSurfacePtr& renderTarget, bool useReadOnlyDepthStencil)
	{
		BOOST_ASSERT(0);
	}

	void GLRenderEngine::SetRenderLayout(RenderLayoutPtr const& renderLayout)
	{
		GLuint va = (GLuint)renderLayout->GetHandle();
		glBindVertexArray(va);
	}

	void GLRenderEngine::SetPrimitiveTopology(TopologyType topologyType)
	{

	}

	void GLRenderEngine::SetEffect(RenderEffectPtr const& effect)
	{

	}
	void GLRenderEngine::DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation)
	{

	}
	void GLRenderEngine::Draw(unsigned int vertexCount, unsigned int startVertexLocation)
	{

	}

	void* GLRenderEngine::Map(RenderBufferPtr const& buffer, BufferAccess bufferAccess)
	{
		return NULL;
	}
	void GLRenderEngine::UnMap(RenderBufferPtr const& buffer)
	{

	}
	void GLRenderEngine::UpdateSubresource(RenderBufferPtr const& buffer, void* pSrcData, unsigned int srcDataSize)
	{

	}

	void GLRenderEngine::SaveSurfaceView(SurfaceViewPtr const& surface, std::string const& fileName)
	{

	}


	void GLRenderEngine::SetBlendState(BlendStatePtr const& blendState)
	{

	}
	void GLRenderEngine::SetRasterizeState(RasterizeStatePtr const& rasterizeState)
	{

	}
	void GLRenderEngine::SetDepthStencilState(DepthStencilStatePtr const& depthStencilState)
	{

	}



	void GLRenderEngine::sGLEngineCache::CacheFrameBufferObject(GLuint fbo)
	{
		if (FrameBufferObject != fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			FrameBufferObject = fbo;
		}
	}





}