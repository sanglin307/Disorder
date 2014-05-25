#ifndef _DISORDER_RENDERSTATE_H_
#define _DISORDER_RENDERSTATE_H_


namespace Disorder
{
 
	typedef struct _RasterizeDesc
	{
		_RasterizeDesc()
		:FillMode(RFM_Solid), CullMode(RCM_CullBack), FrontCounterClockwise(true), DepthBias(0), DepthBiasClamp(0.0f), SlopeScaledDepthBias(0.0f),
			DepthClipEnable(true),ScissorEnable(false),MultisampleEnable(false),AntialiasedLineEnable(false)
		{
		}
		RenderFillMode FillMode;
        RenderCullMode CullMode;
        bool           FrontCounterClockwise;
        int            DepthBias;
        float          DepthBiasClamp;
        float          SlopeScaledDepthBias;
        bool           DepthClipEnable;
        bool           ScissorEnable;
        bool           MultisampleEnable;
        bool           AntialiasedLineEnable;

	}RasterizeDesc;

	typedef struct _BlendDesc 
	{
		_BlendDesc()
		:BlendEnable(false),SrcBlend(BLEND_ONE),DestBlend(BLEND_ZERO),BlendOp(BLEND_OP_ADD),
		 SrcBlendAlpha(BLEND_ONE),DestBlendAlpha(BLEND_ZERO),BlendOpAlpha(BLEND_OP_ADD),RenderTargetWriteMask(COLOR_WRITE_ENABLE_ALL)
		{
		}
		bool           BlendEnable;
		BlendOptions   SrcBlend;
		BlendOptions   DestBlend;
		BlendOperation BlendOp;
		BlendOptions   SrcBlendAlpha;
		BlendOptions   DestBlendAlpha;
		BlendOperation BlendOpAlpha;
		unsigned char  RenderTargetWriteMask;

    } BlendDesc;

	typedef struct _DepthStencilDesc
	{
		_DepthStencilDesc()
		:DepthEnable(true),DepthWrite(true),DepthFunc(CF_Less),StencilEnable(false),StencilReadMask(0xff),StencilWriteMask(0xff),
			FrontFaceStencilFailOp(STENCIL_OP_KEEP),FrontFaceStencilDepthFailOp(STENCIL_OP_KEEP),FrontFaceStencilPassOp(STENCIL_OP_KEEP),FrontFaceStencilFunc(CF_Always),
			BackFaceStencilFailOp(STENCIL_OP_KEEP),BackFaceStencilDepthFailOp(STENCIL_OP_KEEP),BackFaceStencilPassOp(STENCIL_OP_KEEP),BackFaceStencilFunc(CF_Always)
		{
		}

		bool               DepthEnable;
        bool               DepthWrite;  // if write to depth buffer
        ComparisonFunc     DepthFunc;

        bool               StencilEnable;
        BYTE               StencilReadMask;
        BYTE               StencilWriteMask;

		StencilOperation   FrontFaceStencilFailOp;
        StencilOperation   FrontFaceStencilDepthFailOp;
        StencilOperation   FrontFaceStencilPassOp;
        ComparisonFunc     FrontFaceStencilFunc;

		StencilOperation   BackFaceStencilFailOp;
        StencilOperation   BackFaceStencilDepthFailOp;
        StencilOperation   BackFaceStencilPassOp;
        ComparisonFunc     BackFaceStencilFunc;

	} DepthStencilDesc;

	typedef struct _SamplerDesc
	{
		_SamplerDesc()
		:Filter(SF_Min_Mag_Mip_Linear), AddressU(TAM_Clamp), AddressV(TAM_Clamp), AddressW(TAM_Clamp),
		MipLODBias(0.f), MaxAnisotropy(1), CompareFunc(CF_None), CompareTypeSampler(false)
		{
			BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 1.0f;
			MinLOD = 0.f;
			MaxLOD = std::numeric_limits<float>::max();
		}
		SamplerFilter Filter;
		TextureAddressMode AddressU;
		TextureAddressMode AddressV;
		TextureAddressMode AddressW;
		float MipLODBias;
		unsigned int MaxAnisotropy;
		ComparisonFunc CompareFunc;
		float BorderColor[4];
		float MinLOD;
		float MaxLOD;
		bool CompareTypeSampler;
	} SamplerDesc;

	class RenderState
	{
	public:
		virtual void* GetHandle(){ return 0; }
	};

	class RasterizeState : public RenderState
	{
	public:
		RasterizeDesc Desc;
	protected:
		RasterizeState(){}
	};

	class DepthStencilState : public RenderState
	{
	public:
		DepthStencilDesc Desc;
		unsigned int StencilRef;

	protected:
		DepthStencilState(){}
	};

	class BlendState : public RenderState
	{
    public:
         float BlendFactor[4];
         unsigned int SampleMask;
		 BlendDesc Desc[8];
		 bool AlphaToCoverageEnable;
		 bool IndependentBlendEnable;

	protected:
		 BlendState()
         {
             BlendFactor[0] = BlendFactor[1] = BlendFactor[2] = BlendFactor[3] = 1.0f;
             SampleMask =  0xffffffff;
			 AlphaToCoverageEnable = false;
			 IndependentBlendEnable = false;
         }
	};

	class SamplerState : public RenderState
	{
	public:
		SamplerDesc Desc;
	protected:
		SamplerState(){}
	};


}



#endif