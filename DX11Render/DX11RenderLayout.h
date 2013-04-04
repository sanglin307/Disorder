#ifndef _DISORDER_DX11RENDERLAYOUT_H_
#define _DISORDER_DX11RENDERLAYOUT_H_

namespace Disorder
{
	class DX11RenderLayout : public RenderLayout
	{
	public:
		virtual bool CreateLayout(ShaderObjectPtr const& vertexShader,const std::vector<VertexInputDes> const& vertexInput,TopologyType topologyType);

	    void GetSemanticDes(VertexInputSematic vertexDes,std::string& semanticDes,unsigned int& semanticIndex);

		virtual void BindVertexBuffer(RenderBufferPtr const& renderBuffer)
		{
			RenderLayout::BindVertexBuffer(renderBuffer);
			VertexArray.push_back((ID3D11Buffer*)(renderBuffer->GetLowInterface()));
			VertexArrayElementSize.push_back(renderBuffer->GetElementSize());
			VertexArrayOffset.push_back(0);
		}

		virtual void * GetLowInterface();

	public:
		ID3D11InputLayoutPtr D3DInterface;
		std::vector<ID3D11Buffer*> VertexArray;
		std::vector<UINT> VertexArrayElementSize;
		std::vector<UINT> VertexArrayOffset;


	};
}

#endif