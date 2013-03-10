#ifndef _DISORDER_DX11RENDERSTATE_H_
#define _DISORDER_DX11RENDERSTATE_H_


namespace Disorder
{
	class DX11SamplerState : public SamplerState
	{
	public:
		virtual bool Create(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy = 16);
		virtual void* GetLowInterface();

	protected:
		ID3D11SamplerStatePtr D3DInterface;
	};

	class DX11RasterizeState : public RasterizeState
	{
	public:

		virtual bool Create(RasterizeDesc *pDesc);
		virtual void* GetLowInterface();

	protected:
		ID3D11RasterizerStatePtr D3DInterface;

	};

	class DX11BlendState : public BlendState
	{
	public:
		virtual bool Create(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);
		virtual void* GetLowInterface();

	private:

		inline D3D11_BLEND_OP GetD3DBlendOp(BlendOperation blendOp)
		{
			switch( blendOp )
			{
				case BLEND_OP_ADD:
					return D3D11_BLEND_OP_ADD;
				case BLEND_OP_SUBTRACT:
					return D3D11_BLEND_OP_SUBTRACT;
				case BLEND_OP_REV_SUBTRACT:
					return D3D11_BLEND_OP_REV_SUBTRACT;
				case BLEND_OP_MIN:
					return D3D11_BLEND_OP_MIN;
				case BLEND_OP_MAX:
					return D3D11_BLEND_OP_MAX;
				default:
					BOOST_ASSERT(0);
			}

			return D3D11_BLEND_OP_ADD;
		}

		inline D3D11_BLEND GetD3DBlendDesc(BlendOptions blendOptions)
		{
			switch(blendOptions)
			{
				case BLEND_ZERO:
					return D3D11_BLEND_ZERO;
				case BLEND_ONE:
					return D3D11_BLEND_ONE;
				case BLEND_SRC_COLOR:
					return D3D11_BLEND_SRC_COLOR;
				case BLEND_INV_SRC_COLOR:
					return D3D11_BLEND_INV_SRC_COLOR;
				case BLEND_SRC_ALPHA:
					return D3D11_BLEND_SRC_ALPHA;
				case BLEND_INV_SRC_ALPHA:
					return D3D11_BLEND_INV_SRC_ALPHA;
				case BLEND_DEST_ALPHA:
					return D3D11_BLEND_DEST_ALPHA;
				case BLEND_INV_DEST_ALPHA:
					return D3D11_BLEND_INV_DEST_ALPHA;
				case BLEND_DEST_COLOR:
					return D3D11_BLEND_DEST_COLOR;
				case BLEND_INV_DEST_COLOR:
					return D3D11_BLEND_INV_DEST_COLOR;
				case BLEND_SRC_ALPHA_SAT:
					return D3D11_BLEND_SRC_ALPHA_SAT;
				case BLEND_BLEND_FACTOR:
					return D3D11_BLEND_BLEND_FACTOR;
				case BLEND_INV_BLEND_FACTOR:
					return D3D11_BLEND_INV_BLEND_FACTOR;
				case BLEND_SRC1_COLOR:
					return D3D11_BLEND_SRC1_COLOR;
				case BLEND_INV_SRC1_COLOR:
					return D3D11_BLEND_INV_SRC1_COLOR;
				case BLEND_SRC1_ALPHA:
					return D3D11_BLEND_SRC1_ALPHA;
				case BLEND_INV_SRC1_ALPHA:
					return D3D11_BLEND_INV_SRC1_ALPHA;
				default:
					BOOST_ASSERT(0);
			}

			return D3D11_BLEND_ZERO;
		}

	protected:
		ID3D11BlendStatePtr D3DInterface;
	};
}

#endif