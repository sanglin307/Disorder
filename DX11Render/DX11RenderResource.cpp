#include "DX11RenderInclude.h"


namespace Disorder
{
	void* DX11RenderBuffer::GetLowInterface()
	{
		if( D3DInterface != NULL )
			return D3DInterface.get();

		return 0;
	}

	void DX11RenderBuffer::Resize(unsigned int size)
	{
		// release old and create new one
		D3DInterface.reset();

		_bufferSize = size;
		DoCreateBuffer(NULL);
	}

	void DX11RenderBuffer::DoCreateBuffer(void *pData)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.ByteWidth = _bufferSize;
		GetD3DFlags(bd.Usage,bd.CPUAccessFlags,bd.BindFlags,bd.MiscFlags);

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		BOOST_ASSERT(renderEngine);

		ID3D11Buffer *pBuffer = NULL;

		if( pData != NULL )
		{
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory( &InitData, sizeof(InitData) );
			InitData.pSysMem = pData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			HRESULT hr = renderEngine->D3DDevice()->CreateBuffer( &bd, &InitData, &pBuffer );
			BOOST_ASSERT(hr==S_OK);
		}
		else
		{
			HRESULT hr = renderEngine->D3DDevice()->CreateBuffer( &bd, NULL, &pBuffer );
			BOOST_ASSERT(hr==S_OK);
		}

