#include "DX11RenderInclude.h"


namespace Disorder
{
	void* DX11RenderBuffer::GetHandle()
	{
		return D3DInterface;
	}

	void DX11RenderBuffer::Resize(unsigned int size)
	{
		// release old and create new one
		D3DInterface->Release();

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

		DX11RenderEngine* renderEngine = (DX11RenderEngine*)GRenderEngine; 
		 
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

 
		D3DInterface = pBuffer;
	}
 
	DX11RenderBuffer::DX11RenderBuffer(const std::string& bufferName, RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData)
	{ 
		_type = type;
		_bufferUsage = bufferUsage;
		_elementSize = elementSize;
	    _bufferSize = size;

		if( _type == RBT_Vertex )
		{		
			_bindFlags = D3D11_BIND_VERTEX_BUFFER;
		}
		else if( _type == RBT_Index )
		{
			_bindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		else if( _type == RBT_Constant )
		{
			_bindFlags = D3D11_BIND_CONSTANT_BUFFER;
		}
		else
		{
			BOOST_ASSERT(0);
		}

		DoCreateBuffer(pData);
		D3DInterface->SetPrivateData(WKPDID_D3DDebugObjectName, bufferName.size(), bufferName.c_str());
	}

	DX11RenderBuffer::DX11RenderBuffer(const std::string& bufferName, RenderBufferType type, Geometry* data, std::string const& sematic, BufferUsage bufferUsage, ShaderObject* vertexShader)
	{
		_bufferUsage = bufferUsage;
		_elementSize = 0;
		_bufferSize = 0;
		_type = type;
		
		void *pData = NULL;
		std::vector<float> vData;
  
		if( type == RBT_Vertex )
		{
			_bindFlags = D3D11_BIND_VERTEX_BUFFER;

			DX11ShaderObject* shader = (DX11ShaderObject*)vertexShader;
			for(unsigned int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
			{
				if(shader->ShaderReflect->InputSignatureParameters[i].SemanticName.compare(sematic) == 0 )
				{
					_elementSize = shader->ShaderReflect->InputSignatureParameters[i].GetElementSize();
					break;
				}		 
			}

			if( _elementSize == 0 )
			{
				BOOST_ASSERT(0);
			}

			if(sematic.compare(DX11RenderLayout::POSITION) == 0 )
			{
				_bufferSize = _elementSize*data->Positions.size();
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
				_bufferSize = _elementSize*data->Colors.size();
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
				_bufferSize = _elementSize*data->Normals.size();
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
				_bufferSize = _elementSize*data->Texcoords.size();
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

		BOOST_ASSERT(_bufferSize > 0 );

		DoCreateBuffer(pData);
		D3DInterface->SetPrivateData(WKPDID_D3DDebugObjectName, bufferName.size(), bufferName.c_str());
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

	DX11RenderTexture2D::DX11RenderTexture2D(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		InternalCreate(pixelFormat, width, height, bMipmap, bMultiSample, viewFlag, arraySize, pData, flag);
	}

	DX11RenderTexture2D::DX11RenderTexture2D(PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag)
	{
		if (image.size() == 0)
		{
			BOOST_ASSERT(0);
			return;
		}
		 
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

		InternalCreate(pixelFormat, spec.width, spec.height, false, bMultiSample, SV_ShaderResource, image.size(), vBufferInitData.data(), flag);
		delete pData;
	}

	DX11RenderTexture2D::DX11RenderTexture2D(PixelFormat pixelFormat, bool bMultiSample,Image* image)
	{
		const ImageSpec &spec = image->GetSpec();
		BufferInitData data;
		data.Data = image->GetImageData();
		data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat)/8 * spec.width;
		data.SlicePitch = 0;
		InternalCreate(pixelFormat, spec.width, spec.height, false, bMultiSample, SV_ShaderResource, 1, &data, 0);
	}

	void DX11RenderTexture2D::InternalCreate(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		Format = pixelFormat;
		Width = width;
	    Height = height;
		ViewFlag = viewFlag;
		ArraySize = arraySize;

		if (bMultiSample && !pData)
		{
			MultiSampleCount = GConfig->pRenderConfig->MultiSampleCount;
			MultiSampleQuality = GConfig->pRenderConfig->MultiSampleQuality;
		}
		else
		{
			MultiSampleCount = 1;
			MultiSampleQuality = 0;
		}

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		
		if( bMipmap )
		{
			desc.MipLevels = MipLevel = 0;
			desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
		else 
		{
			desc.MipLevels = MipLevel = 1;
		}
 
		desc.SampleDesc.Count = MultiSampleCount;
		desc.SampleDesc.Quality = MultiSampleQuality;
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

		DX11RenderEngine* renderEngine = (DX11RenderEngine*)GRenderEngine; 

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

		D3DInterface = pTex2D;
	}
	 
	

	void * DX11RenderTexture2D::GetHandle()
	{
		return D3DInterface;
	}

	DX11RenderTexture2D::DX11RenderTexture2D(PixelFormat pixelFormat, unsigned int width, unsigned int height, unsigned int viewFlag, bool bmipmap, unsigned int multiSampleCount, unsigned int multiSampleQuality, ID3D11Texture2D* DXInterface)
	{
		Width = width;
		Height = height;
		Format = pixelFormat;
		D3DInterface = DXInterface;
		Sampler = NULL;
		ViewFlag = viewFlag;
		MipLevel = bmipmap ? 0 : 1;
		MultiSampleCount = multiSampleCount;
		MultiSampleQuality = multiSampleQuality;
		ArraySize = 1;
	}
	 
 
}