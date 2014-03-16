#include "GLRenderInclude.h"


namespace Disorder
{

	GLRenderEnginePtr GLRenderEngine::Create()
	{
		GLRenderEngine *pEngine = new GLRenderEngine;
		return GLRenderEnginePtr(pEngine);
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
		if (_hRC)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(_hRC);
			_hRC = 0;
		}
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