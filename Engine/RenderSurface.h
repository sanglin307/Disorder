#ifndef _DISORDER_RENDERSURFACE_H_
#define _DISORDER_RENDERSURFACE_H_

#include "Renderer.h"

namespace Disorder
{
	enum ESurfaceLocation
	{
		SL_ShaderResource,
		SL_DepthStencil,
		SL_RenderTarget1,
		SL_RenderTarget2,
		SL_RenderTarget3,
		SL_RenderTarget4,
		SL_RenderTarget5,
		SL_RenderTarget6,
		SL_RenderTarget7,
		SL_RenderTarget8
	};

	enum ESurfaceFlag
	{
		SF_ReadOnlyDepth = 0x01,
		SF_ReadOnlyStencil = 0x20
	};

	struct sRenderSurfaceDes
	{
		ESurfaceLocation Location;
		PixelFormat Format;
		unsigned int Flag;
		void * Handler;

		sRenderSurfaceDes()
			:Location(SL_RenderTarget1), Format(PF_UNKNOWN), Flag(0), Handler(0)
		{
		}

		bool IsRenderTarget()
		{
			return Location >= SL_RenderTarget1 && Location <= SL_RenderTarget8;
		}

		bool IsDepthStencil()
		{
			return Location == SL_DepthStencil;
		}

		bool IsShaderResource()
		{
			return Location == SL_ShaderResource;
		}
	};

	struct sRenderSurfaceObject
	{
		RenderTexturePtr Resource;
		std::vector<sRenderSurfaceDes> vDes;
	};

	class RenderSurface 
	{

	public:
		void AddSurface(const RenderTexturePtr& tex, const std::vector<sRenderSurfaceDes>& des);
		const std::vector<sRenderSurfaceDes>& GetSurface(const RenderTexturePtr& tex) const;

	private:
		//virtual void * GetHandle(ESurfaceLocation location);
		//RenderTexturePtr GetResource(ESurfaceLocation location);
		boost::unordered_map<RenderTexturePtr, sRenderSurfaceObject> _mapSurfaces;
	};
 
	class RenderGBuffer
	{
	public:
	 
		RenderSurfacePtr DepthStencilBuffer;
		RenderSurfacePtr RenderTargetBuffer;

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