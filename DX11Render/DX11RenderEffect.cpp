#include "DX11RenderInclude.h"
#include "../Engine/Logger.h"

namespace Disorder
{
	//=============================Shader object=========================================
	void DX11ShaderObject::PrepareRenderParam()
	{
		if( ShaderReflect->ConstantBuffers.size() > 0 && CachedConstBuffer.size() == 0)
		{
			for(int i=0;i<ShaderReflect->ConstantBuffers.size();i++)
			{
				MaterialParamCBufferPtr cbuff = ShaderReflect->ConstantBuffers[i].BufferParamRef;
				if( cbuff == NULL || cbuff->GetValue() == NULL)
				{
					std::stringstream stream;
					stream << "vertex shader's constant buffer is null "<< GetShaderName() <<"," << ShaderReflect->ConstantBuffers[i].CBName;
					GLogger->Error(stream.str());
					continue;
				}
				CachedConstBuffer.push_back((ID3D11Buffer*)(cbuff->GetValue()->GetLowInterface()));
			}
		}

		if( ShaderReflect->ResourceBindings.size() > 0 && CachedSamplerState.size() == 0 && CachedShaderResourceView.size() == 0)
		{
			for( int i=0;i<ShaderReflect->ResourceBindings.size();++i)
			{
				if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_TEXTURE )
				{
					MaterialParamShaderResPtr res = boost::dynamic_pointer_cast<MaterialParamShaderRes>(ShaderReflect->ResourceBindings[i].ParamRef);
					if( res == NULL || res->GetValue() == NULL)
					{
						std::stringstream stream;
						stream << "vertex shader's shader resource is null "<< GetShaderName() <<"," << ShaderReflect->ResourceBindings[i].Name;
						GLogger->Error(stream.str());
						continue;
					}
					CachedShaderResourceView.push_back((ID3D11ShaderResourceView*)(res->GetValue()->GetLowInterface()));
				}

				if( ShaderReflect->ResourceBindings[i].Type == D3D_SIT_SAMPLER )
				{
					MaterialParamSamplerStatePtr res = boost::dynamic_pointer_cast<MaterialParamSamplerState>(ShaderReflect->ResourceBindings[i].ParamRef);
					if( res == NULL || res->GetValue() == NULL)
					{
						std::stringstream stream;
						stream << "vertex shader's sampler state is null "<< GetShaderName() <<"," << ShaderReflect->ResourceBindings[i].Name;
						GLogger->Error(stream.str());
						continue;
					}
					CachedSamplerState.push_back((ID3D11SamplerState*)(res->GetValue()->GetLowInterface()));
				}
			}
		}
	}

	void DX11ShaderObject::UpdateRenderParam()
	{
		if(ShaderReflect->ConstantBuffers.size())
		{
			RenderEnginePtr renderEngine = GEngine->RenderEngine;
			for(int i=0;i<ShaderReflect->ConstantBuffers.size();i++)
			{
				//void *pDest = renderEngine->Map(ShaderReflect->ConstantBuffers[i].BufferParamRef->GetValue(),BA_Write_Only);
				BYTE* pData = new BYTE[ShaderReflect->ConstantBuffers[i].CBSize];
				BYTE* pDest = pData;
				for( int j=0;j<ShaderReflect->ConstantBuffers[i].Parameters.size();j++)
				{
					if(ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_VECTOR)
					{
						void *pSrc = ShaderReflect->ConstantBuffers[i].Parameters[j]->GetData();
						memcpy(pDest,pSrc,sizeof(Vector3));
						pDest = pDest + sizeof(Vector3);
					}
					if(ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_MATRIX_ROWS ||
					   ShaderReflect->ConstantBuffers[i].Types[j].Class == D3D_SVC_MATRIX_COLUMNS)
					{
						void *pSrc = ShaderReflect->ConstantBuffers[i].Parameters[j]->GetData();
						memcpy(pDest,pSrc,sizeof(Matrix4));
						Matrix4 w = *((Matrix4*)pDest);
						pDest = pDest + sizeof(Matrix4);
					}
				}
				
				
				//renderEngine->UnMap(ShaderReflect->ConstantBuffers[i].BufferParamRef->GetValue());
				renderEngine->UpdateSubresource(ShaderReflect->ConstantBuffers[i].BufferParamRef->GetValue(),pData,ShaderReflect->ConstantBuffers[i].CBSize);
				delete pData;

			}
		}
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
	 
	//==============================Effect==============================================

	HRESULT DX11RenderEffect::CompileShaderFromFile(std::string const& fileName, std::string const& entryPoint, std::string const& shaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		FileObjectPtr fileptr = GEngine->FileManager->OpenFile(fileName,std::ios::in);
		std::string shaderContent = GEngine->FileManager->ReadFile(fileptr);

		DWORD dwShaderFlags = 0; //D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;;
	#endif

	 
		ID3DBlob* pErrorBlob;
		hr = D3DCompile(shaderContent.c_str(),shaderContent.size(),"",NULL,NULL,entryPoint.c_str(),shaderModel.c_str(),dwShaderFlags, 0,ppBlobOut, &pErrorBlob);
	 
		/*hr = D3DX11CompileFromFileA( fileName.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(), 
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );*/
		if( FAILED(hr) )
		{
			if( pErrorBlob != NULL )
				OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			return hr;
		}
		if( pErrorBlob ) pErrorBlob->Release();

		return S_OK;
	}

	ShaderObjectPtr DX11RenderEffect::LoadShaderFromFile(std::string const& fileName,std::string const& entryPoint,ShaderType type)
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
				return ShaderObjectPtr();
			}

			DX11ShaderObjectPtr shader = boost::make_shared<DX11ShaderObject>(fileName,entryPoint,ST_VertexShader);
			shader->VertexShaderInterface =  MakeComPtr<ID3D11VertexShader>(_pVertexShader);
			shader->DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);

			ShaderReflection(shader);
			_vertexShader = shader;
			return shader;
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
				return ShaderObjectPtr();
			}

			DX11ShaderObjectPtr shader = boost::make_shared<DX11ShaderObject>(fileName,entryPoint,ST_PixelShader);
			shader->PixelShaderInterface =  MakeComPtr<ID3D11PixelShader>(_pPixelShader);
			shader->DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);
			ShaderReflection(shader);
			_pixelShader = shader;
			
			return shader;
		}
		
		BOOST_ASSERT(0);

		return ShaderObjectPtr();
	
	}

	void DX11RenderEffect::ShaderReflection(ShaderObjectPtr const& shader)
	{
		DX11ShaderObjectPtr dx11Shader = boost::dynamic_pointer_cast<DX11ShaderObject>(shader);
		DX11ShaderReflectionPtr pReflection = boost::make_shared<DX11ShaderReflection>();

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;

		ID3D11ShaderReflection* pReflector = NULL;
		ID3DBlob* shaderData = (ID3DBlob*)(shader->GetDataInterface());
		BOOST_ASSERT(shaderData);

		HRESULT hr = D3DReflect(shaderData->GetBufferPointer(), shaderData->GetBufferSize(),
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
			 
				constantBuffer.BufferParamRef = GetConstantBufferParameter(constantBuffer.CBName);
			 
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
					MaterialParamPtr pParam;
					std::string varname = var_desc.Name;
					if ( type_desc.Class == D3D_SVC_VECTOR )
					{					
						pParam = GetVectorParameter(varname);
					}
					else if ( ( type_desc.Class == D3D_SVC_MATRIX_ROWS ) ||
								( type_desc.Class == D3D_SVC_MATRIX_COLUMNS ) )
					{
						// Check if it is an array of matrices first...
						unsigned int count = type_desc.Elements;
						if ( count == 0 ) 
						{
							pParam = GetMatrixParameter(varname);
						}
						else
						{
							BOOST_ASSERT(0);
						}
					}

					constantBuffer.Parameters.push_back( pParam );
					 
				}
				 
				RenderBufferPtr constBuffer = resourceManager->CreateConstBuffer(bufferDesc.Size,BAH_GPU_Read);
				constantBuffer.BufferParamRef->SetValue(constBuffer);

				pReflection->ConstantBuffers.push_back( constantBuffer );
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
				binddesc.ParamRef = GetConstantBufferParameter(rname);
			}
			else if ( resource_desc.Type == D3D_SIT_TEXTURE || resource_desc.Type == D3D_SIT_STRUCTURED )
			{
				binddesc.ParamRef = GetShaderResourceParameter(rname);
			}
			else if ( resource_desc.Type == D3D_SIT_SAMPLER )
			{
				binddesc.ParamRef = GetSamplerStateParameter(rname);
			}
			else if ( resource_desc.Type == D3D_SIT_UAV_RWTYPED || resource_desc.Type == D3D_SIT_UAV_RWSTRUCTURED
				|| resource_desc.Type == D3D_SIT_BYTEADDRESS || resource_desc.Type == D3D_SIT_UAV_RWBYTEADDRESS
				|| resource_desc.Type == D3D_SIT_UAV_APPEND_STRUCTURED || resource_desc.Type == D3D_SIT_UAV_CONSUME_STRUCTURED
				|| resource_desc.Type == D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER )
			{
				binddesc.ParamRef = GetUnorderedAccessParameter(rname);
			}


			pReflection->ResourceBindings.push_back( binddesc );
		}


		// Release the shader reflection interface
		pReflector->Release();

		dx11Shader->ShaderReflect = pReflection;
		return;
	}
 
	void DX11RenderEffect::PrepareRenderParam()
	{
		if( _vertexShader != NULL )
			_vertexShader->PrepareRenderParam();
		if( _pixelShader != NULL )
			_pixelShader->PrepareRenderParam();
	}

	void DX11RenderEffect::UpdateRenderParam()
	{
		if( _vertexShader != NULL )
			_vertexShader->UpdateRenderParam();
		if( _pixelShader != NULL )
			_pixelShader->UpdateRenderParam();
	}

	MaterialParamCBufferPtr DX11RenderEffect::GetConstantBufferParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamCBuffer>(_materialParamMap.at(name));

		MaterialParamCBufferPtr constBuffer = boost::make_shared<MaterialParamCBuffer>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,constBuffer));

		return constBuffer;
	}

	MaterialParamVectorPtr DX11RenderEffect::GetVectorParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamVector>(_materialParamMap.at(name));

		MaterialParamVectorPtr vector = boost::make_shared<MaterialParamVector>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,vector));

		return vector;
	}

	MaterialParamMatrixPtr DX11RenderEffect::GetMatrixParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamMatrix>(_materialParamMap.at(name));

		MaterialParamMatrixPtr matrix = boost::make_shared<MaterialParamMatrix>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,matrix));

		return matrix;
	}

	MaterialParamShaderResPtr DX11RenderEffect::GetShaderResourceParameter(std::string const& name)
	{
		// texture and structed.
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamShaderRes>(_materialParamMap.at(name));

		MaterialParamShaderResPtr shaderres = boost::make_shared<MaterialParamShaderRes>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,shaderres));

		return shaderres;
	}

	MaterialParamSamplerStatePtr DX11RenderEffect::GetSamplerStateParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamSamplerState>(_materialParamMap.at(name));

		MaterialParamSamplerStatePtr ss = boost::make_shared<MaterialParamSamplerState>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,ss));

		return ss;
	}

	MaterialParamUnorderedPtr DX11RenderEffect::GetUnorderedAccessParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamUnordered>(_materialParamMap.at(name));

		MaterialParamUnorderedPtr ua = boost::make_shared<MaterialParamUnordered>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,ua));

		return ua;
	}
}