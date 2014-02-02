#include "DX11RenderInclude.h"
 

namespace Disorder
{
 
	//=============================Shader object=========================================
 
	void DX11ShaderObject::UpdateShaderParameter()
	{
		CachedShaderResourceView.clear();
		CachedConstBuffer.clear();
		CachedSamplerState.clear();
 
		for(unsigned  int i=0;i<ShaderReflect->ResourceBindings.size();++i)
		{
			ShaderPropertyPtr res = ShaderReflect->ResourceBindings[i].ParamRef;
			if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_CBUFFER )
			{
				if( res == NULL || res->GetDataAsConstBuffer() == NULL)
				{
					std::stringstream stream;
					stream << "vertex shader's constant buffer is null "<< GetShaderName() <<"," << ShaderReflect->ConstantBuffers[i].CBName;
					GLogger->Error(stream.str());
					continue;
				}
				CachedConstBuffer.push_back((ID3D11Buffer*)(res->GetDataAsConstBuffer()->GetLowInterface()));
			}
			else if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_TEXTURE )
			{			
				if( res == NULL || res->GetDataAsShaderResource() == NULL)
				{
					std::stringstream stream;
					stream << "vertex shader's shader resource is null "<< GetShaderName() <<"," << ShaderReflect->ResourceBindings[i].Name;
					GLogger->Error(stream.str());
					continue;
				}
				CachedShaderResourceView.push_back((ID3D11ShaderResourceView*)(res->GetDataAsShaderResource()->GetLowInterface(RSU_ShaderResource)));
			}
			else if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_SAMPLER )
			{
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
			std::ostringstream stream;
			stream << "the shader file :"<< fileName << "  entry point: ("<< entryPoint << ")   cannot be compiled.";
			MessageBoxA( NULL,stream.str().c_str(), "Error", MB_OK );
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

		RenderResourceManagerPtr resourceManager  = GEngine->RenderResourceMgr;
	 
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
  
		ShaderPropertyManagerPtr GlobalPropertyManager = resourceManager->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
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
				DX11ShaderPropertyManagerPtr propertyManager = boost::dynamic_pointer_cast<DX11ShaderPropertyManager>(resourceManager->GetPropertyManager(constantBuffer.CBName));		 
				BOOST_ASSERT(propertyManager->Name != ShaderPropertyManager::sManagerGlobal);

				constantBuffer.BufferParamRef = GlobalPropertyManager->GetProperty(constantBuffer.CBName);
				if( constantBuffer.BufferParamRef == NULL )
				{
					RenderBufferPtr constBuffer = resourceManager->CreateRenderBuffer(RBT_Constant,BAH_GPU_Read,bufferDesc.Size,bufferDesc.Size,NULL);
					constantBuffer.BufferParamRef = GlobalPropertyManager->CreateProperty(constantBuffer.CBName,eSP_ConstBuffer); 
					constantBuffer.BufferParamRef->SetData(constBuffer);
				}

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
					unsigned int length = 1;
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
						 shaderProperty = eSP_Float;
						 length = type_desc.Columns;
					}
					else if ( ( type_desc.Class == D3D_SVC_MATRIX_ROWS ) ||
								( type_desc.Class == D3D_SVC_MATRIX_COLUMNS ) )
					{
						shaderProperty = eSP_Float;
						if ( type_desc.Columns == 4 && type_desc.Rows == 4  ) 
						{
							length = 16;
						}
						else if( type_desc.Columns == 3 && type_desc.Rows == 3 )
						{
							length = 9;
						}
						else
						{
							BOOST_ASSERT(0);
						}
					}


					// we don't support array now
					BOOST_ASSERT(type_desc.Elements == 0);
					pParam = propertyManager->CreateProperty(varName,shaderProperty,length);

					constantBuffer.Parameters.push_back( pParam );
					 
				}
				
				bool result = propertyManager->Validate(&constantBuffer);
				if( !result )
				{
					std::ostringstream error;
					error << "Const buffer : " << constantBuffer.CBName << " have multi-defination ! in file : " <<  _fileName;
					GLogger->Error(error.str());
					return;
				}

				pReflection->ConstantBuffers.push_back( *propertyManager->ConstantBuffer );
			}
		}
 
		
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

	void DX11ShaderPropertyManager::UpdateShaderProperty()
	{
		if( ConstantBuffer == NULL )
			return;

		BYTE* pData = new BYTE[ConstantBuffer->CBSize];
		BYTE* pDest = pData;
		for(unsigned int j=0;j<ConstantBuffer->Parameters.size();j++)
		{
			ShaderVariableDesc &vaDesc = ConstantBuffer->Variables[j];
			ShaderTypeDesc &taDesc = ConstantBuffer->Types[j];
			ShaderPropertyPtr &paDesc =  ConstantBuffer->Parameters[j];
			pDest = pData + vaDesc.StartOffset;
				
			if( taDesc.Class == D3D_SVC_SCALAR && taDesc.Type == D3D_SVT_INT)
			{
				void *pSrc = paDesc->GetData();					
				memcpy(pDest,pSrc,sizeof(int));
			}
			else if(taDesc.Class == D3D_SVC_SCALAR && taDesc.Type == D3D_SVT_FLOAT)
			{
				void *pSrc = paDesc->GetData();
				memcpy(pDest,pSrc,sizeof(float));
			}

			if(taDesc.Class == D3D_SVC_VECTOR)
			{
				BYTE *pSrc = (BYTE*)(paDesc->GetData());
				if( taDesc.Columns == 3 ) // 128bit pack
				{
					//for( int n = 0;n<elementNumber;n++)
					//{
						memcpy(pDest,pSrc,sizeof(Eigen::Vector3f));
						//pDest += sizeof(Eigen::Vector4f);
						//pSrc += sizeof(Eigen::Vector3f);
					//}
				}
				else if( taDesc.Columns == 4 )
				{
					memcpy(pDest,pSrc,sizeof(Eigen::Vector4f));
				}
			}
			if(taDesc.Class == D3D_SVC_MATRIX_ROWS ||
				taDesc.Class == D3D_SVC_MATRIX_COLUMNS)
			{
				void *pSrc = paDesc->GetData();
				memcpy(pDest,pSrc,sizeof(Eigen::Matrix4f));
			}
		}
				
		GEngine->RenderEngine->UpdateSubresource(ConstantBuffer->BufferParamRef->GetDataAsConstBuffer(),pData,ConstantBuffer->CBSize);
		delete pData;
	}

	DX11ShaderPropertyManager::DX11ShaderPropertyManager(std::string name)
		:ShaderPropertyManager(name)
	{
		ConstantBuffer = NULL;
	}

	DX11ShaderPropertyManager::~DX11ShaderPropertyManager()
	{
		if( ConstantBuffer )
			delete ConstantBuffer;
		ConstantBuffer = NULL;
	}

	DX11ShaderPropertyManagerPtr DX11ShaderPropertyManager::Create(std::string name)
	{
		DX11ShaderPropertyManager *pManager = new DX11ShaderPropertyManager(name);
		return DX11ShaderPropertyManagerPtr(pManager);
	}

	bool DX11ShaderPropertyManager::Validate(ConstantBufferDesc *pConstBuffer)
	{
		BOOST_ASSERT(pConstBuffer != NULL);

		if( ConstantBuffer == NULL )
		{
			ConstantBuffer = new ConstantBufferDesc;
			ConstantBuffer->CBName = pConstBuffer->CBName;
			ConstantBuffer->CBSize = pConstBuffer->CBSize;
			ConstantBuffer->CBType = pConstBuffer->CBType;
			ConstantBuffer->CBuFlags = pConstBuffer->CBuFlags;
			ConstantBuffer->CBVariables = pConstBuffer->CBVariables;
			ConstantBuffer->BufferParamRef = pConstBuffer->BufferParamRef;
			ConstantBuffer->Parameters.assign(pConstBuffer->Parameters.begin(),pConstBuffer->Parameters.end());
			ConstantBuffer->Types.assign(pConstBuffer->Types.begin(),pConstBuffer->Types.end());
			ConstantBuffer->Variables.assign(pConstBuffer->Variables.begin(),pConstBuffer->Variables.end());
			return true;
		}
		else
		{
			if(ConstantBuffer->CBName != pConstBuffer->CBName )
				return false;
			if(ConstantBuffer->CBSize != pConstBuffer->CBSize )
				return false;
			if( ConstantBuffer->CBType != pConstBuffer->CBType )
				return false;
			if( ConstantBuffer->CBVariables != pConstBuffer->CBVariables )
				return false;

			if( ConstantBuffer->Variables.size() != pConstBuffer->Variables.size() )
				return false;

			for(size_t i=0;i<pConstBuffer->Variables.size();i++)
			{
				bool found = false;
				for( size_t j=0;j<ConstantBuffer->Variables.size();j++)
				{
					if( pConstBuffer->Variables[i].Name == ConstantBuffer->Variables[j].Name &&
						pConstBuffer->Variables[i].Size == ConstantBuffer->Variables[j].Size )
					{
						found = true;
						break;
					}
				}

				if( !found )
					return false;
			}

			return true;
		}
	}
 
}