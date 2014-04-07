#ifndef _DISORDER_GLRENDERENGINE_H_
#define _DISORDER_GLRENDERENGINE_H_

namespace Disorder
{
 
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

		virtual void ClearRenderTarget(const RenderSurfacePtr& renderTarget, const glm::vec4& color);
		virtual void ClearDepthStencil(const RenderSurfacePtr& depthBuffer, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil);
		virtual void SetRenderTarget(const std::vector<RenderSurfacePtr>& renderTarget, const RenderSurfacePtr& depthStencil, bool useReadOnlyDepthStencil = false);
		virtual void SetRenderTarget(const RenderSurfacePtr& renderTarget, const RenderSurfacePtr& depthStencil, bool useReadOnlyDepthStencil = false);
		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout);
		virtual void SetPrimitiveTopology(TopologyType topologyType);

		virtual void SetEffect(RenderEffectPtr const& effect);
		virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation);
		virtual void Draw(unsigned int vertexCount, unsigned int startVertexLocation);

		virtual void* Map(RenderBufferPtr const& buffer, BufferAccess bufferAccess);
		virtual void UnMap(RenderBufferPtr const& buffer);
		virtual void UpdateSubresource(RenderBufferPtr const& buffer, void* pSrcData, unsigned int srcDataSize);

		virtual void SaveRenderSurface(RenderSurfacePtr const& surface, std::string const& fileName);

		static GLenum GetPixelFormat(PixelFormat format, GLenum &glFormat, GLenum &glType);
		static GLint GetGLAddressMode(TextureAddressMode addrMode);
		static GLint GetGLComparisonFunc(ComparisonFunc func);

	protected:
		virtual void SetBlendState(BlendStatePtr const& blendState);
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState);
		virtual void SetDepthStencilState(DepthStencilStatePtr const& depthStencilState);
 
		GLRenderEngine();
		bool CreateGLContext(HWND window);
		void LoadShaderIncludeFiles();
		HGLRC _hRC;
		HDC _hDC;
		int _mainVersion;
		int _subVersion;
	};


}

#endif