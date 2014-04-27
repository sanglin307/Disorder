#ifndef _DISORDER_RENDERENGINE_H_
#define _DIRORDER_RENDERENGINE_H_


namespace Disorder
{
	enum RenderEngineType
	{
		RET_DirectX,
		RET_OpenGL
	};

	class RenderEngine
	{
	public:
		virtual void Init() = 0;
		virtual void Exit() = 0;
		
		virtual void OnFrameBegin(){};
		virtual void OnFrameEnd(){};

		virtual void AdjustProjMatrix(const glm::mat4 &matrix){};

		virtual void CreateViewport(void *hWnd) = 0;

		virtual void ClearRenderSurface(const RenderSurfacePtr& renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil) = 0;
 
		virtual void SetRenderTarget(const RenderSurfacePtr& renderTarget,bool useReadOnlyDepthStencil = false) = 0;
		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout) = 0;
		virtual void SetPrimitiveTopology(TopologyType topologyType) = 0;

		virtual void SetEffect(RenderEffectPtr const& effect) = 0;
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation) = 0;
		virtual void Draw(unsigned int vertexCount,unsigned int startVertexLocation) = 0;

		virtual void* Map(RenderBufferPtr const& buffer,BufferAccess bufferAccess) = 0;
		virtual void UnMap(RenderBufferPtr const& buffer) = 0;
		virtual void UpdateSubresource(RenderBufferPtr const& buffer,void* pSrcData,unsigned int srcDataSize) = 0;

		virtual void SaveSurfaceView(SurfaceViewPtr const& surface,std::string const& fileName){};

        BlendStatePtr CachedBlendState;
		float CachedBlendFactor[4];
		unsigned int CachedBlendSampleMask;
        RasterizeStatePtr CachedRasterizeState;
		DepthStencilStatePtr CachedDepthStencilState;
		unsigned int CachedStencilRef;
		TopologyType CachedTopology;
 
		static size_t ComputePixelSizeBits(PixelFormat format);
		static size_t ComputeBlockSizeBits(PixelFormat format);
		static size_t ComputePixelChannel(PixelFormat format); 
		static void ComputePixelColorAlphaSize(PixelFormat format, int& colorSize, int& alphaSize);
		static void ComputeDepthStencilSize(PixelFormat format, int& depth, int& stencil);
		static bool  IsCompressed(PixelFormat format);
		static unsigned int GetTextureFormatDimensionAlignment(PixelFormat format);
		static bool IsDepthStencilFormat(PixelFormat format);
 
		static int GetTriangleCountFromTopology(TopologyType topology, int indexCount);

	protected:

		virtual void SetBlendState(BlendStatePtr const& blendState) = 0;
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState)=0;
		virtual void SetDepthStencilState(DepthStencilStatePtr const& depthStencilState) = 0;

	};
 
}


#endif