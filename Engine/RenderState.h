#ifndef _DISORDER_RENDERSTATE_H_
#define _DISORDER_RENDERSTATE_H_


namespace Disorder
{
	enum SamplerFilter
	{
		SF_Point,
		SF_Linear,
		SF_Anisotropic
	};

	enum TextureAddressMode
	{
		TAM_Wrap,
		TAM_Mirror,
		TAM_Clamp,
		TAM_Border
	};

	enum RenderFillMode
	{
		RFM_Wireframe,
		RFM_Solid
	};

	enum RenderCullMode
	{
		RCM_None,
		RCM_CullFront,
		RCM_CullBack
	};

	enum ComparisonFunc
	{
		 CF_Never,
         CF_Less,            
         CF_Equal,           
         CF_Less_Equal,       
         CF_Greater,        
         CF_Not_Equal,     
         CF_Greater_Equal,  
         CF_Always   
	};

	enum BlendOptions
	{
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_INV_SRC_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_INV_SRC_ALPHA,
		BLEND_DEST_ALPHA,
		BLEND_INV_DEST_ALPHA,
		BLEND_DEST_COLOR,
		BLEND_INV_DEST_COLOR,
		BLEND_SRC_ALPHA_SAT,
		BLEND_BLEND_FACTOR,
		BLEND_INV_BLEND_FACTOR,
		BLEND_SRC1_COLOR,
		BLEND_INV_SRC1_COLOR,
		BLEND_SRC1_ALPHA,
		BLEND_INV_SRC1_ALPHA
	};

	enum BlendOperation
	{
		BLEND_OP_ADD,
        BLEND_OP_SUBTRACT,
        BLEND_OP_REV_SUBTRACT,
        BLEND_OP_MIN,
        BLEND_OP_MAX
	};

	enum BlendWriteEnable
	{
	    COLOR_WRITE_ENABLE_RED     = 1,
        COLOR_WRITE_ENABLE_GREEN   = 2,
        COLOR_WRITE_ENABLE_BLUE    = 4,
        COLOR_WRITE_ENABLE_ALPHA   = 8,
        COLOR_WRITE_ENABLE_ALL     = 
        ( COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN |  
          COLOR_WRITE_ENABLE_BLUE | COLOR_WRITE_ENABLE_ALPHA ) 
	};

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