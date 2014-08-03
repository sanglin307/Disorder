#include "DX11RenderInclude.h"


namespace Disorder
{
	void* DX11RenderBuffer::GetHandle()
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
		bd.BindFlags = _bindFlags;
		GetD3DFlags(pData,bd.Usage,bd.CPUAccessFlags,bd.MiscFlags);

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
 
	DX11RenderBufferPtr DX11RenderBuffer::Create(const std::string& bufferName, RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData)
	{
		DX11RenderBuffer *pBuffer = new DX11RenderBuffer;

		pBuffer->_type = type;
		pBuffer->_bufferUsage = bufferUsage;
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
		pBuffer->D3DInterface->SetPrivateData(WKPDID_D3DDebugObjectName, bufferName.size(), bufferName.c_str());

		return DX11RenderBufferPtr(pBuffer);
 
	}

	DX11RenderBufferPtr DX11RenderBuffer::Create(const std::string& bufferName, RenderBufferType type, GeometryPtr const& data, std::string const& sematic, BufferUsage bufferUsage, ShaderObjectPtr const& vertexShader)
	{
		DX11RenderBuffer *pBuffer = new DX11RenderBuffer;
 
		pBuffer->_bufferUsage = bufferUsage;
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

			if(sematic.compare(DX11RenderLayout::POSITION) == 0 )
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
			else if(sematic.compare(DX11RenderLayout::COLOR) == 0 )
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
			else if(sematic.compare(DX11RenderLayout::NORMAL) == 0 )
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
			else if(sematic.compare(DX11RenderLayout::TEXCOORD) == 0 )
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
		pBuffer->D3DInterface->SetPrivateData(WKPDID_D3DDebugObjectName, bufferName.size(), bufferName.c_str());

		return DX11RenderBufferPtr(pBuffer);

	}
 
  

