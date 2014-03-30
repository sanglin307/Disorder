#include "GLRenderInclude.h"


namespace Disorder
{

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
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		HDC dc = GetDC(window);
		int nPixelFormat = ChoosePixelFormat(dc, &pfd);

		if (nPixelFormat == 0) 
			return false;

		BOOL bResult = SetPixelFormat(dc, nPixelFormat, &pfd);

		if (!bResult) 
			return false;

		HGLRC tempContext = wglCreateContext(dc);
		wglMakeCurrent(dc, tempContext);
 
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			MessageBox(NULL, L"GLEW is not initialized!", L"GLRenderInit", 0);
			return false;
		}

		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			WGL_CONTEXT_FLAGS_ARB, 0,
			0
		};

		if (wglewIsSupported("WGL_ARB_create_context") == 1)
		{
			_hRC = wglCreateContextAttribsARB(dc, 0, attribs);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempContext);
			wglMakeCurrent(dc, _hRC);
		}
		else
		{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
			_hRC = tempContext;
		}

		//Checking GL version
		const GLubyte *GLVersionString = glGetString(GL_VERSION);

		//Or better yet, use the GL3 way to get the version number
		glGetIntegerv(GL_MAJOR_VERSION, &_mainVersion);
		glGetIntegerv(GL_MINOR_VERSION, &_subVersion);

		if (!_hRC) 
			return false;

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

	}
	void GLRenderEngine::OnDrawEnd()
	{

	}

	void GLRenderEngine::AdjustProjMatrix(const glm::mat4 &matrix)
	{

	}
 
	void GLRenderEngine::ClearRenderTarget(const RenderSurfacePtr& renderTarget, const glm::vec4& color)
	{

	}
	void GLRenderEngine::ClearDepthStencil(const RenderSurfacePtr& depthBuffer, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil)
	{

	}
	void GLRenderEngine::SetRenderTarget(const std::vector<RenderSurfacePtr>& renderTarget, const RenderSurfacePtr& depthStencil, bool useReadOnlyDepthStencil)
	{

	}
	void GLRenderEngine::SetRenderTarget(const RenderSurfacePtr& renderTarget, const RenderSurfacePtr& depthStencil, bool useReadOnlyDepthStencil)
	{

	}
	void GLRenderEngine::SetRenderLayout(RenderLayoutPtr const& renderLayout)
	{

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

	void GLRenderEngine::SaveRenderSurface(RenderSurfacePtr const& surface, std::string const& fileName)
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









}