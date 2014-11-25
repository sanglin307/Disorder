#include "EngineInclude.h"

namespace Disorder
{
	RenderEngine* GRenderEngine = NULL;

	void RenderEngine::ComputePixelColorAlphaSize(PixelFormat format, int& colorSize, int& alphaSize)
	{
		colorSize = 0;
		alphaSize = 0;

		switch (format)
		{
		case PF_R1_UNORM:
			colorSize = 1;
			break;
		case PF_A8_UNORM:
			alphaSize = 8;
			break;
		case PF_R8_SINT:
		case PF_R8_SNORM:
		case PF_R8_TYPELESS:
		case PF_R8_UINT:
		case PF_R8_UNORM:
			colorSize = 8;
			break;
		case PF_B5G5R5A1_UNORM:
			colorSize = 15;
			alphaSize = 1;
			break;
		case PF_B5G6R5_UNORM:
			colorSize = 16;
			break;
		case PF_R16_FLOAT:
		case PF_R16_SINT:
		case PF_R16_SNORM:
		case PF_R16_TYPELESS:
		case PF_R16_UINT:
		case PF_R16_UNORM:
		case PF_R8G8_SINT:
		case PF_R8G8_SNORM:
		case PF_R8G8_TYPELESS:
		case PF_R8G8_UINT:
		case PF_R8G8_UNORM:
			colorSize = 16;
			break;
		case PF_B8G8R8X8_TYPELESS:
		case PF_B8G8R8X8_UNORM:
		case PF_B8G8R8X8_UNORM_SRGB:
		case PF_R8G8B8A8_SINT:
		case PF_R8G8B8A8_SNORM:
		case PF_R8G8B8A8_TYPELESS:
		case PF_R8G8B8A8_UINT:
		case PF_R8G8B8A8_UNORM:
		case PF_R8G8B8A8_UNORM_SRGB:
		case PF_B8G8R8A8_TYPELESS:
		case PF_B8G8R8A8_UNORM:
		case PF_B8G8R8A8_UNORM_SRGB:
			colorSize = 24;
			alphaSize = 8;
			break;
		case PF_G8R8_G8B8_UNORM:
			colorSize = 16;
			break;
		case PF_R10G10B10_XR_BIAS_A2_UNORM:
		case PF_R10G10B10A2_TYPELESS:
		case PF_R10G10B10A2_UINT:
		case PF_R10G10B10A2_UNORM:
			colorSize = 30;
			alphaSize = 2;
			break;
		case PF_R11G11B10_FLOAT:
		case PF_R16G16_FLOAT:
		case PF_R16G16_SINT:
		case PF_R16G16_SNORM:
		case PF_R16G16_TYPELESS:
		case PF_R16G16_UINT:
		case PF_R16G16_UNORM:
		case PF_R32_FLOAT:
		case PF_R32_FLOAT_X8X24_TYPELESS:
		case PF_R32_SINT:
		case PF_R32_TYPELESS:
		case PF_R32_UINT:
		case PF_R24G8_TYPELESS:
			colorSize = 32;
			break;
 

		case PF_R16G16B16A16_FLOAT:
		case PF_R16G16B16A16_SINT:
		case PF_R16G16B16A16_SNORM:
		case PF_R16G16B16A16_TYPELESS:
		case PF_R16G16B16A16_UINT:
		case PF_R16G16B16A16_UNORM:
			colorSize = 48;
			alphaSize = 16;
			break;

		case PF_R32G32_FLOAT:
		case PF_R32G32_SINT:
		case PF_R32G32_TYPELESS:
		case PF_R32G32_UINT:
			colorSize = 64;
			break;

		case PF_R32G32B32_FLOAT:
		case PF_R32G32B32_SINT:
		case PF_R32G32B32_TYPELESS:
		case PF_R32G32B32_UINT:
			colorSize = 96;
			break;
		case PF_R32G32B32A32_FLOAT:
		case PF_R32G32B32A32_SINT:
		case PF_R32G32B32A32_TYPELESS:
		case PF_R32G32B32A32_UINT:
			colorSize = 96;
			alphaSize = 32;
			break;
		}
	}

	void RenderEngine::ComputeDepthStencilSize(PixelFormat format, int& depth, int& stencil)
	{
		depth = 0;
		stencil = 0;

		switch (format)
		{
		case PF_D16_UNORM:
			depth = 16;
			break;
		case PF_D24_UNORM_S8_UINT:
			depth = 24;
			stencil = 8;
			break;
		case PF_D32_FLOAT:
			depth = 32;
			break;
		case PF_D32_FLOAT_S8X24_UINT:
			depth = 32;
			stencil = 8;
			break;
		}
	}

