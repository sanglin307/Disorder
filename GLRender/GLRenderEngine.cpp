#include "GLRenderInclude.h"


namespace Disorder
{
	void GLDebugLayer::Init()
	{
		GLRenderEngine* engine = (GLRenderEngine*)GEngine->RenderEngine;
		if (engine->IsVersionSupported(4, 3))
		{
			glDebugMessageCallback(DebugCallback, this);
			glEnable(GL_DEBUG_OUTPUT);
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
			glEnable(GL_DEBUG_OUTPUT);
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

#ifdef _DEBUG
		OutputDebugStringA(stream.str().c_str());
#endif
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
			glFormat = GL_RGBA_INTEGER;
			glType = GL_UNSIGNED_INT;
			return GL_RGBA32UI;
		case PF_R32G32B32A32_SINT:
			glFormat = GL_RGBA_INTEGER;
			glType = GL_INT;
			return GL_RGBA32I;
		case PF_R32G32B32_TYPELESS:
		case PF_R32G32B32_FLOAT:
			glFormat = GL_RGB;
			glType = GL_FLOAT;
			return GL_RGB32F;
		case PF_R32G32B32_UINT:
			glFormat = GL_RGB_INTEGER;
			glType = GL_UNSIGNED_INT;
			return GL_RGB16UI;
		case PF_R32G32B32_SINT:
			glFormat = GL_RGB_INTEGER;
			glType = GL_INT;
			return GL_RGB16I;
		case PF_R16G16B16A16_UNORM:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGBA16;
		case PF_R16G16B16A16_SNORM:
			glFormat = GL_RGBA;
			glType = GL_SHORT;
			return GL_RGBA_SNORM;
		case PF_R16G16B16A16_TYPELESS:
		case PF_R16G16B16A16_FLOAT:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGBA16F;
		case PF_R16G16B16A16_UINT:
			glFormat = GL_RGBA_INTEGER;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGBA16UI;
		case PF_R16G16B16A16_SINT:
			glFormat = GL_RGBA_INTEGER;
			glType = GL_SHORT;
			return GL_RGBA16I;
		case PF_R32G32_TYPELESS:
		case PF_R32G32_FLOAT:
			glFormat = GL_RG;
			glType = GL_FLOAT;
			return GL_RG32F;
		case PF_R32G32_UINT:
			return GL_RG32UI;
			glFormat = GL_RG_INTEGER;
			glType = GL_UNSIGNED_INT;
		case PF_R32G32_SINT:
			glFormat = GL_RG_INTEGER;
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
			glFormat = GL_RGBA_INTEGER;
			glType = GL_UNSIGNED_INT_10_10_10_2;
			return GL_RGB10_A2UI;
		case PF_R11G11B10_FLOAT:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_INT_10F_11F_11F_REV;
			return GL_R11F_G11F_B10F;
		case PF_R8G8B8A8_TYPELESS:
		case PF_R8G8B8A8_UNORM:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_BYTE;
			return GL_RGBA8;
		case PF_R8G8B8A8_UNORM_SRGB:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_BYTE;
			return GL_SRGB8;
		case PF_R8G8B8A8_SNORM:
			glFormat = GL_RGBA;
			glType = GL_BYTE;
			return GL_RGBA8_SNORM;
		case PF_R8G8B8A8_UINT:
			glFormat = GL_RGBA_INTEGER;
			glType = GL_UNSIGNED_BYTE;
			return GL_RGBA8UI;
		case PF_R8G8B8A8_SINT:
			glFormat = GL_RGBA_INTEGER;
			glType = GL_BYTE;
			return GL_RGBA8I;
		case PF_R16G16_TYPELESS:
		case PF_R16G16_FLOAT:
			glFormat = GL_RG;
			glType = GL_HALF_FLOAT;
			return GL_RG16F;
		case PF_R16G16_UNORM:
			glFormat = GL_RG;
			glType = GL_UNSIGNED_SHORT;
			return GL_RGB16;
		case PF_R16G16_SNORM:
			glFormat = GL_RG;
			glType = GL_SHORT;		
			return GL_RG16_SNORM;
		case PF_R16G16_UINT:
			glFormat = GL_RG_INTEGER;
			glType = GL_UNSIGNED_SHORT;
			return GL_RG16UI;
		case PF_R16G16_SINT:
			glFormat = GL_RG_INTEGER;
			glType = GL_SHORT;
			return GL_RG16I;
		case PF_D32_FLOAT:
			glFormat = GL_DEPTH_COMPONENT;
			glType = GL_FLOAT;
			return GL_DEPTH_COMPONENT32F;
		case PF_R32_TYPELESS:
		case PF_R32_FLOAT:
			glFormat = GL_RED;
			glType = GL_FLOAT;
			return GL_R32F;	
		case PF_R32_UINT:
			glFormat = GL_RED_INTEGER;
			glType = GL_UNSIGNED_INT;
			return GL_R32UI;
		case PF_R32_SINT :
			glFormat = GL_RED_INTEGER;
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
			glFormat = GL_RG_INTEGER;
			glType = GL_UNSIGNED_BYTE;
			return GL_RG8UI;
		case PF_R8G8_SNORM:
			glFormat = GL_RG;
			glType = GL_BYTE;
			return GL_RG8_SNORM;
		case PF_R8G8_SINT:
			glFormat = GL_RG_INTEGER;
			glType = GL_UNSIGNED_BYTE;
			return GL_RG8I;
		case PF_R16_TYPELESS:
		case PF_R16_FLOAT:
			glFormat = GL_RED;
			glType = GL_HALF_FLOAT;
			return GL_R16F;
		case PF_R16_UNORM:
			glFormat = GL_RED;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16;
		case PF_D16_UNORM:
			glFormat = GL_DEPTH_COMPONENT;
			glType = GL_UNSIGNED_SHORT;
			return GL_DEPTH_COMPONENT16;
		case PF_R16_UINT:
			glFormat = GL_RED_INTEGER;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16UI;
		case PF_R16_SNORM:
			glFormat = GL_RED;
			glType = GL_UNSIGNED_SHORT;
			return GL_R16_SNORM;
		case PF_R16_SINT:
			glFormat = GL_RED_INTEGER;
			glType = GL_SHORT;
			return GL_R16I;
		case PF_R8_TYPELESS:
		case PF_R8_UNORM:
			glFormat = GL_RED;
			glType = GL_UNSIGNED_BYTE;
			return GL_R8;
		case PF_R8_UINT:
			glFormat = GL_RED_INTEGER;
			glType = GL_UNSIGNED_BYTE;
			return GL_R8UI;
		case PF_R8_SNORM:
			glFormat = GL_RED;
			glType = GL_BYTE;
			return GL_R8_SNORM;
		case PF_R8_SINT:
			glFormat = GL_RED_INTEGER;
			glType = GL_BYTE;
			return GL_R8I;
		case PF_R9G9B9E5_SHAREDEXP:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_INT_5_9_9_9_REV;
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

	GLenum GLRenderEngine::GetGLFillMode(RenderFillMode fm)
	{
		switch (fm)
		{
		case RFM_Wireframe:
			return GL_LINE;
		case Disorder::RFM_Solid:
			return GL_FILL;
		default:
			BOOST_ASSERT(0);
		}

		return 0;
	}

	GLenum GLRenderEngine::GetGLCullMode(RenderCullMode cm)
	{
		switch (cm)
		{
		case RCM_CullFront:
			return GL_FRONT;
		case RCM_CullBack:
			return GL_BACK;
		default:
			BOOST_ASSERT(0);
		}

		return 0;
	}

	GLenum GLRenderEngine::GetGLStencilOp(StencilOperation sop)
	{
		switch (sop)
		{
		case STENCIL_OP_KEEP:
			return GL_KEEP;
		case STENCIL_OP_ZERO:
			return GL_ZERO;
		case STENCIL_OP_REPLACE:
			return GL_REPLACE;
		case STENCIL_OP_INCR_SAT:
			return GL_INCR_WRAP;
		case STENCIL_OP_DECR_SAT:
			return GL_DECR_WRAP;
		case STENCIL_OP_INVERT:
			return GL_INVERT;
		case STENCIL_OP_INCR:
			return GL_INCR;
		case STENCIL_OP_DECR:
			return GL_DECR;
		default:
			BOOST_ASSERT(0);
		}

		return 0;
	}

	GLenum GLRenderEngine::GetGLBlendFunc(BlendOptions blendOptions)
	{
		switch (blendOptions)
		{
		case BLEND_ZERO:
			return GL_ZERO;
		case BLEND_ONE:
			return GL_ONE;
		case BLEND_SRC_COLOR:
			return GL_SRC_COLOR;
		case BLEND_INV_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case BLEND_SRC_ALPHA:
			return GL_SRC_ALPHA;
		case BLEND_INV_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case BLEND_DEST_ALPHA:
			return GL_DST_ALPHA;
		case BLEND_INV_DEST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
		case BLEND_DEST_COLOR:
			return GL_DST_COLOR;
		case BLEND_INV_DEST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case BLEND_SRC_ALPHA_SAT:
			return GL_SRC_ALPHA_SATURATE;
		case BLEND_BLEND_FACTOR:
			return GL_CONSTANT_COLOR;
		case BLEND_INV_BLEND_FACTOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case BLEND_SRC1_COLOR:
			return GL_SRC1_COLOR;
		case BLEND_INV_SRC1_COLOR:
			return GL_ONE_MINUS_SRC1_COLOR;
		case BLEND_SRC1_ALPHA:
			return GL_SRC1_ALPHA;
		case BLEND_INV_SRC1_ALPHA:
			return GL_ONE_MINUS_SRC1_ALPHA;
		default:
			BOOST_ASSERT(0);
		}

		return GL_NONE;
	}

	GLenum GLRenderEngine::GetGLBlendOp(BlendOperation blendOp)
	{
		switch (blendOp)
		{
		case BLEND_OP_ADD:
			return GL_FUNC_ADD;
		case BLEND_OP_SUBTRACT:
			return GL_FUNC_SUBTRACT;
		case BLEND_OP_REV_SUBTRACT:
			return GL_FUNC_REVERSE_SUBTRACT;
		case BLEND_OP_MIN:
			return GL_MIN;
		case BLEND_OP_MAX:
			return GL_MAX;
		default:
			BOOST_ASSERT(0);
		}

		return GL_NONE;
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

	GLRenderEngine::~GLRenderEngine()
	{
	
	}

	void GLRenderEngine::CreateMainTarget()
	{
		RenderTexture2D* depthStencilTex = GEngine->RenderResourceMgr->CreateTexture2D(NULL, GConfig->pRenderConfig->DepthStencilFormat, GConfig->pRenderConfig->SizeX, GConfig->pRenderConfig->SizeY, false, false, SV_DepthStencil,1, NULL,0);
		SurfaceView* DepthBufferView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_DepthStencil, depthStencilTex, GConfig->pRenderConfig->DepthStencilFormat, 0);
 
		RenderTexture2D* mainTex = GEngine->RenderResourceMgr->CreateTexture2D(NULL, GConfig->pRenderConfig->ColorFormat, GConfig->pRenderConfig->SizeX, GConfig->pRenderConfig->SizeY, false, false, SV_RenderTarget ,1, NULL,0);
		SurfaceView* mainTargetView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_RenderTarget, mainTex, GConfig->pRenderConfig->ColorFormat);
		SurfaceView* shaderView = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, mainTex, GConfig->pRenderConfig->ColorFormat);

		std::map<ESurfaceLocation, SurfaceView*> viewMap;
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_DepthStencil, DepthBufferView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget1, mainTargetView));
		RenderSurface* mainSurface = GEngine->RenderResourceMgr->CreateRenderSurface(viewMap);

		GEngine->SurfaceCache->MainTarget = new MainRenderTarget(mainSurface, shaderView, mainTargetView, DepthBufferView);
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

		CreateMainTarget();

		LoadShaderIncludeFiles();
		LoadGLExtensions();

		LoadGLProfile();

		_debugLayer = new GLDebugLayer;
		_debugLayer->Init();
	}

	void GLRenderEngine::SetViewport(float width, float height, float minDepth, float maxDepth, float topX, float topY)
	{
		glViewport((GLint)topX, (GLint)topY, (GLsizei)width, (GLsizei)height);
		glDepthRange(minDepth, maxDepth);
	}

	void GLRenderEngine::LoadGLProfile()
	{
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &_profile.UniformBufferMaxBindings);
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
			if (!_stricmp(extension.c_str(),_vGLExtensions[i].c_str()))
				return true;
		}

		return false;
	}

	void GLRenderEngine::LoadShaderIncludeForPath(const boost::filesystem::path& p)
	{
		if (boost::filesystem::is_directory(p))
		{
			boost::filesystem::directory_iterator enditer;
			for (boost::filesystem::directory_iterator fileiter(p); fileiter != enditer; ++fileiter)
			{
				LoadShaderIncludeForPath(fileiter->path());
			}
		}
		else if (boost::filesystem::extension(p) == ".gls" || boost::filesystem::extension(p) == ".h")
		{
			std::string path = p.string();
			boost::to_lower(path);
			int pos = path.find("glsl");
			std::string filekey = "/" + path.substr(pos + 5);//"/glsl/" + p.leaf().string();
			for (size_t i = 0; i < filekey.size(); i++)
			{
				if (filekey[i] == '\\')
					filekey[i] = '/';
			}

			FileObject* file = GEngine->FileManager->OpenTextFile(path, eF_Read);
			std::string content = file->ReadText();
			if (ShaderObject::sMapIncludeFiles.find(filekey) == ShaderObject::sMapIncludeFiles.end())
			{
				ShaderObject::sMapIncludeFiles.insert(std::pair<std::string, std::string>(filekey, content));
			}
		}
	}

	void GLRenderEngine::LoadShaderIncludeFiles()
	{
		std::string shaderPath = GConfig->ResourceFXPath + "GLSL\\";
		boost::filesystem::path p(shaderPath);
		if (!boost::filesystem::exists(p) || !boost::filesystem::is_directory(p) )
		{
			GLogger->Error("shader's path not exist!");
			return;
		}

		boost::filesystem::directory_iterator enditer;
		for (boost::filesystem::directory_iterator fileiter(p); fileiter != enditer; ++fileiter)
		{
			LoadShaderIncludeForPath(fileiter->path());
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

		//_renderCache.Init();

		glViewport(0, 0, GConfig->pRenderConfig->SizeX, GConfig->pRenderConfig->SizeY);
	

		return true;
	}


	void GLRenderEngine::Init()
	{
		
	}
	
	void GLRenderEngine::Exit()
	{
		//wglMakeCurrent(NULL, NULL);
		//wglDeleteContext(_hRC);
	}

	void GLRenderEngine::OnFrameBegin()
	{
 
		GEngine->Stat.OnFrameBegin();
	}

	void GLRenderEngine::CopyTexture2D(RenderTexture2D* srcTexture, RenderTexture2D* dstTexture)
	{
		GLRenderTexture2D* srcTex = (GLRenderTexture2D*)srcTexture;
		GLRenderTexture2D* dstTex = (GLRenderTexture2D*)dstTexture;

		glCopyImageSubData((GLuint)srcTex->GetHandle(), srcTex->GetGLFormat(), 0, 0, 0, 0, (GLuint)dstTex->GetHandle(), dstTex->GetGLFormat(), 0, 0, 0, 0, srcTex->Width, srcTex->Height, 1);
	}

	void GLRenderEngine::OnFrameEnd()
	{
		//blit
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLint)GEngine->SurfaceCache->MainTarget->RenderTargetSurface->GetHandle());
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		GLint SizeX = GConfig->pRenderConfig->SizeX;
		GLint SizeY = GConfig->pRenderConfig->SizeY;
		glBlitFramebuffer(0, 0, SizeX, SizeY, 0, 0, SizeX, SizeY, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		SwapBuffers(_hDC);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
	
		GEngine->Stat.OnFrameEnd();
	}

  
	void GLRenderEngine::ClearRenderSurface(RenderSurface* renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil, int sliceIndex)
	{
		//SetRenderTarget(renderSurface,sliceIndex);

		if (renderSurface == NULL) //default
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));

			if (bClearDepth)
				glClearBufferfv(GL_DEPTH, 0, &depth);

			if (bClearStencil)
			{
				GLint istencil = stencil;
				glClearBufferiv(GL_STENCIL, 0, &istencil);
			}

			return;
		}

		GLRenderSurface* surface = (GLRenderSurface*)renderSurface;
 
		for (int i = 0; i < SL_SurfaceLoactionMax; i++)
		{
			if (surface->_surfacesViewArray[i] == NULL)
				continue;

			if (i == SL_DepthStencil)
			{
				if (bClearDepth)
					glClearBufferfv(GL_DEPTH, 0, &depth);

				if (bClearStencil)
				{
					GLint istencil = stencil;
					glClearBufferiv(GL_STENCIL, 0, &istencil);
				}
			}
			else if (i >= SL_RenderTarget1 && i <= SL_RenderTarget8)
			{
				int loc = i - SL_RenderTarget1;
				glClearBufferfv(GL_COLOR, loc, glm::value_ptr(color));
			}
		}
	}

 

	void GLRenderEngine::SetRenderTarget(RenderSurface* renderTarget, int sliceIndex,bool useReadOnlyDepthStencil)
	{
		if (renderTarget == NULL)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_BACK);
			return;
		}

		GLuint fbo = (GLuint)renderTarget->GetHandle();
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// for layer renderer
		if (sliceIndex >= 0)
		{
			for (int index = SL_DepthStencil; index < SL_SurfaceLoactionMax; index++)
			{
				SurfaceView* view = renderTarget->GetSurfaceView((ESurfaceLocation)index);
				if (view != NULL && view->Resource->ArraySize > 1)
				{
					GLenum attachment = GL_DEPTH_ATTACHMENT;
					if (index >= SL_RenderTarget1)
						attachment = GL_COLOR_ATTACHMENT0 + index - SL_RenderTarget1;
					if (view->Resource->ViewFlag & SF_AsCubeMap)
					{
						GLenum cubeFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X + sliceIndex;
						glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, cubeFace, (GLuint)view->Resource->GetHandle(), 0);
					}
					else
					    glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, (GLuint)view->Resource->GetHandle(), 0, sliceIndex);
					//glDrawBuffer(GL_NONE);
					//glReadBuffer(GL_NONE);
					//GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
					//BOOST_ASSERT(result == GL_FRAMEBUFFER_COMPLETE);
				}
				else
					break;
			}
		}

		GLRenderSurface* GLSurface = (GLRenderSurface*)renderTarget;
		std::vector<GLenum> bufferArray;
		GLSurface->GetGLDrawBuffers(bufferArray);
		 
		if (bufferArray.size() > 0)
			glDrawBuffers(bufferArray.size(),bufferArray.data());
		else
			glDrawBuffer(GL_NONE);
	}

	bool GLRenderEngine::IsTextureSamplerResouce(GLenum type)
	{
		if (type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_SHADOW)
			return true;
		if (type == GL_SAMPLER_1D || type == GL_SAMPLER_1D_SHADOW)
			return true;
		
		if (type == GL_SAMPLER_CUBE || type == GL_SAMPLER_CUBE_SHADOW)
			return true;

		if (type == GL_SAMPLER_2D_ARRAY || type == GL_SAMPLER_2D_ARRAY_SHADOW)
			return true;

		if (type == GL_SAMPLER_3D)
			return true;

		return false;
	}

	void GLRenderEngine::SetRenderLayout(RenderLayout* renderLayout)
	{
		TopologyType topology = renderLayout->GetTopology();
		CachedTopology = topology;
	
		RenderBuffer* indexBuffer = renderLayout->GetIndexBuffer();
		GLuint vao = (GLuint)renderLayout->GetHandle();
		if (indexBuffer)
		{	
			_indexElementSize = indexBuffer->GetElementSize();
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)indexBuffer->GetHandle());
		}
		else
			glBindVertexArray(vao); // don't use index draw
	}

	void GLRenderEngine::SetPrimitiveTopology(TopologyType topologyType)
	{
		CachedTopology = topologyType;
	}

	void GLRenderEngine::SetEffect(RenderEffect* effect)
	{
		if (effect == NULL)
		{
			glUseProgram(0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,0);
			return;
		}

		SetBlendState(effect->GetBlendState());
		SetRasterizeState(effect->GetRasterizeState());
		SetDepthStencilState(effect->GetDepthStencilState());

		GLRenderEffect* effectGL = (GLRenderEffect*)effect;
		GLShaderResourceBinding* pDesc = NULL;
		std::vector<GLuint> samplerBinding;
 
		int tex2DIndex = 0;
		for (size_t i = 0; i < effectGL->EffectReflection->ResourceArray.size(); i++)
		{
			pDesc = &(effectGL->EffectReflection->ResourceArray[i]);
			if (GLRenderEngine::IsTextureSamplerResouce(pDesc->Type))
			{
				SurfaceView* tex = pDesc->ParamRef->GetDataAsShaderResource();
				GLRenderTexture2D* res = (GLRenderTexture2D*)tex->Resource;
				
				if (pDesc->Type == GL_SAMPLER_CUBE)
				{
					//glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)res->GetHandle());
				}
				else if (pDesc->Type == GL_SAMPLER_2D_ARRAY)
				{
					//glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D_ARRAY, (GLuint)res->GetHandle());
				}
				else
				{
					glActiveTexture(GL_TEXTURE0 + tex2DIndex);
					glBindTexture(GL_TEXTURE_2D, (GLuint)res->GetHandle());
					tex2DIndex++;
				}

				if (res->Sampler)
				    samplerBinding.push_back((GLuint)res->Sampler->GetHandle());
				else
				{
					//default texture binding
					samplerBinding.push_back((GLuint)RenderResourceManager::DefaultSamplerState->GetHandle());
				}
				
			}
		}
 
		if (samplerBinding.size() > 0)
		{
			glBindSamplers(0, samplerBinding.size(), samplerBinding.data());
		}

		GLuint program = (GLuint)effect->GetHandle();
		glUseProgram(program);
 
	}

	void GLRenderEngine::DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation)
	{
		//GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT
		GLenum type;
		if (_indexElementSize == 1)
			type = GL_UNSIGNED_BYTE;
		else if (_indexElementSize == 2)
			type = GL_UNSIGNED_SHORT;
		else if (_indexElementSize == 4)
			type = GL_UNSIGNED_INT;
		else
			return;

		glDrawElementsBaseVertex(GetPlatformTopology(CachedTopology), indexCount, type, (const GLvoid*)startIndexLocation, baseVertexLocation);
		GEngine->Stat.DrawTriNumber += GetTriangleCountFromTopology(CachedTopology, indexCount);
	}

	void GLRenderEngine::Draw(unsigned int vertexCount, unsigned int startVertexLocation)
	{
		glDrawArrays(GetPlatformTopology(CachedTopology), startVertexLocation, vertexCount);
		GEngine->Stat.DrawTriNumber += GetTriangleCountFromTopology(CachedTopology, vertexCount);
	}

	GLenum GLRenderEngine::GetBufferAccessFlag(BufferAccess ba)
	{
		if (ba == BA_Read_Only)
			return GL_MAP_READ_BIT;
		else if (ba == BA_Write_Only)
			return GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		else if (ba == BA_Read_Write)
			return GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;

		return GL_NONE;
	}

	void* GLRenderEngine::Map(RenderBuffer* buffer, BufferAccess bufferAccess)
	{
		if (buffer->GetBufferType() == RBT_Vertex)
		{
			GLuint vbo = (GLuint)buffer->GetHandle();
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			return glMapBufferRange(GL_ARRAY_BUFFER, 0, buffer->GetBufferSize(), GetBufferAccessFlag(bufferAccess));
		}
		else if (buffer->GetBufferType() == RBT_Index)
		{
			GLuint ibo = (GLuint)buffer->GetHandle();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, buffer->GetBufferSize(), GetBufferAccessFlag(bufferAccess));
		}
		else if (buffer->GetBufferType() == RBT_Constant)
		{
			GLRenderBuffer* GLbuffer = (GLRenderBuffer*)buffer;
			GLuint ubo = (GLuint)GLbuffer->GetHandle();
			glBindBufferBase(GL_UNIFORM_BUFFER, GLbuffer->GetBindingPoint(), ubo);
			return glMapBufferRange(GL_UNIFORM_BUFFER, 0, buffer->GetBufferSize(), GetBufferAccessFlag(bufferAccess));
		}

		return NULL;
	}
	void GLRenderEngine::UnMap(RenderBuffer* buffer)
	{
		if (buffer->GetBufferType() == RBT_Vertex)
		{
			GLuint vbo = (GLuint)buffer->GetHandle();
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		else if (buffer->GetBufferType() == RBT_Index)
		{
			GLuint ibo = (GLuint)buffer->GetHandle();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
		else if (buffer->GetBufferType() == RBT_Constant)
		{
			GLRenderBuffer* GLbuffer = (GLRenderBuffer*)buffer;
			GLuint ubo = (GLuint)GLbuffer->GetHandle();
			glBindBufferBase(GL_UNIFORM_BUFFER, GLbuffer->GetBindingPoint(), ubo);
			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}
	}

	void GLRenderEngine::UpdateSubresource(RenderBuffer* buffer, void* pSrcData, unsigned int srcDataSize)
	{
		if (buffer->GetBufferType() == RBT_Vertex)
		{
			GLuint vbo = (GLuint)buffer->GetHandle();
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER,0,srcDataSize,pSrcData);
		}
		else if (buffer->GetBufferType() == RBT_Index)
		{
			GLuint ibo = (GLuint)buffer->GetHandle();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, srcDataSize, pSrcData);
		}
		else if (buffer->GetBufferType() == RBT_Constant)
		{
			GLRenderBuffer* GLbuffer = (GLRenderBuffer*)buffer;
			GLuint ubo = (GLuint)GLbuffer->GetHandle();
			glBindBufferBase(GL_UNIFORM_BUFFER, GLbuffer->GetBindingPoint(), ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, srcDataSize, pSrcData);
		}
		
	}

	void GLRenderEngine::SaveSurfaceView(SurfaceView* surface, std::string const& fileName)
	{
		// use glGetTexLevelParameterfv to get texture information, and then download texture use glGetTexImage
		BOOST_ASSERT(0);
	}


	void GLRenderEngine::SetBlendState(BlendState* blendState)
	{
		if (CachedBlendState != blendState)
		{
			CachedBlendState = blendState;
			if ( blendState->AlphaToCoverageEnable )
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			else
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

			bool bEnable = false;
			for (int i = 0; i < 8; i++)
			{
				bEnable |= blendState->Desc[i].BlendEnable;
				if (bEnable)
					break;
			}

			if (!bEnable)
			{
				glDisable(GL_BLEND);
				return;
			}

			glEnable(GL_BLEND);
			glBlendColor(blendState->BlendFactor[0], blendState->BlendFactor[1], blendState->BlendFactor[2], blendState->BlendFactor[3]);
			if (blendState->IndependentBlendEnable)
			{
				for (int j = 0; j < 8; j++)
				{
					if (!blendState->Desc[j].BlendEnable)
						continue;

					glBlendEquationSeparatei(j, GetGLBlendOp(blendState->Desc[j].BlendOp), GetGLBlendOp(blendState->Desc[j].BlendOpAlpha));
					glBlendFuncSeparatei(j, GetGLBlendFunc(blendState->Desc[j].SrcBlend), GetGLBlendFunc(blendState->Desc[j].DestBlend),
						                       GetGLBlendFunc(blendState->Desc[j].SrcBlendAlpha), GetGLBlendFunc(blendState->Desc[j].DestBlendAlpha));
				}
			}
			else
			{
				glBlendEquationSeparate(GetGLBlendOp(blendState->Desc[0].BlendOp), GetGLBlendOp(blendState->Desc[0].BlendOpAlpha));
				glBlendFuncSeparate(GetGLBlendFunc(blendState->Desc[0].SrcBlend), GetGLBlendFunc(blendState->Desc[0].DestBlend),
					                        GetGLBlendFunc(blendState->Desc[0].SrcBlendAlpha), GetGLBlendFunc(blendState->Desc[0].DestBlendAlpha));
			}
		}
	}

	void GLRenderEngine::SetRasterizeState(RasterizeState* rasterizeState)
	{
		if (CachedRasterizeState != rasterizeState)
		{
			CachedRasterizeState = rasterizeState;
			
			glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(rasterizeState->Desc.FillMode));
			if (rasterizeState->Desc.CullMode == RCM_None)
			{
				glDisable(GL_CULL_FACE);
			}
			else
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GetGLCullMode(rasterizeState->Desc.CullMode));
			}

			if (rasterizeState->Desc.FrontCounterClockwise)
				glFrontFace(GL_CCW);
			else
				glFrontFace(GL_CW);

			glPolygonOffset(rasterizeState->Desc.SlopeScaledDepthBias, (GLfloat)rasterizeState->Desc.DepthBias);
			if (rasterizeState->Desc.AntialiasedLineEnable)
				glEnable(GL_LINE_SMOOTH);
			else
				glDisable(GL_LINE_SMOOTH);
 
			if (rasterizeState->Desc.DepthClipEnable)
				glEnable(GL_DEPTH_CLAMP);
			else
				glDisable(GL_DEPTH_CLAMP);

			if (rasterizeState->Desc.MultisampleEnable)
				glEnable(GL_MULTISAMPLE);
			else
				glDisable(GL_MULTISAMPLE);

			if (rasterizeState->Desc.ScissorEnable)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
		}
	}

	void GLRenderEngine::SetDepthStencilState(DepthStencilState* depthStencilState)
	{
		if (CachedDepthStencilState != depthStencilState)
		{
			CachedDepthStencilState = depthStencilState;
 
			if (depthStencilState->Desc.DepthEnable)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GLRenderEngine::GetGLComparisonFunc(depthStencilState->Desc.DepthFunc));
				glDepthMask(depthStencilState->Desc.DepthWrite);
			}
			else
				glDisable(GL_DEPTH_TEST);

			if (depthStencilState->Desc.StencilEnable)
			{
				glEnable(GL_STENCIL_TEST);

				if ( depthStencilState->Desc.FrontFaceStencilFunc == depthStencilState->Desc.BackFaceStencilFunc )
					glStencilFunc(GLRenderEngine::GetGLComparisonFunc(depthStencilState->Desc.FrontFaceStencilFunc), depthStencilState->StencilRef, depthStencilState->Desc.StencilReadMask);
				else
				{
					glStencilFuncSeparate(GL_FRONT, GLRenderEngine::GetGLComparisonFunc(depthStencilState->Desc.FrontFaceStencilFunc), depthStencilState->StencilRef, depthStencilState->Desc.StencilReadMask);
					glStencilFuncSeparate(GL_BACK, GLRenderEngine::GetGLComparisonFunc(depthStencilState->Desc.BackFaceStencilFunc), depthStencilState->StencilRef, depthStencilState->Desc.StencilReadMask);
				}
			 
				glStencilOpSeparate(GL_FRONT, GLRenderEngine::GetGLStencilOp(depthStencilState->Desc.FrontFaceStencilFailOp), GLRenderEngine::GetGLStencilOp(depthStencilState->Desc.FrontFaceStencilDepthFailOp),
					GLRenderEngine::GetGLStencilOp(depthStencilState->Desc.FrontFaceStencilPassOp));
				glStencilOpSeparate(GL_BACK, GLRenderEngine::GetGLStencilOp(depthStencilState->Desc.BackFaceStencilFailOp), GLRenderEngine::GetGLStencilOp(depthStencilState->Desc.BackFaceStencilDepthFailOp),
					GLRenderEngine::GetGLStencilOp(depthStencilState->Desc.BackFaceStencilPassOp));
				
				glStencilMask(depthStencilState->Desc.StencilWriteMask);
			}
			else
				glDisable(GL_STENCIL_TEST);
		}
	}

	GLenum GLRenderEngine::GetPlatformTopology(TopologyType tType)
	{			
		switch (tType)
		{
		case TT_PointList:
			return GL_POINTS;
		case TT_LineList:
			return GL_LINES;
		case TT_LineStrip:
			return GL_LINE_STRIP;
		case TT_TriangleList:
			return GL_TRIANGLES;
		case TT_TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case TT_LineList_Adj:
			return GL_LINES_ADJACENCY;
		case TT_LineStrip_Adj:
			return GL_LINE_STRIP_ADJACENCY;
		case TT_TriangleList_Adj:
			return GL_TRIANGLES_ADJACENCY;
		case TT_TriangleStrip_Adj:
			return GL_TRIANGLE_STRIP_ADJACENCY;
		default:
			break;
		}

		if (tType >= TT_1_Ctrl_Pt_PatchList && tType <= TT_32_Ctrl_Pt_PatchList)
		{
			return GL_PATCHES;
		}

		return GL_NONE;
	}

	/*void GLRenderEngine::GLEngineCache::Init()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
		glUseProgram(ShaderProgram);
		glBindVertexArray(VertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
		glBindBufferBase(GL_UNIFORM_BUFFER, UniformBufferBindingPoint,UniformBufferObject);

		if (DepthEnable)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(DepthWrite);
			glDepthFunc(DepthFunc);
		}
		else
			glDisable(GL_DEPTH_TEST);
		

		if (StencilEnable)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFuncSeparate(FrontFaceStencilFunc, BackFaceStencilFunc, StencilRef, StencilReadMask);
			glStencilOpSeparate(GL_FRONT, FrontFaceStencilFailOp, FrontFaceStencilDepthFailOp, FrontFaceStencilPassOp);
			glStencilOpSeparate(GL_BACK, BackFaceStencilFailOp, BackFaceStencilDepthFailOp, BackFaceStencilPassOp);
			glStencilMask(StencilWriteMask);
		}
		else
			glDisable(GL_STENCIL_TEST);

		
		glPolygonMode(GL_FRONT_AND_BACK, FillMode);
		if (CullEnable)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(CullMode);
		}
		else
			glDisable(GL_CULL_FACE);

		glFrontFace(FrontFace);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(SlopeScaledDepthBias, (GLfloat)DepthBias);

		if (DepthClipEnable)
			glEnable(GL_DEPTH_CLAMP);
		else
			glDisable(GL_DEPTH_CLAMP);

		if (ScissorEnable)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);

		if (MultisampleEnable)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);

		if (AntialiasedLineEnable)
			glEnable(GL_LINE_SMOOTH);
		else
			glDisable(GL_LINE_SMOOTH);

		if (AlphaToCoverageEnable)
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		else
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		if (BlendEnable)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		if (MultiDrawBuffers.size() > 0)
			glDrawBuffers(MultiDrawBuffers.size(), MultiDrawBuffers.data());
		else
			glDrawBuffer(SingleDrawBuffer);

	}*/

	/*void GLRenderEngine::GLEngineCache::CacheFrameBufferObject(GLuint fbo)
	{
		if (FrameBufferObject != fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			FrameBufferObject = fbo;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheSingleDrawBuffer(GLenum buf)
	{
		if (buf != SingleDrawBuffer)
		{
			buf = SingleDrawBuffer;
			glDrawBuffer(buf);
			
			MultiDrawBuffers.clear();
		}
	}

	void GLRenderEngine::GLEngineCache::CacheMultiDrawBuffers(const std::vector<GLenum>& buffers)
	{
		bool bChange = false;
		if (MultiDrawBuffers.size() != buffers.size())
			bChange = true;

		if (!bChange)
		{
			for (size_t i = 0; i < MultiDrawBuffers.size(); i++)
			{
				if (MultiDrawBuffers[i] != buffers[i])
				{
					bChange = true;
					break;
				}
			}
		}

		if (!bChange)
			return;

		MultiDrawBuffers.clear();
		std::copy(buffers.begin(), buffers.end(), std::back_inserter(MultiDrawBuffers));
		glDrawBuffers(MultiDrawBuffers.size(), MultiDrawBuffers.data());
		SingleDrawBuffer = 0;
	}

	void GLRenderEngine::GLEngineCache::CacheShaderProgram(GLuint sp)
	{
		if (ShaderProgram != sp)
		{
			glUseProgram(sp);
			ShaderProgram = sp;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheVertexArrayObject(GLuint vao,GLuint ies)
	{
		if (VertexArrayObject != vao)
		{
			glBindVertexArray(vao);
			VertexArrayObject = vao;
			IndexElementSize = ies;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheVertexBufferObject(GLuint vbo)
	{
		if (VertexBufferObject != vbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			VertexBufferObject = vbo;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheIndexBufferObject(GLuint ibo, GLuint ies)
	{
		if (IndexBufferObject != ibo)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			IndexBufferObject = ibo;
			IndexElementSize = ies;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheUniformBufferObject(GLuint ubo, GLuint bindingPoint)
	{
		if (UniformBufferObject != ubo)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
			UniformBufferObject = ubo;
			UniformBufferBindingPoint = bindingPoint;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheDepthEnable(bool bEnable)
	{
		if (DepthEnable != bEnable)
		{
			DepthEnable = bEnable;
			if (bEnable)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheDepthWrite(bool bWrite)
	{
		if (!DepthEnable)
			return;

		if (DepthWrite != bWrite)
		{
			DepthWrite = bWrite; 
			glDepthMask(bWrite);		 
		}
	}

	void GLRenderEngine::GLEngineCache::CacheDepthFunc(GLenum eFunc)
	{
		if (!DepthEnable)
			return;

		if (DepthFunc != eFunc)
		{
			DepthFunc = eFunc;
			glDepthFunc(eFunc);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheStencilEnable(bool bEnable)
	{
		if (StencilEnable != bEnable)
		{
			StencilEnable = bEnable;
			if (bEnable)
				glEnable(GL_STENCIL_TEST);
			else
				glDisable(GL_STENCIL_TEST);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheStencilFunc(GLenum frontfunc, GLenum backfunc, GLint ref, BYTE mask)
	{
		if (!StencilEnable)
			return;

		if (FrontFaceStencilFunc != frontfunc || BackFaceStencilFunc != backfunc || StencilRef != ref || mask != StencilReadMask)
		{
			if (frontfunc == backfunc)
				glStencilFunc(frontfunc, ref, mask);
			else
			{
				if (FrontFaceStencilFunc != frontfunc || StencilRef != ref || mask != StencilReadMask)
					glStencilFuncSeparate(GL_FRONT,frontfunc, ref, mask);

				if (BackFaceStencilFunc != backfunc || StencilRef != ref || mask != StencilReadMask)
					glStencilFuncSeparate(GL_BACK, backfunc, ref, mask);
			}
		
			FrontFaceStencilFunc = frontfunc;
			BackFaceStencilFunc = backfunc;
			StencilRef = ref;
			StencilReadMask = mask;
		}
	}

	 
	void GLRenderEngine::GLEngineCache::CacheStencilOpFront(GLenum sfail, GLenum dpfail, GLenum dppass)
	{
		if (!StencilEnable)
			return;

		if (FrontFaceStencilDepthFailOp != dpfail || FrontFaceStencilFailOp != sfail || FrontFaceStencilPassOp != dppass)
		{
			glStencilOpSeparate(GL_FRONT, sfail, dpfail, dppass);
			FrontFaceStencilDepthFailOp = dpfail;
			FrontFaceStencilFailOp = sfail;
			FrontFaceStencilPassOp = dppass;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheStencilOpBack(GLenum sfail, GLenum dpfail, GLenum dppass)
	{
		if (!StencilEnable)
			return;

		if (BackFaceStencilDepthFailOp != dpfail || BackFaceStencilFailOp != sfail || BackFaceStencilPassOp != dppass)
		{
			glStencilOpSeparate(GL_BACK, sfail, dpfail, dppass);
			BackFaceStencilDepthFailOp = dpfail;
			BackFaceStencilFailOp = sfail;
			BackFaceStencilPassOp = dppass;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheStencilWriteMask(BYTE mask)
	{
		if (!StencilEnable)
			return;

		if (StencilWriteMask != mask)
		{
			glStencilMask(mask);
			StencilWriteMask = mask;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheFillMode(GLenum fillmode)
	{
		if (FillMode != fillmode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, fillmode);
			FillMode = fillmode;
		}

	}

	void GLRenderEngine::GLEngineCache::CacheCullMode(bool bEnable,GLenum cullmode)
	{
		if (bEnable != CullEnable)
		{
			CullEnable = bEnable;
			if (bEnable)
			{
				glEnable(GL_CULL_FACE);
				if (CullMode != cullmode)
				{
					glCullFace(cullmode);
					CullMode = cullmode;
				}
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}
		else if (bEnable && cullmode != CullMode)
		{
			glCullFace(cullmode);
			CullMode = cullmode;
		}
	}

	void GLRenderEngine::GLEngineCache::CacheFrontFace(GLenum ff)
	{
		if (FrontFace != ff)
		{
			FrontFace = ff;
			glFrontFace(ff);
		}
	}
	
	void  GLRenderEngine::GLEngineCache::CachePolygonOffset(int depthbias, float slopedepthbias)
	{
		if (depthbias != DepthBias || slopedepthbias != SlopeScaledDepthBias)
		{
			glPolygonOffset(slopedepthbias, (GLfloat)depthbias);
			SlopeScaledDepthBias = slopedepthbias;
			DepthBias = depthbias;
		}
	}

	void  GLRenderEngine::GLEngineCache::CacheDepthClip(bool bEnable)
	{
		if (DepthClipEnable != bEnable)
		{
			DepthClipEnable = bEnable;
			if (bEnable)
				glEnable(GL_DEPTH_CLAMP);
			else
				glDisable(GL_DEPTH_CLAMP);
		}

	}

	void  GLRenderEngine::GLEngineCache::CacheScissor(bool bEnable)
	{
		if (ScissorEnable != bEnable)
		{
			ScissorEnable = bEnable;
			if (bEnable)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
		}
	}

	void  GLRenderEngine::GLEngineCache::CacheMultiSample(bool bEnable)
	{
		if (MultisampleEnable != bEnable)
		{
			MultisampleEnable = bEnable;
			if (bEnable)
				glEnable(GL_MULTISAMPLE);
			else
				glDisable(GL_MULTISAMPLE);
		}
	}

	void  GLRenderEngine::GLEngineCache::CacheAntialiasedLine(bool bEnable)
	{
		if (AntialiasedLineEnable != bEnable)
		{
			AntialiasedLineEnable = bEnable;
			if (bEnable)
				glEnable(GL_LINE_SMOOTH);
			else
				glDisable(GL_LINE_SMOOTH);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheAlphaToCoverage(bool bEnable)
	{
		if (AlphaToCoverageEnable != bEnable)
		{
			AlphaToCoverageEnable = bEnable;
			if (bEnable)
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			else
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheBlendEnable(bool bEnable)
	{
		if (BlendEnable != bEnable)
		{
			BlendEnable = bEnable;
			if (bEnable)
				glEnable(GL_BLEND);
			else
				glDisable(GL_BLEND);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheBlendFunc(int index, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		if (!BlendEnable)
			return;

		if (index == -1 && (BlendDesc[0].SrcRGB != srcRGB || BlendDesc[0].DestRGB != destRGB || BlendDesc[0].SrcAlpha != srcAlpha || BlendDesc[0].DestAlpha != destAlpha))
		{
			BlendDesc[0].SrcRGB = srcRGB;
			BlendDesc[0].DestRGB = destRGB;
			BlendDesc[0].SrcAlpha = srcAlpha;
			BlendDesc[0].DestAlpha = destAlpha;
			glBlendFuncSeparate(srcRGB, destRGB, srcAlpha, destAlpha);
		}
		else if (index >=0 && (BlendDesc[index].SrcRGB != srcRGB || BlendDesc[index].DestRGB != destRGB || BlendDesc[index].SrcAlpha != srcAlpha || BlendDesc[index].DestAlpha != destAlpha))
		{
			BlendDesc[index].SrcRGB = srcRGB;
			BlendDesc[index].DestRGB = destRGB;
			BlendDesc[index].SrcAlpha = srcAlpha;
			BlendDesc[index].DestAlpha = destAlpha;
			glBlendFuncSeparatei(index, srcRGB, destRGB, srcAlpha, destAlpha);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheBlendEquation(int index, GLenum rgbMode,GLenum alphaMode)
	{
		if (!BlendEnable)
			return;

		if (index == -1 && (BlendDesc[0].RGBEquation != rgbMode || BlendDesc[0].AlphaEquation != alphaMode))
		{
			BlendDesc[0].RGBEquation = rgbMode;
			BlendDesc[0].AlphaEquation = alphaMode;
			glBlendEquationSeparate(rgbMode,alphaMode);
		}
		else if (index >= 0 && (BlendDesc[index].RGBEquation != rgbMode || BlendDesc[index].AlphaEquation != alphaMode))
		{
			BlendDesc[index].RGBEquation = rgbMode;
			BlendDesc[index].AlphaEquation = alphaMode;
			glBlendEquationSeparatei(index, rgbMode, alphaMode);
		}

	}

	void GLRenderEngine::GLEngineCache::CacheBlendFactor(float r, float g, float b, float alpha)
	{
		if (!BlendEnable)
			return;

		if (BlendFactor[0] != r || BlendFactor[1] != g || BlendFactor[2] != b || BlendFactor[3] != alpha)
		{
			BlendFactor[0] = r;
			BlendFactor[1] = g;
			BlendFactor[2] = b;
			BlendFactor[3] = alpha;
			glBlendColor(r, g, b, alpha);
		}
	}

	void GLRenderEngine::GLEngineCache::CacheTexBinding(GLuint first, const std::vector<GLuint>& bindingArray,const std::vector<GLuint>& sArray)
	{
		bool bTexChange = false;
		bool bSamplerChange = false;

		if (TexBindingBegin != first)
		{
			bTexChange = true;
			bSamplerChange = true;
		}
		else
		{
			if (bindingArray.size() != TexBindingArray.size())
				bTexChange = true;

			if (SamplerArray.size() != sArray.size())
				bSamplerChange = true;
		}
		
		
		if (!bTexChange)
		{
			for (size_t i = 0; i < bindingArray.size(); i++)
			{
				if (bindingArray[i] != TexBindingArray[i])
				{
					bTexChange = true;
					break;
				}
			}
		}

		if (!bSamplerChange)
		{
			for (size_t i = 0; i < sArray.size(); i++)
			{
				if (sArray[i] != SamplerArray[i])
				{
					bSamplerChange = true;
					break;
				}
			}	 
		}

		if (bTexChange)
		{
			TexBindingBegin = first;
			TexBindingArray.clear();
			std::copy(bindingArray.begin(), bindingArray.end(), std::back_inserter(TexBindingArray));
			glBindTextures(first, bindingArray.size(), bindingArray.data());
		}

		if (bSamplerChange)
		{
			SamplerArray.clear();
			std::copy(sArray.begin(), sArray.end(), std::back_inserter(SamplerArray));
			glBindSamplers(TexBindingBegin, sArray.size(), sArray.data());
			 
		}
	}*/
}