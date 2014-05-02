#ifndef _DISORDER_GLRENDERENGINE_H_
#define _DISORDER_GLRENDERENGINE_H_

namespace Disorder
{
 
	struct sGLProfile
	{
		GLint UniformBufferMaxBindings;
	};

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
		~GLRenderEngine();

		static GLRenderEnginePtr Create();

		virtual void Init();
		virtual void Exit();

		virtual void OnFrameBegin();
		virtual void OnFrameEnd();
 
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
		static GLenum GetGLStencilOp(StencilOperation sop);
		static GLenum GetGLFillMode(RenderFillMode fm);
		static GLenum GetGLCullMode(RenderCullMode cm);
		static GLenum GetGLBlendOp(BlendOperation blendOp);
		static GLenum GetGLBlendFunc(BlendOptions blendOptions);

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

		struct sGLCacheBlend
		{		
			GLenum SrcRGB;
			GLenum DestRGB;
			GLenum SrcAlpha;
			GLenum DestAlpha;

			GLenum RGBEquation;
			GLenum AlphaEquation;

			sGLCacheBlend()
			{
				SrcRGB = SrcAlpha = GL_ONE;
				DestAlpha = DestRGB = GL_ZERO;
				RGBEquation = AlphaEquation = GL_FUNC_ADD;
			}
		};

		class GLEngineCache
		{
		private:

			GLenum PrimitiveTopology;
			GLuint ShaderProgram;

			GLuint FrameBufferObject;
			GLuint VertexArrayObject;
			
			GLuint VertexBufferObject;
			GLuint IndexBufferObject;
			GLuint IndexElementSize;

			GLuint UniformBufferObject;
			GLuint UniformBufferBindingPoint;

			bool       DepthEnable;
			bool       DepthWrite;
			GLenum     DepthFunc;

			bool  StencilEnable;
			BYTE  StencilReadMask;
			BYTE  StencilWriteMask;
			GLint StencilRef;

			GLenum   FrontFaceStencilFailOp;
			GLenum   FrontFaceStencilDepthFailOp;
			GLenum   FrontFaceStencilPassOp;
			GLenum   FrontFaceStencilFunc;

			GLenum   BackFaceStencilFailOp;
			GLenum   BackFaceStencilDepthFailOp;
			GLenum   BackFaceStencilPassOp;
			GLenum   BackFaceStencilFunc;

			GLenum   FillMode;
			GLenum   CullMode;
			bool     CullEnable;
			GLenum   FrontFace;
			int      DepthBias;
			float    SlopeScaledDepthBias;

			bool     DepthClipEnable;
			bool     ScissorEnable;
			bool     MultisampleEnable;
			bool     AntialiasedLineEnable;

			bool     AlphaToCoverageEnable;
			
			bool     BlendEnable;
			sGLCacheBlend BlendDesc[8];
			float    BlendFactor[4];

			int      TexBindingBegin;
			std::vector<GLuint>  TexBindingArray;
			std::vector<GLuint>  SamplerArray;
			GLenum  SingleDrawBuffer;
			std::vector<GLenum>  MultiDrawBuffers;

		public:
			inline void CacheFrameBufferObject(GLuint fbo);
			inline void CacheSingleDrawBuffer(GLenum buf);
			inline void CacheMultiDrawBuffers(const std::vector<GLenum>& buffers);

			inline void CacheVertexArrayObject(GLuint vao, GLuint ies);
			inline void CacheVertexBufferObject(GLuint vbo);
			inline void CacheIndexBufferObject(GLuint ibo, GLuint ies);
			inline void CacheUniformBufferObject(GLuint ubo, GLuint bindingPoint);

			inline void CacheShaderProgram(GLuint sp);

			inline void CacheDepthEnable(bool bEnable);
			inline void CacheDepthWrite(bool bWrite);
			inline void CacheDepthFunc(GLenum eFunc);

			inline void CacheStencilEnable(bool bEnable);
			inline void CacheStencilFunc(GLenum frontfunc, GLenum backfunc, GLint ref, BYTE mask);
			inline void CacheStencilOpFront(GLenum sfail, GLenum dpfail, GLenum dppass);
			inline void CacheStencilOpBack(GLenum sfail, GLenum dpfail, GLenum dppass);
			inline void CacheStencilWriteMask(BYTE mask);

