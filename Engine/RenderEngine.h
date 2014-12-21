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
		virtual ~RenderEngine(){};
		
		virtual void OnFrameBegin(){};
		virtual void OnFrameEnd(){};

		virtual void ClearRenderSurface(RenderSurface* renderSurface, const glm::vec4& color, bool bClearDepth, float depth, bool bClearStencil, unsigned char stencil,int sliceIndex = -1) = 0;
 
		virtual void SetViewport(float width, float height, float minDepth, float maxDepth, float topX, float topY) = 0;

		virtual void SetRenderTarget(RenderSurface* renderTarget,int sliceIndex = -1,bool useReadOnlyDepthStencil = false) = 0;
		virtual void SetRenderLayout(RenderLayout* renderLayout) = 0;
		virtual void SetPrimitiveTopology(TopologyType topologyType) = 0;

		virtual void SetEffect(RenderEffect* effect) = 0;
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation) = 0;
		virtual void Draw(unsigned int vertexCount,unsigned int startVertexLocation) = 0;

		virtual void* Map(RenderBuffer* buffer,BufferAccess bufferAccess) = 0;
		virtual void UnMap(RenderBuffer* buffer) = 0;
		virtual void UpdateSubresource(RenderBuffer* buffer,void* pSrcData,unsigned int srcDataSize) = 0;
		virtual void CopyTexture2D(RenderTexture2D* srcTexture, RenderTexture2D* dstTexture) = 0;
		virtual void SaveSurfaceView(SurfaceView* surface,std::string const& fileName){};

        BlendState* CachedBlendState;
		float CachedBlendFactor[4];
		unsigned int CachedBlendSampleMask;
        RasterizeState* CachedRasterizeState;
		DepthStencilState* CachedDepthStencilState;
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

		virtual void SetBlendState(BlendState* blendState) = 0;
		virtual void SetRasterizeState(RasterizeState* rasterizeState)=0;
		virtual void SetDepthStencilState(DepthStencilState* depthStencilState) = 0;

	};

	
 
}


#endif