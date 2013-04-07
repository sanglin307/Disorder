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

		virtual void Draw(std::list<RendererPtr> const& renderList) = 0;

		virtual RenderTargetPtr CreateRenderTarget(RenderConfig const& renderConfig,void *hWnd) = 0;

		virtual void SetBlendState(BlendStatePtr const& blendState) = 0;
		virtual void SetRasterizeState(RasterizeStatePtr const& rasterizeState)=0;
		virtual void SetRenderLayout(RenderLayoutPtr const& renderLayout) = 0;
		//virtual void UpdateMVPMatrix(RenderEffectPtr const& effect, Matrix4 const& worldMatrix,Matrix4 const& viewMatrix,Matrix4 const& projMatrix){};
		virtual void SetEffect(RenderEffectPtr const& effect) = 0;
		virtual void DrawIndexed(unsigned int indexCount,unsigned int startIndexLocation,int baseVertexLocation) = 0;
		virtual void* Map(RenderBufferPtr const& buffer,BufferAccess bufferAccess) = 0;
		virtual void UnMap(RenderBufferPtr const& buffer) = 0;

        BlendStatePtr CachedBlendState;
        RasterizeStatePtr CachedRasterizeState;


		RenderResourceManagerPtr ResourceManager;
	};
 
}


#endif