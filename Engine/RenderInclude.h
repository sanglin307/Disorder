#ifndef _DISORDER_RENDERINCLUDE_H_
#define _DISORDER_RENDERINCLUDE_H_

// used to include all enum and define here.

namespace Disorder
{
	const int MaxMaterialPassNumber = 5;
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
		RPT_ForwardMultiPassLight,
		RPT_MaxRenderPath
	};

	enum ForwardRenderPass
	{
		FRP_BaseLight,
		FRP_DynamicLight,
		FRP_Max
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
		PF_Unknown              =0,
		PF_R32G32B32A32F        =1,
		PF_R32G32B32F           =2,
		PF_R8G8B8A8             =3,  // unsigned int resolve to float evenly [0,1]
		PF_R32G32F              =4,
		PF_R8G8                 =5,  // unsigned int resolve to float evenly [0,1]
		PF_R10G10B10A2          =6,
		PF_D24S8                =7,
		PF_MAX                  =8
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