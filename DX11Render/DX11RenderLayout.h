#ifndef _DISORDER_DX11RENDERLAYOUT_H_
#define _DISORDER_DX11RENDERLAYOUT_H_

namespace Disorder
{
	class DX11RenderLayout : public RenderLayout
	{
	public:
		static std::string POSITION;
		static std::string COLOR;
		static std::string NORMAL;
		static std::string TEXCOORD;

		DX11RenderLayout(RenderEffect* renderEffect,TopologyType topologyType,bool soloBuffer);

	    DXGI_FORMAT GetInputFormat(D3D_REGISTER_COMPONENT_TYPE component,BYTE mask);

		virtual void BindVertexBuffer(RenderBuffer* renderBuffer)
		{
			RenderLayout::BindVertexBuffer(renderBuffer);
			VertexArray.push_back((ID3D11Buffer*)(renderBuffer->GetHandle()));
			VertexArrayElementSize.push_back(renderBuffer->GetElementSize());
			VertexArrayOffset.push_back(0);
		}

		virtual void * GetHandle();

	public:
		ID3D11InputLayout* D3DInterface;
		std::vector<ID3D11Buffer*> VertexArray;
		std::vector<UINT> VertexArrayElementSize;
		std::vector<UINT> VertexArrayOffset;
	};
}

#endif