#include "DX11RenderInclude.h"


namespace Disorder
{
	//=============================Vertex Shader=========================================
	void DX11VertexShader::Load(std::string const& entryPoint)
	{

	}

	void* DX11VertexShader::GetLowInterface()
	{
		if( ShaderInterface != NULL )
			return ShaderInterface.get();

		return 0;
	}

	void* DX11VertexShader::GetDataInterface()
	{
		if ( DataInterface != NULL )
			return DataInterface.get();

		return 0;
	}
	//==============================Pixel Shader==========================================

	void DX11PixelShader::Load(std::string const& entryPoint)
	{
	}

	void* DX11PixelShader::GetLowInterface()
	{
		if( ShaderInterface != NULL )
			return ShaderInterface.get();

		return 0;
	}

	void* DX11PixelShader::GetDataInterface()
	{
		if ( DataInterface != NULL )
			return DataInterface.get();

		return 0;
	}
	//==============================Technique==============================================

	HRESULT DX11RenderTechnique::CompileShaderFromFile(std::string const& fileName, std::string const& entryPoint, std::string const& shaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFileA( fileName.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(), 
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
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

	VertexShaderPtr DX11RenderTechnique::LoadVertexShaderFromFile(std::string const& fileName,std::string const& entryPoint)
	{
		HRESULT hr = S_OK;
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile(fileName, entryPoint, GetVertexShaderModelDes(), &pVSBlob );
		if( FAILED( hr ) )
		{
			MessageBox( NULL,
						L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return VertexShaderPtr();
		}

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		BOOST_ASSERT(renderEngine);

		// Create the vertex shader
		ID3D11VertexShader* _pVertexShader = NULL;
		hr = renderEngine->D3DDevice()->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_pVertexShader );
		if( FAILED( hr ) )
		{	
			pVSBlob->Release();
			MessageBox( NULL,
						L"CreateVertexShader Failed", L"Error", MB_OK );
			return VertexShaderPtr();
		}

		DX11VertexShaderPtr shader = boost::make_shared<DX11VertexShader>(entryPoint);
		shader->ShaderInterface =  MakeComPtr<ID3D11VertexShader>(_pVertexShader);
		shader->DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);
		_vertexShader = shader;
		return shader;
	}

	PixelShaderPtr DX11RenderTechnique::LoadPixelShaderFromFile(std::string const& fileName,std::string const& entryPoint)
	{
		HRESULT hr = S_OK;
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile(fileName, entryPoint, GetPixelShaderModelDes(), &pVSBlob );
		if( FAILED( hr ) )
		{
			MessageBox( NULL,
						L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return PixelShaderPtr();
		}

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		BOOST_ASSERT(renderEngine);

		// Create the vertex shader
		ID3D11PixelShader* _pPixelShader = NULL;
		hr = renderEngine->D3DDevice()->CreatePixelShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_pPixelShader );
		if( FAILED( hr ) )
		{	
			pVSBlob->Release();
			MessageBox( NULL,
						L"CreatePixelShader Failed", L"Error", MB_OK );
			return PixelShaderPtr();
		}

		DX11PixelShaderPtr shader = boost::make_shared<DX11PixelShader>(entryPoint);
		shader->ShaderInterface =  MakeComPtr<ID3D11PixelShader>(_pPixelShader);
		shader->DataInterface = MakeComPtr<ID3DBlob>(pVSBlob);
		_pixelShader = shader;
		return shader;
	}

}