	size_t RenderEngine::ComputePixelChannel(PixelFormat format)
	{
		switch (format)
		{
		case PF_R32G32B32A32_TYPELESS:
		case PF_R32G32B32A32_FLOAT:
		case PF_R32G32B32A32_UINT:
		case PF_R32G32B32A32_SINT:
		case PF_R16G16B16A16_TYPELESS:
		case PF_R16G16B16A16_FLOAT:
		case PF_R16G16B16A16_UNORM:
		case PF_R16G16B16A16_UINT:
		case PF_R16G16B16A16_SNORM:
		case PF_R16G16B16A16_SINT:
		case PF_R10G10B10A2_TYPELESS:
		case PF_R10G10B10A2_UNORM:
		case PF_R10G10B10A2_UINT:
		case PF_R8G8B8A8_TYPELESS:
		case PF_R8G8B8A8_UNORM:
		case PF_R8G8B8A8_UNORM_SRGB:
		case PF_R8G8B8A8_UINT:
		case PF_R8G8B8A8_SNORM:
		case PF_R8G8B8A8_SINT:
		case PF_R9G9B9E5_SHAREDEXP:
		case PF_R8G8_B8G8_UNORM:
		case PF_G8R8_G8B8_UNORM:
		case PF_BC1_TYPELESS:
		case PF_BC1_UNORM:
		case PF_BC1_UNORM_SRGB:
		case PF_BC2_TYPELESS:
		case PF_BC2_UNORM:
		case PF_BC2_UNORM_SRGB:
		case PF_BC3_TYPELESS:
		case PF_BC3_UNORM:
		case PF_BC3_UNORM_SRGB:
		case PF_B5G5R5A1_UNORM:
		case PF_B8G8R8A8_UNORM:
		case PF_B8G8R8X8_UNORM:
		case PF_R10G10B10_XR_BIAS_A2_UNORM:
		case PF_B8G8R8A8_TYPELESS:
		case PF_B8G8R8A8_UNORM_SRGB:
		case PF_B8G8R8X8_TYPELESS:
		case PF_B8G8R8X8_UNORM_SRGB:
		case PF_BC7_TYPELESS:
		case PF_BC7_UNORM:
		case PF_BC7_UNORM_SRGB:
			return 4;

		case PF_R32G32B32_TYPELESS:
		case PF_R32G32B32_FLOAT:
		case PF_R32G32B32_UINT:
		case PF_R32G32B32_SINT:
		case PF_R32G8X24_TYPELESS:
		case PF_D32_FLOAT_S8X24_UINT:
		case PF_R32_FLOAT_X8X24_TYPELESS:
		case PF_X32_TYPELESS_G8X24_UINT:
		case PF_R11G11B10_FLOAT:
		case PF_B5G6R5_UNORM:
		case PF_BC6H_TYPELESS:
		case PF_BC6H_UF16:
		case PF_BC6H_SF16:
			return 3;
	
		case PF_R32G32_TYPELESS:
		case PF_R32G32_FLOAT:
		case PF_R32G32_UINT:
		case PF_R32G32_SINT:
		case PF_R16G16_TYPELESS:
		case PF_R16G16_FLOAT:
		case PF_R16G16_UNORM:
		case PF_R16G16_UINT:
		case PF_R16G16_SNORM:
		case PF_R16G16_SINT:
		case PF_R24G8_TYPELESS:
		case PF_D24_UNORM_S8_UINT:
		case PF_R24_UNORM_X8_TYPELESS:
		case PF_X24_TYPELESS_G8_UINT:
		case PF_R8G8_TYPELESS:
		case PF_R8G8_UNORM:
		case PF_R8G8_UINT:
		case PF_R8G8_SNORM:
		case PF_R8G8_SINT:
		case PF_BC5_TYPELESS:
		case PF_BC5_UNORM:
		case PF_BC5_SNORM:
			return 2;
		
	    case PF_R32_TYPELESS:
		case PF_D32_FLOAT:
		case PF_R32_FLOAT:
		case PF_R32_UINT:
		case PF_R32_SINT:
		case PF_R16_TYPELESS:
		case PF_R16_FLOAT:
		case PF_D16_UNORM:
		case PF_R16_UNORM:
		case PF_R16_UINT:
		case PF_R16_SNORM:
		case PF_R16_SINT:
		case PF_R8_TYPELESS:
		case PF_R8_UNORM:
		case PF_R8_UINT:
		case PF_R8_SNORM:
		case PF_R8_SINT:
		case PF_A8_UNORM:
		case PF_R1_UNORM:
		case PF_BC4_TYPELESS:
		case PF_BC4_UNORM:
		case PF_BC4_SNORM:
			return 1;
 
		default:
			BOOST_ASSERT(0);
			return 0;
		}
	}

