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
 
	DX11RenderBufferPtr DX11RenderBuffer::Create(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData)
	{
		DX11RenderBuffer *pBuffer = new DX11RenderBuffer;

		pBuffer->_type = type;
		pBuffer->_accessHint = accessHint;
		pBuffer->_elementSize = elementSize;
		pBuffer->_bufferSize = size;

		if( pBuffer->_type == RBT_Vertex )
		{		
			pBuffer->_bindFlags = D3D11_BIND_VERTEX_BUFFER;
		}
		else if( pBuffer->_type == RBT_Index )
		{
			pBuffer->_bindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		else if( pBuffer->_type == RBT_Constant )
		{
			pBuffer->_bindFlags = D3D11_BIND_CONSTANT_BUFFER;
		}
		else
		{
			BOOST_ASSERT(0);
		}

		pBuffer->DoCreateBuffer(pData);

		return DX11RenderBufferPtr(pBuffer);
 
	}

	DX11RenderBufferPtr DX11RenderBuffer::Create(RenderBufferType type,GeometryPtr const& data,std::string const& sematic,unsigned int accessHint,ShaderObjectPtr const& vertexShader)
	{
		DX11RenderBuffer *pBuffer = new DX11RenderBuffer;
 
		pBuffer->_accessHint = accessHint;
		pBuffer->_elementSize = 0;
		pBuffer->_bufferSize = 0;
		pBuffer->_type = type;
		
		void *pData = NULL;
		std::vector<float> vData;
  
		if( type == RBT_Vertex )
		{
			pBuffer->_bindFlags = D3D11_BIND_VERTEX_BUFFER;

			DX11ShaderObjectPtr shader = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader);
			for(unsigned int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
			{
				if(shader->ShaderReflect->InputSignatureParameters[i].SemanticName.compare(sematic) == 0 )
				{
					pBuffer->_elementSize = shader->ShaderReflect->InputSignatureParameters[i].GetElementSize();
					break;
				}		 
			}

			if( pBuffer->_elementSize == 0 )
			{
				BOOST_ASSERT(0);
				delete pBuffer;
				return NULL;
			}

			if(sematic.compare(RenderLayout::POSITION) == 0 )
			{
				pBuffer->_bufferSize = pBuffer->_elementSize*data->Positions.size();
				for(unsigned int index=0;index<data->Positions.size();++index)
				{
					glm::vec3& vec = data->ControllPositions[data->Positions[index]];
					vData.push_back(vec.x);
					vData.push_back(vec.y);
					vData.push_back(vec.z);
				}
			}
			else if(sematic.compare(RenderLayout::COLOR) == 0 )
			{
				pBuffer->_bufferSize = pBuffer->_elementSize*data->Colors.size();
				for(unsigned int index=0;index<data->Colors.size();++index)
				{
					glm::vec4& vec = data->Colors[index];
					vData.push_back(vec.x);
					vData.push_back(vec.y);
					vData.push_back(vec.z);
					vData.push_back(vec.w);
				}
			}
			else if(sematic.compare(RenderLayout::NORMAL) == 0 )
			{
				pBuffer->_bufferSize = pBuffer->_elementSize*data->Normals.size();
				for(unsigned int index=0;index<data->Normals.size();++index)
				{
					glm::vec3& vec = data->Normals[index];
					vData.push_back(vec.x);
					vData.push_back(vec.y);
					vData.push_back(vec.z);
				}
			}
			else if(sematic.compare(RenderLayout::TEXCOORD) == 0 )
			{
				pBuffer->_bufferSize = pBuffer->_elementSize*data->Texcoords.size();
				for(unsigned int index=0;index<data->Texcoords.size();++index)
				{
					glm::vec2& vec = data->Texcoords[index];
					vData.push_back(vec.x);
					vData.push_back(vec.y);
				}
			}
			else
			{
				BOOST_ASSERT(0);
			}

			  pData = vData.data();
	    }
		else if( pBuffer->_type == RBT_Index )
		{
			pBuffer->_bindFlags = D3D11_BIND_INDEX_BUFFER;
			if( data->Indices.size() > 0 )
			{
				pBuffer->_elementSize = sizeof(unsigned int);
				pBuffer->_bufferSize = sizeof(unsigned int) * data->Indices.size();
				pData = data->Indices.data();
			}
		}

		BOOST_ASSERT(pBuffer->_bufferSize > 0 );

	  
		pBuffer->DoCreateBuffer(pData);

		return DX11RenderBufferPtr(pBuffer);

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
				if( _bindFlags != D3D11_BIND_VERTEX_BUFFER && _bindFlags != D3D11_BIND_INDEX_BUFFER )
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

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat,ImagePtr const& image)
	{
		const ImageSpec &spec = image->GetSpec();
		_BufferInitData data;
		data.Data = image->GetImageData();
		data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat)/8 * spec.width;
		data.SlicePitch = 0;
		return Create(pixelFormat,spec.width,spec.height,false,RSU_ShaderResource,&data);
	}

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,unsigned int usage,BufferInitData const* pData)
	{
		DX11RenderTexture2D* pTexture = new DX11RenderTexture2D;

		pTexture->Format = pixelFormat;
		pTexture->Width = width;
		pTexture->Height = height;


		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		
		if( bMipmap )
		{
			desc.MipLevels = pTexture->MipLevel = 0;
		}
		else 
		{
			desc.MipLevels = pTexture->MipLevel = 1;
			desc.SampleDesc.Count = 1; // just set it to 1...
			desc.SampleDesc.Quality = 0;
		}

		desc.ArraySize = 1;
		desc.Format = DX11RenderEngine::GetPixelFormat(pixelFormat);
		desc.Usage = D3D11_USAGE_DEFAULT;
		if( usage & RSU_RenderTarget )
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		if( usage & RSU_DepthStencil )
			desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		if( usage & RSU_ShaderResource )
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 

		ID3D11Texture2D* pTex2D = NULL;
		if( pData != NULL )
		{
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory( &InitData, sizeof(InitData) );
			InitData.pSysMem = pData->Data;
			InitData.SysMemPitch = pData->RowPitch;
			InitData.SysMemSlicePitch = pData->SlicePitch;
			HRESULT hr = renderEngine->D3DDevice()->CreateTexture2D( &desc, &InitData, &pTex2D );
			BOOST_ASSERT(hr==S_OK);
		}
		else
		{
			HRESULT hr = renderEngine->D3DDevice()->CreateTexture2D( &desc, NULL, &pTex2D );
			BOOST_ASSERT(hr==S_OK);
		}

		pTexture->D3DInterface = MakeComPtr<ID3D11Texture2D>(pTex2D);
 
		return DX11RenderTexture2DPtr(pTexture);

	}
	 
	

	void * DX11RenderTexture2D::GetLowInterface()
	{
		return D3DInterface.get();
	}

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,ID3D11Texture2DPtr DXInterface)
	{
		DX11RenderTexture2D *pTex = new DX11RenderTexture2D;
		pTex->Width = width;
		pTex->Height = height;
		pTex->Format = pixelFormat;
		pTex->D3DInterface = DXInterface;

		return DX11RenderTexture2DPtr(pTex);
	}
	 
	
}