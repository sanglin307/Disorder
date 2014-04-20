#ifndef _DISORDER_RENDERSURFACE_H_
#define _DISORDER_RENDERSURFACE_H_

#include "Renderer.h"

namespace Disorder
{
	enum ESurfaceFlag
	{
		SF_ReadOnlyDepth = 0x1L,
		SF_ReadOnlyStencil = 0x2L
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
		RenderTexturePtr Resource;
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
		SurfaceViewPtr GetSurfaceView(ESurfaceLocation location);

	protected:
		RenderSurface();
		std::vector<SurfaceViewPtr> _surfacesViewArray;
	};
 
	class RenderGBuffer
	{
	public:
	 
		RenderSurfacePtr RenderTargetBuffer;
		RenderSurfacePtr MainTargetGDepth;

		SurfaceViewPtr DepthShaderView;
		SurfaceViewPtr DepthBufferView;
		
		SurfaceViewPtr BasicColorShaderView;
		SurfaceViewPtr BasicColorTargetView;

		SurfaceViewPtr NormalDataShaderView;
		SurfaceViewPtr NormalDataTargetView;

		SurfaceViewPtr SpecularDataShaderView;
		SurfaceViewPtr SpecularDataTargetView;

		virtual void UpdateShaderProperty();

		virtual void DebugVisual();

		static RenderGBufferPtr Create(unsigned int width, unsigned int height);

	protected:
		RenderGBuffer(unsigned int width, unsigned int height);

		ShaderPropertyPtr _GBufferDepthTexture;
		ShaderPropertyPtr _GBufferColorSpecIntTexture;
		ShaderPropertyPtr _GBufferNormalTexture;
		ShaderPropertyPtr _GBufferSpecPowTexture;
		ShaderPropertyPtr _GBufferPointSampler;
		ShaderPropertyPtr _SurfaceVisTex;
		ShaderPropertyPtr _SurfaceSampler;

		RenderEffectPtr _GBufferVisualEffect;

		SimpleTile _DepthVisTile;
		SimpleTile _ColorVisTile;
		SimpleTile _NormalVisTile;
		SimpleTile _SpecPowVisTile;


	};

	class RenderSurfaceCache
	{
	public:
		RenderSurfacePtr MainTarget;
		RenderGBufferPtr GBuffer;

		static RenderSurfaceCachePtr Create();
	private:
		RenderSurfaceCache(){}
	};
 
}


#endif