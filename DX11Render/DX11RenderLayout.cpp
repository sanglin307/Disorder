#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11RenderLayout::GetLowInterface()
	{
		if( D3DInterface != NULL )
			return D3DInterface.get();

		return 0;

	}

	bool DX11RenderLayout::CreateLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType)
	{
		 
		DX11ShaderReflectionPtr shaderReflection = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader)->ShaderReflect;
		BOOST_ASSERT(shaderReflection != NULL );
 
		_topologyType = topologyType;

		std::vector<D3D11_INPUT_ELEMENT_DESC> vElementDes;

		for(int index=0;index<shaderReflection->InputSignatureParameters.size();index++)
		{
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.SemanticName = shaderReflection->InputSignatureParameters[index].SemanticName;
			desc.SemanticIndex = shaderReflection->InputSignatureParameters[index].SemanticIndex;
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.Format =  DXGI_FORMAT_R32G32B32_FLOAT;
			desc.InputSlot = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			vElementDes.push_back(desc);
		/*	if( iter->InstanceData)
			{
				pElementDes[index].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				pElementDes[index].InstanceDataStepRate = iter->InstanceDataStepRate;
			}
			else
			{
				pElementDes[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				pElementDes[index].InstanceDataStepRate = 0;
			}*/
		}

		ID3DBlob* shaderData = (ID3DBlob*)(vertexShader->GetDataInterface());
		BOOST_ASSERT(shaderData != NULL );

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
 
		// Create the input layout
		ID3D11InputLayout* pInputLayout = NULL;
		HRESULT hr = renderEngine->D3DDevice()->CreateInputLayout(vElementDes.data(), vElementDes.size(), shaderData->GetBufferPointer(),
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