	size_t RenderEngine::ComputePixelSizeBits(PixelFormat format)
	{
		switch (format)
		{
		  case PF_R1_UNORM:
			return 1;

		  case PF_A8_UNORM:
		  case PF_R8_SINT:
		  case PF_R8_SNORM:
		  case PF_R8_TYPELESS:
		  case PF_R8_UINT:
		  case PF_R8_UNORM:
			return 8;

		  case PF_B5G5R5A1_UNORM:
		  case PF_B5G6R5_UNORM:
		  case PF_D16_UNORM:
		  case PF_R16_FLOAT:
		  case PF_R16_SINT:
		  case PF_R16_SNORM:
		  case PF_R16_TYPELESS:
		  case PF_R16_UINT:
		  case PF_R16_UNORM:
		  case PF_R8G8_SINT:
		  case PF_R8G8_SNORM:
		  case PF_R8G8_TYPELESS:
		  case PF_R8G8_UINT:
		  case PF_R8G8_UNORM:
			return 16;

		  case PF_B8G8R8X8_TYPELESS:
		  case PF_B8G8R8X8_UNORM:
		  case PF_B8G8R8X8_UNORM_SRGB:
		  case PF_D24_UNORM_S8_UINT:
		  case PF_D32_FLOAT:
		  case PF_D32_FLOAT_S8X24_UINT:
		  case PF_G8R8_G8B8_UNORM:
		  case PF_R10G10B10_XR_BIAS_A2_UNORM:
		  case PF_R10G10B10A2_TYPELESS:
		  case PF_R10G10B10A2_UINT:
		  case PF_R10G10B10A2_UNORM:
		  case PF_R11G11B10_FLOAT:
		  case PF_R16G16_FLOAT:
		  case PF_R16G16_SINT:
		  case PF_R16G16_SNORM:
		  case PF_R16G16_TYPELESS:
		  case PF_R16G16_UINT:
		  case PF_R16G16_UNORM:
		  case PF_R24_UNORM_X8_TYPELESS:
		  case PF_R24G8_TYPELESS:
		  case PF_R32_FLOAT:
		  case PF_R32_FLOAT_X8X24_TYPELESS:
		  case PF_R32_SINT:
		  case PF_R32_TYPELESS:
		  case PF_R32_UINT:
		  case PF_R8G8_B8G8_UNORM:
		  case PF_R8G8B8A8_SINT:
		  case PF_R8G8B8A8_SNORM:
		  case PF_R8G8B8A8_TYPELESS:
		  case PF_R8G8B8A8_UINT:
		  case PF_R8G8B8A8_UNORM:
		  case PF_R8G8B8A8_UNORM_SRGB:
		  case PF_B8G8R8A8_TYPELESS:
		  case PF_B8G8R8A8_UNORM:
		  case PF_B8G8R8A8_UNORM_SRGB:
		  case PF_R9G9B9E5_SHAREDEXP:
		  case PF_X24_TYPELESS_G8_UINT:
		  case PF_X32_TYPELESS_G8X24_UINT:
			return 32;

		  case PF_R16G16B16A16_FLOAT:
		  case PF_R16G16B16A16_SINT:
		  case PF_R16G16B16A16_SNORM:
		  case PF_R16G16B16A16_TYPELESS:
		  case PF_R16G16B16A16_UINT:
		  case PF_R16G16B16A16_UNORM:
		  case PF_R32G32_FLOAT:
		  case PF_R32G32_SINT:
		  case PF_R32G32_TYPELESS:
		  case PF_R32G32_UINT:
		  case PF_R32G8X24_TYPELESS:
			return 64;

		  case PF_R32G32B32_FLOAT:
		  case PF_R32G32B32_SINT:
		  case PF_R32G32B32_TYPELESS:
		  case PF_R32G32B32_UINT:
			return 96;

		  case PF_R32G32B32A32_FLOAT:
		  case PF_R32G32B32A32_SINT:
		  case PF_R32G32B32A32_TYPELESS:
		  case PF_R32G32B32A32_UINT:
			return 128;

		  case PF_BC1_TYPELESS:
		  case PF_BC1_UNORM:
		  case PF_BC1_UNORM_SRGB:
		  case PF_BC4_SNORM:
		  case PF_BC4_TYPELESS:
		  case PF_BC4_UNORM:
			return 4;

		  case PF_BC2_TYPELESS:
		  case PF_BC2_UNORM:
		  case PF_BC2_UNORM_SRGB:
		  case PF_BC3_TYPELESS:
		  case PF_BC3_UNORM:
		  case PF_BC3_UNORM_SRGB:
		  case PF_BC5_SNORM:
		  case PF_BC5_TYPELESS:
		  case PF_BC5_UNORM:
		  case PF_BC6H_SF16:
		  case PF_BC6H_TYPELESS:
		  case PF_BC6H_UF16:
		  case PF_BC7_TYPELESS:
		  case PF_BC7_UNORM:
		  case PF_BC7_UNORM_SRGB:
			return 8;

		  default:
			return 0;
		}
	}