			inline void CacheFillMode(GLenum fillmode);
			inline void CacheCullMode(bool bEnable, GLenum cullmode);
			inline void CacheFrontFace(GLenum ff);

			inline void CachePolygonOffset(int depthbias, float slopedepthbias);

			inline void CacheDepthClip(bool bEnable);
			inline void CacheScissor(bool bEnable);
			inline void CacheMultiSample(bool bEnable);
			inline void CacheAntialiasedLine(bool bEnable);

			inline void CacheAlphaToCoverage(bool bEnable);
		
			inline void CacheBlendEnable(bool bEanble);
			inline void CacheBlendFunc(int index, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha); // index = -1 for all buffer
			inline void CacheBlendEquation(int index, GLenum rgbMode, GLenum alphaMode);
			inline void CacheBlendFactor(float r, float g, float b, float alpha);

			inline void CacheTexBinding(GLuint first, const std::vector<GLuint>& bindingArray, const std::vector<GLuint>& samplerArray);

			inline void SetPrimitiveTopology(GLenum pt)
			{
				PrimitiveTopology = pt;
			}

			inline GLenum GetPrimitiveTopology()
			{
				return PrimitiveTopology;
			}

			inline GLuint GetIndexElementSize()
			{
				return IndexElementSize;
			}


			GLEngineCache()
			{
				PrimitiveTopology = GL_POINTS;
				ShaderProgram = 0 ;

				FrameBufferObject = 0;
				VertexArrayObject = 0;

				VertexBufferObject = 0;
				IndexBufferObject = 0;
				IndexElementSize = 0;

				UniformBufferObject = 0;
				UniformBufferBindingPoint = 0;

				DepthEnable = true;
				DepthWrite = true;
				DepthFunc = GL_LESS;

				StencilEnable = false;
				StencilReadMask = 0xff;
				StencilWriteMask = 0xff;
				StencilRef = 0;

				FrontFaceStencilFailOp = GL_KEEP;
				FrontFaceStencilDepthFailOp = GL_KEEP;
				FrontFaceStencilPassOp = GL_KEEP;
				FrontFaceStencilFunc = GL_ALWAYS;

				BackFaceStencilFailOp = GL_KEEP;
				BackFaceStencilDepthFailOp = GL_KEEP;
				BackFaceStencilPassOp = GL_KEEP;
				BackFaceStencilFunc = GL_ALWAYS;

				FillMode = GL_FILL;
				CullEnable = true;
				CullMode = GL_BACK;

				FrontFace = GL_CCW;

				DepthBias = 0;
				SlopeScaledDepthBias = 0.f;

				DepthClipEnable = true;
				ScissorEnable = false;
			    MultisampleEnable = false;
				AntialiasedLineEnable = false;

				AlphaToCoverageEnable = false;
				BlendEnable = false;
				BlendFactor[0] = BlendFactor[1] = BlendFactor[2] = BlendFactor[3] = 1.0f;

				TexBindingBegin = 0;

				SingleDrawBuffer = GL_BACK;
			}

			void Init();
		};

		virtual void SetBlendState(BlendStatePtr const& blendState);
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState);
		virtual void SetDepthStencilState(DepthStencilStatePtr const& depthStencilState);
 
		static GLenum GetPlatformTopology(TopologyType tType);
		static GLenum GetBufferAccessFlag(BufferAccess ba);

		GLRenderEngine();
		bool CreateGLContext(HWND window);
		void CreateMainTarget();

		void LoadShaderIncludeFiles();
		void LoadShaderIncludeForPath(const boost::filesystem::path& p);
		void LoadGLExtensions();
		void LoadGLProfile();
		
		std::vector<std::string> _vGLExtensions;

		HGLRC _hRC;
		HDC _hDC;
		int _mainVersion;
		int _subVersion;

		GLDebugLayerPtr _debugLayer;

		GLEngineCache _renderCache;

		sGLProfile _profile;


	};


}

#endif