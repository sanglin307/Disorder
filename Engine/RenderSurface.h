#ifndef _DISORDER_RENDERSURFACE_H_
#define _DISORDER_RENDERSURFACE_H_

 

namespace Disorder
{
	enum ESurfaceFlag
	{
		SF_ReadOnlyDepth = 0x1L,
		SF_ReadOnlyStencil = 0x2L,
		SF_AsCubeMap = 0x4L,
		SF_MultiSliceView = 0x8L,   // create multiple rendertarget or depth view for a texture array object.
	};

	enum ESurfaceViewType
	{
		SV_ShaderResource = 0x1L,
		SV_RenderTarget = 0x2L,
		SV_DepthStencil = 0x4L,
		SV_UnorderedAccess = 0x8L
	};

	class SurfaceView
	{
	public:
		virtual void* GetHandle(){ return 0; }

		ESurfaceViewType Type;
		RenderTexture* Resource;
		PixelFormat Format;
		unsigned int Flag;
		
	};

	enum ESurfaceLocation
	{
		SL_DepthStencil,
		SL_RenderTarget1,
		SL_RenderTarget2,
		SL_RenderTarget3,
		SL_RenderTarget4,
		SL_RenderTarget5,
		SL_RenderTarget6,
		SL_RenderTarget7,
		SL_RenderTarget8,
		SL_SurfaceLoactionMax
	};
 
	class RenderSurface : public RenderResource
	{
	public:
		SurfaceView* GetSurfaceView(ESurfaceLocation location);
		RenderSurface();
	protected:	
		std::vector<SurfaceView*> _surfacesViewArray;
	};
 
	class MainRenderTarget
	{
	public:
 
		MainRenderTarget(RenderSurface* renderTargetBuffer, SurfaceView* renderTargetShaderView, SurfaceView* renderTargetView,SurfaceView* depthBufferView)
		{
			RenderTargetSurface = renderTargetBuffer;
			RenderTargetShaderView = renderTargetShaderView;
			RenderTargetView = renderTargetView;
			DepthBufferView = depthBufferView;
		}

		RenderSurface* RenderTargetSurface;
		SurfaceView* RenderTargetShaderView;
		SurfaceView* RenderTargetView;
		SurfaceView* DepthBufferView;

 
	};

	class RenderGBuffer
	{
	public:
	 
		RenderSurface* RenderTargetBuffer;
		RenderSurface* MainTargetGDepth;

		SurfaceView* DepthShaderView;
		SurfaceView* DepthBufferView;
		
		SurfaceView* BasicColorShaderView;
		SurfaceView* BasicColorTargetView;

		SurfaceView* NormalDataShaderView;
		SurfaceView* NormalDataTargetView;

		SurfaceView* SpecularDataShaderView;
		SurfaceView* SpecularDataTargetView;

		virtual void UpdateShaderProperty();

		virtual void DebugVisual();

		RenderGBuffer(unsigned int width, unsigned int height);

	protected:
		
		ShaderProperty* _GBufferDepthTexture;
		ShaderProperty* _GBufferColorSpecIntTexture;
		ShaderProperty* _GBufferNormalTexture;
		ShaderProperty* _GBufferSpecPowTexture;

		ShaderProperty* _SurfaceVisTex;
		ShaderProperty* _SurfaceSampler;

		RenderEffect* _GBufferVisualEffect;

		SimpleTile _DepthVisTile;
		SimpleTile _ColorVisTile;
		SimpleTile _NormalVisTile;
		SimpleTile _SpecPowVisTile;
	};

	class RenderSurfaceCache
	{
	public:
		RenderSurfaceCache();
		~RenderSurfaceCache();
		MainRenderTarget* MainTarget;
		RenderGBuffer* GBuffer;
		ShadowMap* ShadowMapBuffer;
	};
 
}


#endif