	size_t RenderEngine::ComputeBlockSizeBits(PixelFormat format)
	{
		switch (format)
		{
		  case PF_BC1_TYPELESS:
		  case PF_BC1_UNORM:
		  case PF_BC1_UNORM_SRGB:
		  case PF_BC4_SNORM:
		  case PF_BC4_TYPELESS:
		  case PF_BC4_UNORM:
		  case PF_BC2_TYPELESS:
		  case PF_BC2_UNORM:
		  case PF_BC2_UNORM_SRGB:
		  case PF_BC3_TYPELESS:
		  case PF_BC3_UNORM:
		  case PF_BC3_UNORM_SRGB:
		  case PF_BC5_SNORM:
		  case PF_BC5_TYPELESS:
		  case PF_BC5_UNORM:
		  case PF_BC6H_SF16:
		  case PF_BC6H_TYPELESS:
		  case PF_BC6H_UF16:
		  case PF_BC7_TYPELESS:
		  case PF_BC7_UNORM:
		  case PF_BC7_UNORM_SRGB:
			return ComputePixelSizeBits(format) * 16;
		  default:
			 BOOST_ASSERT(0);
			return 0;
		}
	}

	bool RenderEngine::IsCompressed(PixelFormat format)
	{
		switch (format)
		{
		  case PF_BC1_TYPELESS:
		  case PF_BC1_UNORM:
		  case PF_BC1_UNORM_SRGB:
		  case PF_BC4_SNORM:
		  case PF_BC4_TYPELESS:
		  case PF_BC4_UNORM:
		  case PF_BC2_TYPELESS:
		  case PF_BC2_UNORM:
		  case PF_BC2_UNORM_SRGB:
		  case PF_BC3_TYPELESS:
		  case PF_BC3_UNORM:
		  case PF_BC3_UNORM_SRGB:
		  case PF_BC5_SNORM:
		  case PF_BC5_TYPELESS:
		  case PF_BC5_UNORM:
		  case PF_BC6H_SF16:
		  case PF_BC6H_TYPELESS:
		  case PF_BC6H_UF16:
		  case PF_BC7_TYPELESS:
		  case PF_BC7_UNORM:
		  case PF_BC7_UNORM_SRGB:
			return true;
		 
		  default:
			return false;
		}
	}

	unsigned int RenderEngine::GetTextureFormatDimensionAlignment(PixelFormat format)
	{
		switch (format)
		{
		  case PF_BC1_TYPELESS:
		  case PF_BC1_UNORM:
		  case PF_BC1_UNORM_SRGB:
		  case PF_BC4_SNORM:
		  case PF_BC4_TYPELESS:
		  case PF_BC4_UNORM:
		  case PF_BC2_TYPELESS:
		  case PF_BC2_UNORM:
		  case PF_BC2_UNORM_SRGB:
		  case PF_BC3_TYPELESS:
		  case PF_BC3_UNORM:
		  case PF_BC3_UNORM_SRGB:
		  case PF_BC5_SNORM:
		  case PF_BC5_TYPELESS:
		  case PF_BC5_UNORM:
		  case PF_BC6H_SF16:
		  case PF_BC6H_TYPELESS:
		  case PF_BC6H_UF16:
		  case PF_BC7_TYPELESS:
		  case PF_BC7_UNORM:
		  case PF_BC7_UNORM_SRGB:
			return 4;
		  default:
			return 1;
		}
	}

	int RenderEngine::GetTriangleCountFromTopology(TopologyType topology,int indexCount)
	{
		switch (topology)
		{
		case TT_PointList:
			return indexCount / 3;
		case TT_LineList:
			return indexCount / 2;
		case TT_LineStrip:
			return indexCount - 1;
		case TT_TriangleList:
			return indexCount / 3;
		case TT_TriangleStrip:
			return indexCount - 2;
		default:
			BOOST_ASSERT(0);
		}

		return 0;
	}

	bool RenderEngine::IsDepthStencilFormat(PixelFormat format)
	{
		switch (format)
		{
		  case PF_D32_FLOAT_S8X24_UINT:
		  case PF_D32_FLOAT:
		  case PF_D24_UNORM_S8_UINT:
		  case PF_D16_UNORM:
			return true;
		  default:
			return false;
		}
	}

	
}