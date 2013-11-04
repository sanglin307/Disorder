#ifndef _DISORDER_RENDERENGINE_H_
#define _DIRORDER_RENDERENGINE_H_


namespace Disorder
{
	class RenderEngine
	{
	public:
		virtual void Init() = 0;
		virtual void Exit() = 0;
		
		virtual void OnDrawBegin(){};
		virtual void OnDrawEnd(){};

		virtual void AdjustProjMatrix(Matrix4 &matrix){};

		virtual void CreateViewport(void *hWnd) = 0;

		virtual void ClearRenderTarget(const RenderSurfacePtr& renderTarget,const Vector4& color ) = 0;
		virtual void ClearDepthStencil(const RenderSurfacePtr& depthBuffer,bool bClearDepth,float depth,bool bClearStencil,unsigned char stencil) = 0;
		virtual void SetRenderTarget(const RenderSurfacePtr& renderTarget,const RenderSurfacePtr& depthStencil) = 0;
		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout) = 0;
		
		virtual void SetEffect(RenderEffectPtr const& effect) = 0;
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation) = 0;
		virtual void* Map(RenderBufferPtr const& buffer,BufferAccess bufferAccess) = 0;
		virtual void UnMap(RenderBufferPtr const& buffer) = 0;
		virtual void UpdateSubresource(RenderBufferPtr const& buffer,void* pSrcData,unsigned int srcDataSize) = 0;

        BlendStatePtr CachedBlendState;
        RasterizeStatePtr CachedRasterizeState;
		DepthStencilStatePtr CachedDepthStencilState;


		RenderResourceManagerPtr ResourceManager;
		MaterialParameterManagerPtr ParameterManager;

	protected:
		virtual void SetBlendState(BlendStatePtr const& blendState) = 0;
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState)=0;
		virtual void SetDepthStencilState(DepthStencilStatePtr const& depthStencilState) = 0;

	};
 
}


#endif