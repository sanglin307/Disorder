#include "DX11RenderInclude.h"
#include "../Engine/Logger.h"

namespace Disorder
{
 
	//=============================Shader object=========================================
 
	void DX11ShaderObject::UpdateShaderParameter()
	{
		CachedShaderResourceView.clear();
		CachedConstBuffer.clear();
		CachedSamplerState.clear();

		if(ShaderReflect->ConstantBuffers.size())
		{
			RenderEnginePtr renderEngine = GEngine->RenderEngine;
			 
			for(unsigned int i=0;i<ShaderReflect->ConstantBuffers.size();i++)
			{
				ShaderPropertyPtr cbuff = ShaderReflect->ConstantBuffers[i].BufferParamRef;
				if( cbuff == NULL || cbuff->GetDataAsConstBuffer() == NULL)
				{
					std::stringstream stream;
					stream << "vertex shader's constant buffer is null "<< GetShaderName() <<"," << ShaderReflect->ConstantBuffers[i].CBName;
					GLogger->Error(stream.str());
					continue;
				}
				CachedConstBuffer.push_back((ID3D11Buffer*)(cbuff->GetDataAsConstBuffer()->GetLowInterface()));

				BYTE* pData = new BYTE[ShaderReflect->ConstantBuffers[i].CBSize];
				BYTE* pDest = pData;
				for(unsigned int j=0;j<ShaderReflect->ConstantBuffers[i].Parameters.size();j++)
				{
					ShaderVariableDesc vaDesc = ShaderReflect->ConstantBuffers[i].Variables[j];
					ShaderTypeDesc taDesc = ShaderReflect->ConstantBuffers[i].Types[j];
					pDest = pData + ShaderReflect->ConstantBuffers[i].Variables[j].StartOffset;
				
					if( ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_SCALAR && ShaderReflect->ConstantBuffers[i].Types[j].Type == D3D_SVT_INT)
					{
						void *pSrc = ShaderReflect->ConstantBuffers[i].Parameters[j]->GetData();					
						memcpy(pDest,pSrc,sizeof(int));
					}
					else if(ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_SCALAR && ShaderReflect->ConstantBuffers[i].Types[j].Type == D3D_SVT_FLOAT)
					{
						void *pSrc = ShaderReflect->ConstantBuffers[i].Parameters[j]->GetData();
						memcpy(pDest,pSrc,sizeof(float));
					}

					if(ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_VECTOR)
					{
						BYTE *pSrc = (BYTE*)(ShaderReflect->ConstantBuffers[i].Parameters[j]->GetData());
						if( ShaderReflect->ConstantBuffers[i].Types[j].Columns == 3 ) // 128bit pack
						{
							//for( int n = 0;n<elementNumber;n++)
							//{
								memcpy(pDest,pSrc,sizeof(Vector3));
								//pDest += sizeof(Vector4);
								//pSrc += sizeof(Vector3);
							//}
						}
						else if( ShaderReflect->ConstantBuffers[i].Types[j].Columns == 4 )
						{
							memcpy(pDest,pSrc,sizeof(Vector4));
						}
					}
					if(ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_MATRIX_ROWS ||
					   ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_MATRIX_COLUMNS)
					{
						void *pSrc = ShaderReflect->ConstantBuffers[i].Parameters[j]->GetData();
						memcpy(pDest,pSrc,sizeof(Matrix4));
					}
				}
				
				renderEngine->UpdateSubresource(ShaderReflect->ConstantBuffers[i].BufferParamRef->GetDataAsConstBuffer(),pData,ShaderReflect->ConstantBuffers[i].CBSize);
				delete pData;

			}
		}

		for(unsigned  int i=0;i<ShaderReflect->ResourceBindings.size();++i)
		{
			if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_TEXTURE )
			{
				ShaderPropertyPtr res = ShaderReflect->ResourceBindings[i].ParamRef;
				if( res == NULL || res->GetDataAsShaderResource() == NULL)
				{
					std::stringstream stream;
					stream << "vertex shader's shader resource is null "<< GetShaderName() <<"," << ShaderReflect->ResourceBindings[i].Name;
					GLogger->Error(stream.str());
					continue;
				}
				CachedShaderResourceView.push_back((ID3D11ShaderResourceView*)(res->GetDataAsShaderResource()->GetLowInterface(RSU_ShaderResource)));
			}

			if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_SAMPLER )
			{
				ShaderPropertyPtr res = ShaderReflect->ResourceBindings[i].ParamRef;
				if( res == NULL || res->GetDataAsSampler() == NULL)
				{
					std::stringstream stream;
					stream << "vertex shader's sampler state is null "<< GetShaderName() <<"," << ShaderReflect->ResourceBindings[i].Name;
					GLogger->Error(stream.str());
					continue;
				}
				CachedSamplerState.push_back((ID3D11SamplerState*)(res->GetDataAsSampler()->GetLowInterface()));
			}
		}
	}

	DX11ShaderObjectPtr DX11ShaderObject::Create(std::string const& effectName,std::string const& entryPoint,ShaderType shaderType,ShaderModel shaderModel)
	{
		DX11ShaderObject *pObject = new DX11ShaderObject(effectName,entryPoint,shaderType,shaderModel);
		return DX11ShaderObjectPtr(pObject);
	}

	void* DX11ShaderObject::GetLowInterface()
	{
		switch(_type)
		{
		case ST_VertexShader:
			return VertexShaderInterface.get();
		case ST_PixelShader:
			return PixelShaderInterface.get();
		}
	 
		BOOST_ASSERT(0);

		return 0;
	}

	void* DX11ShaderObject::GetDataInterface()
	{
		if ( DataInterface != NULL )
			return DataInterface.get();

		return 0;
	}
	 
	DX11ShaderReflectionPtr DX11ShaderReflection::Create()
	{
		DX11ShaderReflection *pReflection = new DX11ShaderReflection();
		return DX11ShaderReflectionPtr(pReflection);
	}
 
	HRESULT DX11ShaderObject::CompileShaderFromFile(std::string const& fileName, std::string const& entryPoint, std::string const& shaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		FileObjectPtr fileptr = GEngine->FileManager->OpenFile(fileName,"rt");
		std::string shaderContent = GEngine->FileManager->ReadFile(fileptr);

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;;
	#endif

	 
		ID3DBlob* pErrorBlob;
		DX11ShaderInclude ShaderInclude;
		hr = D3DCompile(shaderContent.c_str(),shaderContent.size(),"",NULL,&ShaderInclude,entryPoint.c_str(),shaderModel.c_str(),dwShaderFlags, 0,ppBlobOut, &pErrorBlob);
	 
		if( FAILED(hr) )
		{
			if( pErrorBlob != NULL )
				OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			return hr;
		}
		if( pErrorBlob ) 
			pErrorBlob->Release();

		return S_OK;
	}

	


	bool DX11ShaderObject::LoadShaderFromFile(std::string const& fileName,std::string const& entryPoint,ShaderType type)
	{
		HRESULT hr = S_OK;
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile(fileName, entryPoint, GetShaderModelDes(type), &pVSBlob );
		if( FAILED( hr ) )
		{
			MessageBox( NULL,
						L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return ShaderObjectPtr();
		}

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		BOOST_ASSERT(renderEngine);

		// Create the vertex shader
		if( type == ST_VertexShader )
		{
			ID3D11VertexShader* _pVertexShader = NULL;
			hr = renderEngine->D3DDevice()->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_pVertexShader );
			if( FAILED( hr ) )
			{	
				pVSBlob->Release();
				MessageBox( NULL,
							L"CreateVertexShader Failed", L"Error", MB_OK );
				return false;
			}

			VertexShaderInterface =  MakeComPtr<ID3D11VertexShader>(_pVertexShader);
			DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);

			ShaderReflection();
			return true;
		}
		else if( type == ST_PixelShader )
		{
			ID3D11PixelShader* _pPixelShader = NULL;
			hr = renderEngine->D3DDevice()->CreatePixelShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_pPixelShader );
			if( FAILED( hr ) )
			{	
				pVSBlob->Release();
				MessageBox( NULL,
							L"CreatePixelShader Failed", L"Error", MB_OK );
				return false;
			}

			 
			PixelShaderInterface =  MakeComPtr<ID3D11PixelShader>(_pPixelShader);
			DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);
			ShaderReflection();
			return true;
		}
		
		BOOST_ASSERT(0);

		return false;
	
	}

 
	void DX11ShaderObject::ShaderReflection()
	{
		DX11ShaderReflectionPtr pReflection = DX11ShaderReflection::Create();

		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
	 
		ID3D11ShaderReflection* pReflector = NULL;
		BOOST_ASSERT(DataInterface != NULL);

		HRESULT hr = D3DReflect(DataInterface->GetBufferPointer(), DataInterface->GetBufferSize(),
			IID_ID3D11ShaderReflection, (void**) &pReflector);

		if ( FAILED( hr ) )
		{
			MessageBox( NULL,
							L"Failed to shader reflection ", L"Error", MB_OK );
			return;
		}

		D3D11_SHADER_DESC desc;
		pReflector->GetDesc( &desc );
		pReflection->ShaderDescription = desc;

		// input and output signature description arrays.
		for ( UINT i = 0; i < desc.InputParameters; i++ )
		{
			D3D11_SIGNATURE_PARAMETER_DESC input_desc;
			pReflector->GetInputParameterDesc( i, &input_desc );
			pReflection->InputSignatureParameters.push_back( ShaderSignatureDesc(input_desc) );
		}
		for ( UINT i = 0; i < desc.OutputParameters; i++ )
		{
			D3D11_SIGNATURE_PARAMETER_DESC output_desc;
			pReflector->GetOutputParameterDesc( i, &output_desc );
			pReflection->OutputSignatureParameters.push_back( ShaderSignatureDesc(output_desc) );
		}
 
	    //constant buffer information 
		for ( UINT i = 0; i < desc.ConstantBuffers; i++ )
		{
			ID3D11ShaderReflectionConstantBuffer* pConstBuffer = pReflector->GetConstantBufferByIndex( i );
		
			D3D11_SHADER_BUFFER_DESC bufferDesc;
			pConstBuffer->GetDesc( &bufferDesc );
		
			if ( bufferDesc.Type == D3D_CT_CBUFFER || bufferDesc.Type == D3D_CT_TBUFFER )
			{
				ConstantBufferDesc constantBuffer;
				constantBuffer.CBName = bufferDesc.Name;
				constantBuffer.CBSize = bufferDesc.Size;
				constantBuffer.CBType = bufferDesc.Type;
				constantBuffer.CBuFlags = bufferDesc.uFlags;
				constantBuffer.CBVariables = bufferDesc.Variables;
                ShaderPropertyManagerPtr propertyManager = resourceManager->GetPropertyManager(constantBuffer.CBName);
				BOOST_ASSERT(propertyManager->Name != ShaderPropertyManager::sManagerGlobal);
				constantBuffer.BufferParamRef = propertyManager->CreateProperty(constantBuffer.CBName,eSP_ConstBuffer);
			 
				// Load the description of each variable for use later on when binding a buffer
				for ( UINT j = 0; j < bufferDesc.Variables; j++ )
				{
					// Get the variable description and store it
					ID3D11ShaderReflectionVariable* pVariable = pConstBuffer->GetVariableByIndex( j );
					D3D11_SHADER_VARIABLE_DESC var_desc;
					pVariable->GetDesc( &var_desc );

					constantBuffer.Variables.push_back( ShaderVariableDesc(var_desc) );

					// Get the variable type description and store it
					ID3D11ShaderReflectionType* pType = pVariable->GetType();
					D3D11_SHADER_TYPE_DESC type_desc;
					pType->GetDesc( &type_desc );

					constantBuffer.Types.push_back( ShaderTypeDesc(type_desc) );

					// Get references to the parameters for binding to these variables.
					ShaderPropertyPtr pParam;
					std::string varName = var_desc.Name;
					EShaderProperty shaderProperty;
					if( type_desc.Class == D3D_SVC_SCALAR && type_desc.Type == D3D_SVT_INT)
					{
					    shaderProperty = eSP_Int;
					}
					else if(type_desc.Class == D3D_SVC_SCALAR && type_desc.Type == D3D_SVT_FLOAT)
					{
					    shaderProperty = eSP_Float;
					}
					else if ( type_desc.Class == D3D_SVC_VECTOR )
					{		
						if( type_desc.Columns == 3)
						   shaderProperty = eSP_Vector3;
						else if( type_desc.Columns == 4)
						   shaderProperty = eSP_Vector4;
						else
						{
							BOOST_ASSERT(0);
						}
 
					}
					else if ( ( type_desc.Class == D3D_SVC_MATRIX_ROWS ) ||
								( type_desc.Class == D3D_SVC_MATRIX_COLUMNS ) )
					{
						 
						if ( type_desc.Columns == 4 && type_desc.Rows == 4  ) 
						{
							shaderProperty = eSP_Matrix4;
						}
						else if( type_desc.Columns == 3 && type_desc.Rows == 3 )
						{
							shaderProperty = eSP_Matrix3;
						}
						else
						{
							BOOST_ASSERT(0);
						}
					}


					// we don't support array now
					BOOST_ASSERT(type_desc.Elements == 0);
					pParam = propertyManager->CreateProperty(varName,shaderProperty);

					constantBuffer.Parameters.push_back( pParam );
					 
				}
				
				RenderBufferPtr constBuffer = resourceManager->CreateRenderBuffer(RBT_Constant,BAH_GPU_Read,bufferDesc.Size,bufferDesc.Size,NULL);
				constantBuffer.BufferParamRef->SetData(constBuffer);
				 

				pReflection->ConstantBuffers.push_back( constantBuffer );
			}
		}


		 ShaderPropertyManagerPtr GlobalPropertyManager = resourceManager->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		// Get the overall resource binding information for this shader.
		for ( UINT i = 0; i < desc.BoundResources; i++ )
		{
			D3D11_SHADER_INPUT_BIND_DESC resource_desc;
			pReflector->GetResourceBindingDesc( i, &resource_desc );
			ShaderInputBindDesc binddesc( resource_desc );

			std::string rname = binddesc.Name;
			if ( resource_desc.Type == D3D_SIT_CBUFFER || resource_desc.Type == D3D_SIT_TBUFFER )
			{
				binddesc.ParamRef = GlobalPropertyManager->CreateProperty(rname,eSP_ConstBuffer);
			}
			else if ( resource_desc.Type == D3D_SIT_TEXTURE || resource_desc.Type == D3D_SIT_STRUCTURED )
			{
				binddesc.ParamRef = GlobalPropertyManager->CreateProperty(rname,eSP_ShaderResource);
			}
			else if ( resource_desc.Type == D3D_SIT_SAMPLER )
			{
				binddesc.ParamRef = GlobalPropertyManager->CreateProperty(rname,eSP_SampleState);
			}
		/*	else if ( resource_desc.Type == D3D_SIT_UAV_RWTYPED || resource_desc.Type == D3D_SIT_UAV_RWSTRUCTURED
				|| resource_desc.Type == D3D_SIT_BYTEADDRESS || resource_desc.Type == D3D_SIT_UAV_RWBYTEADDRESS
				|| resource_desc.Type == D3D_SIT_UAV_APPEND_STRUCTURED || resource_desc.Type == D3D_SIT_UAV_CONSUME_STRUCTURED
				|| resource_desc.Type == D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER )
			{
				binddesc.ParamRef = GetUnorderedAccessParameter(rname);
			}*/


			pReflection->ResourceBindings.push_back( binddesc );
		}


		// Release the shader reflection interface
		pReflector->Release();

		ShaderReflect = pReflection;
		return;
	}
 
}