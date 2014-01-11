#ifndef _DISORDER_RENDERDEFINE_H_
#define _DISORDER_RENDERDEFINE_H_

// used to include all enum and define here.

namespace Disorder
{
	// fbx means...
	enum MaterialType
	{
		MT_Phong,
		MT_Lambert,
		MT_HLSL,
		MT_CGFX,
		MT_MentalRay,
		MT_None
	};

	enum RenderPathType
	{
		RPT_ForwardLighting,
		RPT_DeferredShading,
		RPT_MaxRenderPath
	};
 
	typedef struct _BufferInitData
	{
		void const *Data;
		unsigned int RowPitch;
		unsigned int SlicePitch;
	} BufferInitData;

	enum BufferAccessHint
	{
		BAH_CPU_Read = 1UL << 0,
		BAH_CPU_Write = 1UL << 1,
		BAH_GPU_Read = 1UL << 2,
		BAH_GPU_Write = 1UL << 3,
		BAH_GPU_Unordered = 1UL << 4,
		BAH_GPU_Structured = 1UL << 5,
		BAH_Generate_Mips = 1UL << 6,
		BAH_Immutable = 1UL << 7
	};

	enum BufferAccess
	{
		BA_Read_Only,
		BA_Write_Only,
		BA_Read_Write
	};

	 

    enum RenderBufferType
	{
		RBT_Vertex,
		RBT_Index,
		RBT_Constant
	};

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

	enum StencilOperation
	{
	    STENCIL_OP_KEEP       = 1,
        STENCIL_OP_ZERO       = 2,
        STENCIL_OP_REPLACE    = 3,
        STENCIL_OP_INCR_SAT   = 4,
        STENCIL_OP_DECR_SAT   = 5,
        STENCIL_OP_INVERT     = 6,
        STENCIL_OP_INCR       = 7,
        STENCIL_OP_DECR       = 8 
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


	enum ShaderModel
	{
		SM_4_0
	};

	enum ShaderType
	{
		ST_VertexShader,
		ST_PixelShader,
		ST_GeometryShader,
		ST_ComputeShader,
		ST_HullShader,
		ST_DomainShader,

		ST_NumShaderTypes
	};



