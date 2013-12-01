#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11RenderLayout::GetLowInterface()
	{
		if( D3DInterface != NULL )
			return D3DInterface.get();

		return 0;

	}

	DX11RenderLayoutPtr DX11RenderLayout::Create(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer)
	{
		 
		DX11ShaderReflectionPtr shaderReflection = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader)->ShaderReflect;
		BOOST_ASSERT(shaderReflection != NULL );
 
		DX11RenderLayout *pLayout = new DX11RenderLayout;

		pLayout->_topologyType = topologyType;

		std::vector<D3D11_INPUT_ELEMENT_DESC> vElementDes;

		int inputSlot = 0;

		for(unsigned int index=0;index<shaderReflection->InputSignatureParameters.size();index++)
		{
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.SemanticName = shaderReflection->InputSignatureParameters[index].SemanticName.c_str();
			desc.SemanticIndex = shaderReflection->InputSignatureParameters[index].SemanticIndex;
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.Format =  pLayout->GetInputFormat(shaderReflection->InputSignatureParameters[index].ComponentType,shaderReflection->InputSignatureParameters[index].Mask);
			if( soloBuffer )
			    desc.InputSlot = 0;
			else
			{
				desc.InputSlot = inputSlot;
				inputSlot++;
			}

			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			vElementDes.push_back(desc);
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
		{
			delete pLayout;
			return NULL;
		}


		pLayout->D3DInterface = MakeComPtr<ID3D11InputLayout>(pInputLayout);

		return DX11RenderLayoutPtr(pLayout);
	}

	DXGI_FORMAT DX11RenderLayout::GetInputFormat(D3D_REGISTER_COMPONENT_TYPE component,BYTE mask)
	{
		 if( component == D3D_REGISTER_COMPONENT_UINT32 )
		 {
			 switch(mask)
			 {
			 case 1:
				 return DXGI_FORMAT_R32_UINT;
			 case 3:
				 return DXGI_FORMAT_R32G32_UINT;
			 case 7:
				 return DXGI_FORMAT_R32G32B32_UINT;
			 case 15:
				 return DXGI_FORMAT_R32G32B32A32_UINT;
			 }
		 }
		 else if( component == D3D_REGISTER_COMPONENT_SINT32 )
		 {
			 switch(mask)
			 {
			 case 1:
				 return DXGI_FORMAT_R32_SINT;
			 case 3:
				 return DXGI_FORMAT_R32G32_SINT;
			 case 7:
				 return DXGI_FORMAT_R32G32B32_SINT;
			 case 15:
				 return DXGI_FORMAT_R32G32B32A32_SINT;
			 }
		 }
		 else if( component == D3D_REGISTER_COMPONENT_FLOAT32 )
		 {
			 switch(mask)
			 {
			 case 1:
				 return DXGI_FORMAT_R32_FLOAT;
			 case 3:
				 return DXGI_FORMAT_R32G32_FLOAT;
			 case 7:
				 return DXGI_FORMAT_R32G32B32_FLOAT;
			 case 15:
				 return DXGI_FORMAT_R32G32B32A32_FLOAT;
			 }
		 }

		 BOOST_ASSERT(0);

		 return DXGI_FORMAT_UNKNOWN;
	}

}