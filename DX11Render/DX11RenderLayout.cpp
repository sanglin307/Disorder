#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11RenderLayout::GetLowInterface()
	{
		if( D3DInterface != NULL )
			return D3DInterface.get();

		return 0;

	}

	bool DX11RenderLayout::CreateLayout(ShaderObjectPtr const& vertexShader,const std::vector<VertexInputDes> const& vertexInput,TopologyType topologyType)
	{
		 
		for(unsigned int index=0;index<vertexInput.size();index++)
		{
			 _vertexDes.push_back(vertexInput[index]);
		}

		 _topologyType = topologyType;

		D3D11_INPUT_ELEMENT_DESC pElementDes[12];
		std::string strSemantic[12];

		std::vector<VertexInputDes>::iterator iter = _vertexDes.begin();
		int index = 0;
		for(;iter != _vertexDes.end();iter++)
		{
			GetSemanticDes(iter->Semantic,strSemantic[index],pElementDes[index].SemanticIndex);
			pElementDes[index].SemanticName = strSemantic[index].c_str();
			pElementDes[index].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			pElementDes[index].Format = (DXGI_FORMAT)GPixelFormats[ iter->Format ].PlatformFormat;
			pElementDes[index].InputSlot = 0;
			if( iter->InstanceData)
			{
				pElementDes[index].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				pElementDes[index].InstanceDataStepRate = iter->InstanceDataStepRate;
			}
			else
			{
				pElementDes[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				pElementDes[index].InstanceDataStepRate = 0;
			}
			index++;
		}

		ID3DBlob* shaderData = (ID3DBlob*)(vertexShader->GetDataInterface());
		BOOST_ASSERT(shaderData != NULL );

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
 
		// Create the input layout
		ID3D11InputLayout* pInputLayout = NULL;
		HRESULT hr = renderEngine->D3DDevice()->CreateInputLayout( pElementDes, _vertexDes.size(), shaderData->GetBufferPointer(),
											  shaderData->GetBufferSize(), &pInputLayout );
		BOOST_ASSERT(hr == S_OK);
		
		if( hr != S_OK )
			return false;

		D3DInterface = MakeComPtr<ID3D11InputLayout>(pInputLayout);

		return true;
	}

	void DX11RenderLayout::GetSemanticDes(VertexInputSematic vertexDes,std::string& semanticDes,unsigned int& semanticIndex)
	{
		if( vertexDes == VIS_Position )
		{
			semanticDes = "POSITION";
			semanticIndex = 0;
		}
		else if( vertexDes == VIS_Color )
		{
			semanticDes = "COLOR";
			semanticIndex = 0;
		}
		else if( vertexDes == VIS_Normal )
		{
			semanticDes = "NORMAL";
			semanticIndex = 0;
		}
		else if( vertexDes == VIS_TexCoord0 )
		{
			semanticDes = "TEXCOORD";
			semanticIndex = 0;
		}
		else
		{
			BOOST_ASSERT(0);
		}
	}

}