	enum PixelFormat
	{
		PF_UNKNOWN	                   = 0,
		PF_R32G32B32A32_TYPELESS       = 1,
		PF_R32G32B32A32_FLOAT          = 2,
		PF_R32G32B32A32_UINT           = 3,
		PF_R32G32B32A32_SINT           = 4,
		PF_R32G32B32_TYPELESS          = 5,
		PF_R32G32B32_FLOAT             = 6,
		PF_R32G32B32_UINT              = 7,
		PF_R32G32B32_SINT              = 8,
		PF_R16G16B16A16_TYPELESS       = 9,
		PF_R16G16B16A16_FLOAT          = 10,
		PF_R16G16B16A16_UNORM          = 11,
		PF_R16G16B16A16_UINT           = 12,
		PF_R16G16B16A16_SNORM          = 13,
		PF_R16G16B16A16_SINT           = 14,
		PF_R32G32_TYPELESS             = 15,
		PF_R32G32_FLOAT                = 16,
		PF_R32G32_UINT                 = 17,
		PF_R32G32_SINT                 = 18,
		PF_R32G8X24_TYPELESS           = 19,
		PF_D32_FLOAT_S8X24_UINT        = 20,
		PF_R32_FLOAT_X8X24_TYPELESS    = 21,
		PF_X32_TYPELESS_G8X24_UINT     = 22,
		PF_R10G10B10A2_TYPELESS        = 23,
		PF_R10G10B10A2_UNORM           = 24,
		PF_R10G10B10A2_UINT            = 25,
		PF_R11G11B10_FLOAT             = 26,
		PF_R8G8B8A8_TYPELESS           = 27,
		PF_R8G8B8A8_UNORM              = 28,
		PF_R8G8B8A8_UNORM_SRGB         = 29,
		PF_R8G8B8A8_UINT               = 30,
		PF_R8G8B8A8_SNORM              = 31,
		PF_R8G8B8A8_SINT               = 32,
		PF_R16G16_TYPELESS             = 33,
		PF_R16G16_FLOAT                = 34,
		PF_R16G16_UNORM                = 35,
		PF_R16G16_UINT                 = 36,
		PF_R16G16_SNORM                = 37,
		PF_R16G16_SINT                 = 38,
		PF_R32_TYPELESS                = 39,
		PF_D32_FLOAT                   = 40,
		PF_R32_FLOAT                   = 41,
		PF_R32_UINT                    = 42,
		PF_R32_SINT                    = 43,
		PF_R24G8_TYPELESS              = 44,
		PF_D24_UNORM_S8_UINT           = 45,
		PF_R24_UNORM_X8_TYPELESS       = 46,
		PF_X24_TYPELESS_G8_UINT        = 47,
		PF_R8G8_TYPELESS               = 48,
		PF_R8G8_UNORM                  = 49,
		PF_R8G8_UINT                   = 50,
		PF_R8G8_SNORM                  = 51,
		PF_R8G8_SINT                   = 52,
		PF_R16_TYPELESS                = 53,
		PF_R16_FLOAT                   = 54,
		PF_D16_UNORM                   = 55,
		PF_R16_UNORM                   = 56,
		PF_R16_UINT                    = 57,
		PF_R16_SNORM                   = 58,
		PF_R16_SINT                    = 59,
		PF_R8_TYPELESS                 = 60,
		PF_R8_UNORM                    = 61,
		PF_R8_UINT                     = 62,
		PF_R8_SNORM                    = 63,
		PF_R8_SINT                     = 64,
		PF_A8_UNORM                    = 65,
		PF_R1_UNORM                    = 66,
		PF_R9G9B9E5_SHAREDEXP          = 67,
		PF_R8G8_B8G8_UNORM             = 68,
		PF_G8R8_G8B8_UNORM             = 69,
		PF_BC1_TYPELESS                = 70,
		PF_BC1_UNORM                   = 71,
		PF_BC1_UNORM_SRGB              = 72,
		PF_BC2_TYPELESS                = 73,
		PF_BC2_UNORM                   = 74,
		PF_BC2_UNORM_SRGB              = 75,
		PF_BC3_TYPELESS                = 76,
		PF_BC3_UNORM                   = 77,
		PF_BC3_UNORM_SRGB              = 78,
		PF_BC4_TYPELESS                = 79,
		PF_BC4_UNORM                   = 80,
		PF_BC4_SNORM                   = 81,
		PF_BC5_TYPELESS                = 82,
		PF_BC5_UNORM                   = 83,
		PF_BC5_SNORM                   = 84,
		PF_B5G6R5_UNORM                = 85,
		PF_B5G5R5A1_UNORM              = 86,
		PF_B8G8R8A8_UNORM              = 87,
		PF_B8G8R8X8_UNORM              = 88,
		PF_R10G10B10_XR_BIAS_A2_UNORM  = 89,
		PF_B8G8R8A8_TYPELESS           = 90,
		PF_B8G8R8A8_UNORM_SRGB         = 91,
		PF_B8G8R8X8_TYPELESS           = 92,
		PF_B8G8R8X8_UNORM_SRGB         = 93,
		PF_BC6H_TYPELESS               = 94,
		PF_BC6H_UF16                   = 95,
		PF_BC6H_SF16                   = 96,
		PF_BC7_TYPELESS                = 97,
		PF_BC7_UNORM                   = 98,
		PF_BC7_UNORM_SRGB              = 99,
		PF_FORCE_UINT                  = 0xffffffff                
	};

	enum TopologyType
	{
		TT_PointList,
		TT_LineList,
		TT_LineStrip,
		TT_TriangleList,
		TT_TriangleStrip,
		TT_LineList_Adj,
		TT_LineStrip_Adj,
		TT_TriangleList_Adj,
		TT_TriangleStrip_Adj,
		TT_1_Ctrl_Pt_PatchList,
		TT_2_Ctrl_Pt_PatchList,
		TT_3_Ctrl_Pt_PatchList,
		TT_4_Ctrl_Pt_PatchList,
		TT_5_Ctrl_Pt_PatchList,
		TT_6_Ctrl_Pt_PatchList,
		TT_7_Ctrl_Pt_PatchList,
		TT_8_Ctrl_Pt_PatchList,
		TT_9_Ctrl_Pt_PatchList,
		TT_10_Ctrl_Pt_PatchList,
		TT_11_Ctrl_Pt_PatchList,
		TT_12_Ctrl_Pt_PatchList,
		TT_13_Ctrl_Pt_PatchList,
		TT_14_Ctrl_Pt_PatchList,
		TT_15_Ctrl_Pt_PatchList,
		TT_16_Ctrl_Pt_PatchList,
		TT_17_Ctrl_Pt_PatchList,
		TT_18_Ctrl_Pt_PatchList,
		TT_19_Ctrl_Pt_PatchList,
		TT_20_Ctrl_Pt_PatchList,
		TT_21_Ctrl_Pt_PatchList,
		TT_22_Ctrl_Pt_PatchList,
		TT_23_Ctrl_Pt_PatchList,
		TT_24_Ctrl_Pt_PatchList,
		TT_25_Ctrl_Pt_PatchList,
		TT_26_Ctrl_Pt_PatchList,
		TT_27_Ctrl_Pt_PatchList,
		TT_28_Ctrl_Pt_PatchList,
		TT_29_Ctrl_Pt_PatchList,
		TT_30_Ctrl_Pt_PatchList,
		TT_31_Ctrl_Pt_PatchList,
		TT_32_Ctrl_Pt_PatchList
	};
 
}

#endif