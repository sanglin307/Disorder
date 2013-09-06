#ifndef _DISORDER_RENDERSTATE_H_
#define _DISORDER_RENDERSTATE_H_


namespace Disorder
{
 
	typedef struct _RasterizeDesc
	{
		_RasterizeDesc()
			:FillMode(RFM_Solid),CullMode(RCM_CullBack),FrontCounterClockwise(false),DepthBias(0),DepthBiasClamp(0.0f),SlopeScaledDepthBias(0.0f),
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
		:TargetIndex(0),BlendEnable(false),SrcBlend(BLEND_ONE),DestBlend(BLEND_ZERO),BlendOp(BLEND_OP_ADD),
		 SrcBlendAlpha(BLEND_ONE),DestBlendAlpha(BLEND_ZERO),BlendOpAlpha(BLEND_OP_ADD),RenderTargetWriteMask(COLOR_WRITE_ENABLE_ALL)
		{
		}
		unsigned int   TargetIndex;
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

	class RenderState
	{
	public:
		virtual void* GetLowInterface(){ return 0; }
	};

	class RasterizeState : public RenderState
	{
	public:

		virtual bool Create(RasterizeDesc *pDesc) = 0;
	};

	class DepthStencilState : public RenderState
	{
	public:
		virtual bool Create(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef) = 0;
		
		unsigned int StencilRef;
	};

	class BlendState : public RenderState
	{
    public:
         BlendState()
         {
             BlendFactor[0] = BlendFactor[1] = BlendFactor[2] = BlendFactor[3] = 1.0f;
             SampleMask =  0xffffffff;
         }

         float BlendFactor[4];
         unsigned int SampleMask;
	public:
		virtual bool Create(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false) = 0;
	};

	class SamplerState : public RenderState
	{
	public:
		virtual bool Create(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16) = 0;

		static std::string GenerateMapKey(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy);

	protected:
		SamplerFilter _filter;
		TextureAddressMode _AddressU;
		TextureAddressMode _AddressV;
		TextureAddressMode _AddressW;
		UINT          _maxAnisotropy;

	};


}



#endif