		D3DInterface = MakeComPtr<ID3D11Buffer>(pBuffer);
	}

	void DX11RenderBuffer::CreateConstBuffer(unsigned int size, unsigned int accessHint)
	{
		_bindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_type = RBT_Constant;
		_accessHint = accessHint;
		_elementSize = size;
		_bufferSize = size;
		DoCreateBuffer(0);
 
	}

	void DX11RenderBuffer::CreateBuffer(RenderBufferType type,GeometryPtr const& data,unsigned int accessHint,ShaderObjectPtr const& vertexShader)
	{
		_type = type;
		_accessHint = accessHint;
		_elementSize = 0;
		_bufferSize = 0;
 
		void *pData = NULL;

		if( _type == RBT_Vertex )
		{
			std::vector<float> vData;
			_bindFlags = D3D11_BIND_VERTEX_BUFFER;
			bool bPositionData = false;
			bool bNormalData = false;
			bool bTexcoodData = false;
			DX11ShaderObjectPtr shader = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader);
			for(int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
			{
				if(strcmp(shader->ShaderReflect->InputSignatureParameters[i].SemanticName,"POSITION") == 0 )
				{
					bPositionData = true;
					continue;
				}
				if(strcmp(shader->ShaderReflect->InputSignatureParameters[i].SemanticName,"NORMAL") == 0 )
				{
					bNormalData = true;
					continue;
				}
				if(strcmp(shader->ShaderReflect->InputSignatureParameters[i].SemanticName,"TEXCOORD0") == 0 )
				{
					bTexcoodData = true;
					continue;
				}
			}
			if(bPositionData && data->Positions.size() > 0)
			{
				_elementSize += sizeof(Vector3);
				_bufferSize += sizeof(Vector3) * data->Positions.size();
			}

			if(bNormalData && data->Normals.size() > 0)
			{
				_elementSize += sizeof(Vector3);
				_bufferSize += sizeof(Vector3) * data->Normals.size();
			}

			if(bTexcoodData && data->Texcoords.size() > 0)
			{
				_elementSize += sizeof(Vector2);
				_bufferSize += sizeof(Vector2) * data->Texcoords.size();
			}

			//data
			if(bPositionData && data->Positions.size() > 0 )
			{
				for(int index=0;index<data->Positions.size();++index)
				{
					vData.push_back(data->Positions[index].x);
					vData.push_back(data->Positions[index].y);
					vData.push_back(data->Positions[index].z);
					if( bNormalData && data->Normals.size() > index )
					{
						vData.push_back(data->Normals[index].x);
						vData.push_back(data->Normals[index].y);
						vData.push_back(data->Normals[index].z);
					}
					else
					{
						BOOST_ASSERT(0);
					}
					if( bTexcoodData && data->Texcoords.size() > index )
					{
						vData.push_back(data->Texcoords[index].x);
						vData.push_back(data->Texcoords[index].y);
					}
					else
					{
						BOOST_ASSERT(0);
					}
				}
				pData = vData.data();
			}

		}
		else if( _type == RBT_Index )
		{
			_bindFlags = D3D11_BIND_INDEX_BUFFER;
			if( data->Indices.size() > 0 )
			{
				_elementSize = sizeof(unsigned int);
				_bufferSize = sizeof(unsigned int) * data->Indices.size();
				pData = data->Indices.data();
			}
		}
		else
		{
			BOOST_ASSERT(0);
		}

		DoCreateBuffer(pData);
	}

  

	void DX11RenderBuffer::GetD3DFlags(D3D11_USAGE& usage, UINT& cpuAccessFlags, UINT& bindFlags, UINT& miscFlags)
	{
		if (_accessHint & BAH_Immutable)
		{
			usage = D3D11_USAGE_IMMUTABLE; // Only GPU read, Must init when create
		}
		else
		{
			if ((BAH_CPU_Write == _accessHint) || ((BAH_CPU_Write | BAH_GPU_Read) == _accessHint))
			{
				usage = D3D11_USAGE_DYNAMIC;
			}
			else
			{
				if (!(_accessHint & BAH_CPU_Read) && !(_accessHint & BAH_CPU_Write))
				{
					usage = D3D11_USAGE_DEFAULT; //A resource that requires read and write access by the GPU
				}
				else
				{
					usage = D3D11_USAGE_STAGING;//A resource that supports data transfer (copy) from the GPU to the CPU.

				}
			}
		}

		cpuAccessFlags = 0;
		if (_accessHint & BAH_CPU_Read)
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
		}
		if (_accessHint & BAH_CPU_Write)
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		}

		if (D3D10_USAGE_STAGING == usage)
		{
			bindFlags = 0;
		}
		else
			bindFlags = _bindFlags;

 
		if( _bindFlags != D3D11_BIND_CONSTANT_BUFFER )
		{
			if (_accessHint & BAH_GPU_Read)
			{
				bindFlags |= D3D11_BIND_SHADER_RESOURCE;
			}
			if (_accessHint & BAH_GPU_Write)
			{
				bindFlags |= D3D11_BIND_STREAM_OUTPUT;
			}
			if (_accessHint & BAH_GPU_Unordered)
			{
				bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
			}
		}

		miscFlags = 0;
		if (_accessHint & BAH_GPU_Unordered)
		{
			miscFlags = (_accessHint & BAH_GPU_Structured)
				? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
	}


	bool DX11RenderTexture2D::Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,BufferInitData const* pData)
	{
		Format = pixelFormat;
		Width = width;
		Height = height;


		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		
		if( bMipmap )
		{
			desc.MipLevels = MipLevel = 0;
		}
		else 
		{
			desc.MipLevels = MipLevel = 1;
			desc.SampleDesc.Count = 1; // just set it to 1...
			desc.SampleDesc.Quality = 0;
		}

		desc.ArraySize = 1;
		desc.Format = (DXGI_FORMAT)GPixelFormats[pixelFormat].PlatformFormat;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 

		ID3D11Texture2D* pTex2D = NULL;
		if( pData != NULL )
		{
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory( &InitData, sizeof(InitData) );
			InitData.pSysMem = pData->Data;
			InitData.SysMemPitch = GPixelFormats[pixelFormat].BlockBytes * Width;
			InitData.SysMemSlicePitch = 0;
			HRESULT hr = renderEngine->D3DDevice()->CreateTexture2D( &desc, &InitData, &pTex2D );
			BOOST_ASSERT(hr==S_OK);
		}
		else
		{
			HRESULT hr = renderEngine->D3DDevice()->CreateTexture2D( &desc, NULL, &pTex2D );
			BOOST_ASSERT(hr==S_OK);
		}

		D3DInterface = MakeComPtr<ID3D11Texture2D>(pTex2D);

		//save it and test
		D3DX11SaveTextureToFile(renderEngine->D3DImmediateContext().get(),pTex2D,D3DX11_IFF_DDS,L"..\\Resource\\Font\\test.dds");

		return true;

	}
	 
	

	void * DX11RenderTexture2D::GetLowInterface()
	{
		return D3DInterface.get();
	}


	
}