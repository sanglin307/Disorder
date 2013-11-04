#ifndef _DISORDER_RENDERSURFACE_H_
#define _DISORDER_RENDERSURFACE_H_

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

		virtual void Create(const RenderTexture2DPtr& resource,unsigned int usage)
		{
			Tex2DResource = resource;
			Usage = usage;
		}

		virtual void* GetLowInterface(RenderSurfaceUsage usage)
		{
			return 0;
		}

		RenderTexture1DPtr Tex1DResource;
		RenderTexture2DPtr Tex2DResource;
		RenderTexture3DPtr Tex3DResource;

	    unsigned int Usage;
	};

	class RenderSurfaceManager
	{
	public:

		RenderSurfacePtr RenderTarget;
		RenderSurfacePtr DepthStencilBuffer;

	};

	extern RenderSurfaceManager GRenderSurface;

}


#endif