	void DX11RenderBuffer::GetD3DFlags(void *pData,D3D11_USAGE& usage, UINT& cpuAccessFlags, UINT& miscFlags)
	{
		if( _bufferUsage == BU_StaticDraw )
		{
			if( pData != NULL )
				usage = D3D11_USAGE_IMMUTABLE;
			else
				BOOST_ASSERT(0);
		}
		else if( _bufferUsage == BU_StaticCopy)
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
			usage = D3D11_USAGE_STAGING;
		}
		else if(_bufferUsage == BU_StaticRead)
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
			usage = D3D11_USAGE_STAGING;
		}
		else if(_bufferUsage == BU_DynamicCopy )
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
			usage = D3D11_USAGE_STAGING;
		}
		else if(_bufferUsage == BU_DynamicRead )
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
			usage = D3D11_USAGE_STAGING;
		}
		else if(_bufferUsage == BU_DynamicDraw )
		{		
			usage = D3D11_USAGE_DYNAMIC;
			cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE; 
		}
		else if(_bufferUsage == BU_StreamRead )
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
			usage = D3D11_USAGE_STAGING;
		}
		else if(_bufferUsage == BU_StreamCopy )
		{
			cpuAccessFlags |= D3D11_CPU_ACCESS_READ;
			usage = D3D11_USAGE_STAGING;
		}
		else if(_bufferUsage == BU_StreamDraw )
		{
			if( pData != NULL )
				usage = D3D11_USAGE_DEFAULT;
			else
				BOOST_ASSERT(0);
		}

	}

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat, bool bMultiSample, const std::vector<ImagePtr>& image, unsigned int flag)
	{
		if (image.size() == 0)
			return NULL;

		const ImageSpec &spec = image[0]->GetSpec();
		BYTE* pData = new BYTE[spec.dataSize*image.size()];
		BYTE* pDest = pData;
		std::vector<BufferInitData> vBufferInitData;
		for (size_t i = 0; i < image.size(); i++)
		{
			BOOST_ASSERT(spec.dataSize == image[i]->GetSpec().dataSize);
			memcpy(pDest, image[i]->GetImageData(), spec.dataSize);
			BufferInitData data;
			data.Data = pDest;
			data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat) / 8 * spec.width;
			data.SlicePitch = 0;
			vBufferInitData.push_back(data);
			pDest += spec.dataSize;

		}

		DX11RenderTexture2DPtr result = Create(pixelFormat, spec.width, spec.height, false, bMultiSample, SV_ShaderResource, image.size(), vBufferInitData.data(),flag);
		delete pData;

		return result;
	}

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat, bool bMultiSample,ImagePtr const& image)
	{
		const ImageSpec &spec = image->GetSpec();
		BufferInitData data;
		data.Data = image->GetImageData();
		data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat)/8 * spec.width;
		data.SlicePitch = 0;
		return Create(pixelFormat,spec.width,spec.height,false,bMultiSample,SV_ShaderResource,1,&data,0);
	}

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		DX11RenderTexture2D* pTexture = new DX11RenderTexture2D;

		pTexture->Format = pixelFormat;
		pTexture->Width = width;
		pTexture->Height = height;
		pTexture->ViewFlag = viewFlag;
		pTexture->ArraySize = arraySize;

		if (bMultiSample && !pData)
		{
			pTexture->MultiSampleCount = GConfig->pRenderConfig->MultiSampleCount;
			pTexture->MultiSampleQuality = GConfig->pRenderConfig->MultiSampleQuality;
		}
		else
		{
			pTexture->MultiSampleCount = 1;
			pTexture->MultiSampleQuality = 0;
		}

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		
		if( bMipmap )
		{
			desc.MipLevels = pTexture->MipLevel = 0;
			desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
		else 
		{
			desc.MipLevels = pTexture->MipLevel = 1;
		}
 
		desc.SampleDesc.Count = pTexture->MultiSampleCount;
		desc.SampleDesc.Quality = pTexture->MultiSampleQuality;
		desc.ArraySize = arraySize;
		desc.Format = DX11RenderEngine::GetPixelFormat(pixelFormat);
		desc.Usage = D3D11_USAGE_DEFAULT;

		 
		if ( viewFlag & SV_ShaderResource )
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		if (viewFlag & SV_DepthStencil )
			desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		if ( viewFlag & SV_RenderTarget )
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		 

		desc.CPUAccessFlags = 0;
		// cube map
		if (arraySize == 6 && (flag & SF_AsCubeMap))
			desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 

		ID3D11Texture2D* pTex2D = NULL;
		if( pData != NULL )
		{
			std::vector<D3D11_SUBRESOURCE_DATA> vInitData;
			for (int i = 0; i < arraySize; i++)
			{
				D3D11_SUBRESOURCE_DATA InitData;
				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = pData->Data;
				InitData.SysMemPitch = pData->RowPitch;
				InitData.SysMemSlicePitch = pData->SlicePitch;
				vInitData.push_back(InitData);
				pData ++;
			}
			
			HRESULT hr = renderEngine->D3DDevice()->CreateTexture2D(&desc, vInitData.data(), &pTex2D);
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
	 
	

	void * DX11RenderTexture2D::GetHandle()
	{
		return D3DInterface.get();
	}

	DX11RenderTexture2DPtr DX11RenderTexture2D::Create(PixelFormat pixelFormat, unsigned int width, unsigned int height, unsigned int viewFlag, bool bmipmap, unsigned int multiSampleCount, unsigned int multiSampleQuality, ID3D11Texture2DPtr DXInterface)
	{
		DX11RenderTexture2D *pTex = new DX11RenderTexture2D;
		pTex->Width = width;
		pTex->Height = height;
		pTex->Format = pixelFormat;
		pTex->D3DInterface = DXInterface;
		pTex->Sampler = NULL;
		pTex->ViewFlag = viewFlag;
		pTex->MipLevel = bmipmap ? 0 : 1;
		pTex->MultiSampleCount = multiSampleCount;
		pTex->MultiSampleQuality = multiSampleQuality;
		pTex->ArraySize = 1;
		return DX11RenderTexture2DPtr(pTex);
	}
	 
 
}