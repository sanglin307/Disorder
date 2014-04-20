#ifndef _DISORDER_GLRENDERENGINE_H_
#define _DISORDER_GLRENDERENGINE_H_

namespace Disorder
{
 
	class GLDebugLayer
	{
	public:
		void Init();
		void Exit();

		static GLDebugLayerPtr Create();
	private:
		GLDebugLayer(){};

		static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
		{
			reinterpret_cast<GLDebugLayer*>(userParam)->OnDebugMessage(source, type, id, severity, length, message);
		}

		void OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message);

		std::map<GLenum, std::string> _mapSourceInfo;
		std::map<GLenum, std::string> _mapTypeInfo;
		std::map<GLenum, std::string> _mapSeverityInfo;

	};

	class GLRenderEngine : public RenderEngine
	{

	public:

		static GLRenderEnginePtr Create();

		virtual void Init();
		virtual void Exit();

		virtual void OnDrawBegin();
		virtual void OnDrawEnd();

		virtual void AdjustProjMatrix(const glm::mat4 &matrix);

		virtual void CreateViewport(void *hWnd);

		virtual void ClearRenderSurface(const RenderSurfacePtr& renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil);

		virtual void SetRenderTarget(const RenderSurfacePtr& renderTarget, bool useReadOnlyDepthStencil = false);
		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout);
		virtual void SetPrimitiveTopology(TopologyType topologyType);

		virtual void SetEffect(RenderEffectPtr const& effect);
		virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation);
		virtual void Draw(unsigned int vertexCount, unsigned int startVertexLocation);

		virtual void* Map(RenderBufferPtr const& buffer, BufferAccess bufferAccess);
		virtual void UnMap(RenderBufferPtr const& buffer);
		virtual void UpdateSubresource(RenderBufferPtr const& buffer, void* pSrcData, unsigned int srcDataSize);

		virtual void SaveSurfaceView(SurfaceViewPtr const& surface, std::string const& fileName);

		static GLenum GetPixelFormat(PixelFormat format, GLenum &glFormat, GLenum &glType);
		static GLint GetGLAddressMode(TextureAddressMode addrMode);
		static GLint GetGLComparisonFunc(ComparisonFunc func);

		const int GetMainVersion() const
		{
			return _mainVersion;
		}

		const int GetSubVersion() const
		{
			return _subVersion;
		}

		bool IsVersionSupported(int mainver, int subver)
		{
			if (_mainVersion > mainver)
				return true;

			if (_mainVersion < mainver)
				return false;

			if (_subVersion >= subver)
				return true;

			return false;
		}

		bool IsExtensionSupported(std::string const& extension);

	protected:

		struct sGLEngineCache
		{
			GLenum PrimitiveTopology;
			GLuint ShaderProgram;

			GLuint FrameBufferObject;
			GLuint VertexArrayObject;
			
			GLuint VertexBufferObject;
			GLuint IndexBufferObject;
			GLuint IndexElementSize;

			GLuint UniformBufferObject;

			void CacheFrameBufferObject(GLuint fbo);
			void CacheVertexArrayObject(GLuint vao);
			void CacheVertexBufferObject(GLuint vbo);
			void CacheIndexBufferObject(GLuint ibo,GLuint ies);
			void CacheUniformBufferObject(GLuint ubo);

			void CacheShaderProgram(GLuint sp);

			sGLEngineCache()
				:FrameBufferObject(0)
			{}
		};

		virtual void SetBlendState(BlendStatePtr const& blendState);
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState);
		virtual void SetDepthStencilState(DepthStencilStatePtr const& depthStencilState);
 
		static GLenum GetPlatformTopology(TopologyType tType);
		static GLenum GetBufferAccessFlag(BufferAccess ba);

		GLRenderEngine();
		bool CreateGLContext(HWND window);
		void LoadShaderIncludeFiles();
		void LoadGLExtensions();
		std::vector<std::string> _vGLExtensions;

		HGLRC _hRC;
		HDC _hDC;
		int _mainVersion;
		int _subVersion;

		GLDebugLayerPtr _debugLayer;

		sGLEngineCache _renderCache;



	};


}

#endif