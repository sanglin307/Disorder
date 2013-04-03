#include "DX11RenderInclude.h"


namespace Disorder
{
	//=============================Vertex Shader=========================================
	void DX11ShaderObject::Load(std::string const& entryPoint)
	{

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

			DX11ShaderObjectPtr shader = boost::make_shared<DX11ShaderObject>(entryPoint,ST_VertexShader);
			shader->VertexShaderInterface =  MakeComPtr<ID3D11VertexShader>(_pVertexShader);
			shader->DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);
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

			DX11ShaderObjectPtr shader = boost::make_shared<DX11ShaderObject>(entryPoint,ST_PixelShader);
			shader->PixelShaderInterface =  MakeComPtr<ID3D11PixelShader>(_pPixelShader);
			_pixelShader = shader;
			shader->DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);
			return shader;
	
		}
		
		BOOST_ASSERT(0);

		return ShaderObjectPtr();
	
	}
 

}