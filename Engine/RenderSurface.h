#ifndef _DISORDER_RENDERSURFACE_H_
#define _DISORDER_RENDERSURFACE_H_

#include "Renderer.h"

namespace Disorder
{
	enum RenderSurfaceUsage
	{
		RSU_RenderTarget = 0x01,
		RSU_DepthStencil = 0x02,
		RSU_ShaderResource = 0x04,
	};

	class RenderSurface
	{
	public:
 
		virtual void* GetLowInterface(RenderSurfaceUsage usage)
		{
			return 0;
		}

		RenderTexture1DPtr Tex1DResource;
		RenderTexture2DPtr Tex2DResource;
		RenderTexture3DPtr Tex3DResource;

	    unsigned int Usage;
	};
 
	class RenderGBuffer
	{
	public:
		RenderSurfacePtr DepthStencilBuffer;
		RenderSurfacePtr BasicColorBuffer;
		RenderSurfacePtr NormalDataBuffer;
		RenderSurfacePtr SpecularDataBuffer;

		virtual void UpdateShaderProperty();

		virtual void DebugVisual(){}

	protected:
		RenderGBuffer();

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
		RenderSurfacePtr RenderTarget;
		RenderSurfacePtr DepthStencilBuffer;
		RenderGBufferPtr GBuffer;

		virtual void InitGBuffer(unsigned int width,unsigned int height) = 0;

	};
 
}


#endif