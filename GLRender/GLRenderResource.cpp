#include "GLRenderInclude.h"

namespace Disorder
{
	GLRenderBuffer::GLRenderBuffer(RenderBufferType type,Geometry* data,GLint location,BufferUsage bufferUsage,RenderEffect* effect)
	{
		GLRenderEffect* glEffect = (GLRenderEffect*)effect;
		
		int elementSize = 0;
		int bufferSize = 0;

		void *pData = NULL;
		std::vector<float> vData;
		_type = type;
		_bufferUsage = bufferUsage;
		

		if( type == RBT_Vertex )
		{
			for(size_t i=0;i<glEffect->EffectReflection->InputArray.size();i++ )
			{
				if( glEffect->EffectReflection->InputArray[i].Location == location )
				{
					elementSize = GLRenderLayout::GetGLTypeSize(glEffect->EffectReflection->InputArray[i].Type);
					break;
				}
			}

			if( elementSize == 0 )
			{
				BOOST_ASSERT(0);
				return;
			}
 
			_elementSize = elementSize;
			if( location == GLRenderLayout::Location_Position )
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
			else if(location == GLRenderLayout::Location_Color )
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
			else if(location == GLRenderLayout::Location_Normal )
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
			else if(location == GLRenderLayout::Location_Tex0 )
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
		else if( type == RBT_Index )
		{
			if( data->Indices.size() > 0 )
			{
				_elementSize = sizeof(unsigned int);
				_bufferSize = sizeof(unsigned int) * data->Indices.size();
				pData = data->Indices.data();
			}
			else
			{
				BOOST_ASSERT(0);
				return;
	
			}
		}
		else
		{
			BOOST_ASSERT(0);
			return;
		}

	    DoCreateBuffer(pData);

	}

	void * GLRenderBuffer::GetHandle()
	{
		return (void*)_bufferHandle;
	}
	
	void GLRenderBuffer::Resize(unsigned int size)
	{
		if( _bufferHandle == 0 )
			return;

		glDeleteBuffers(1,&_bufferHandle);
		glGenBuffers(1,&_bufferHandle);

		_bufferSize = size;
		DoCreateBuffer(NULL);
	}

	void GLRenderBuffer::DoCreateBuffer(void *pData)
	{
		glGenBuffers(1, &_bufferHandle);
		if( _type == RBT_Vertex )
		{
			glBindBuffer(GL_ARRAY_BUFFER,_bufferHandle);
			glBufferData(GL_ARRAY_BUFFER,_bufferSize,pData,GetGLBufferUsage(_bufferUsage));
		}
		else if( _type == RBT_Index )
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_bufferHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,_bufferSize,pData,GetGLBufferUsage(_bufferUsage));
		}
		else if( _type == RBT_Constant )
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint,_bufferHandle);
			glBufferData(GL_UNIFORM_BUFFER,_bufferSize,pData,GetGLBufferUsage(_bufferUsage));
		}

		return;
	}

	GLenum GLRenderBuffer::GetGLBufferUsage(BufferUsage bufferUsage)
	{
		if( bufferUsage == BU_StreamCopy )
			return GL_STREAM_COPY;
		else if(bufferUsage == BU_StreamRead )
			return GL_STREAM_READ;
		else if(bufferUsage == BU_StreamDraw )
			return GL_STREAM_DRAW;
		else if( bufferUsage == BU_StaticCopy )
			return GL_STATIC_COPY;
		else if( bufferUsage == BU_StaticDraw )
			return GL_STATIC_DRAW;
		else if( bufferUsage == BU_StaticRead )
			return GL_STATIC_READ;
		else if( bufferUsage == BU_DynamicCopy )
			return GL_DYNAMIC_COPY;
		else if( bufferUsage == BU_DynamicDraw )
			return GL_DYNAMIC_DRAW;
		else if( bufferUsage == BU_DynamicRead )
			return GL_DYNAMIC_READ;

		BOOST_ASSERT(0);

		return GL_STATIC_DRAW;

	}

	GLRenderBuffer::GLRenderBuffer(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint)
	{
		_type = type;
		_bufferUsage = bufferUsage;
		_elementSize = elementSize;
		_bufferSize = size;
		_bindingPoint = bindingPoint;

		DoCreateBuffer(pData);
	}


	GLRenderBuffer::~GLRenderBuffer()
	{
		glDeleteBuffers(1,&_bufferHandle);
 
	}

	////////////////////////////////////////////////////////////////////////////////////////
	GLRenderTexture2D::GLRenderTexture2D(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		InnerCreator(pixelFormat, width, height, bMipmap, bMultiSample, viewFlag, arraySize, pData, flag);
	}

	GLRenderTexture2D::GLRenderTexture2D(PixelFormat pixelFormat, bool bMultiSample,Image* image)
	{
		const ImageSpec &spec = image->GetSpec();
		BufferInitData data;
		data.Data = image->GetImageData();
		data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat)/8 * spec.width;
		data.SlicePitch = 0;

		InnerCreator(pixelFormat, spec.width, spec.height, false, bMultiSample, SV_ShaderResource,1, &data,0);
	}

	GLRenderTexture2D::GLRenderTexture2D(PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag)
	{
		if (image.size() == 0)
			return;

		const ImageSpec &spec = image[0]->GetSpec();
		BYTE* pData = new BYTE[spec.dataSize*image.size()];
		BYTE* pDest = pData;
		
		for (size_t i = 0; i < image.size(); i++)
		{
			BOOST_ASSERT(spec.dataSize == image[i]->GetSpec().dataSize);
			memcpy(pDest, image[i]->GetImageData(), spec.dataSize);
			pDest += spec.dataSize;
		}

		BufferInitData data; // only one struct
		data.Data = pData;
		data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat) / 8 * spec.width;
		data.SlicePitch = spec.dataSize;

		InnerCreator(pixelFormat, spec.width, spec.height, false, bMultiSample, SV_ShaderResource, image.size(), &data, flag);
		delete pData;
	}

	void GLRenderTexture2D::InnerCreator(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		Format = pixelFormat;
		Height = height;
		Width = width;
		ArraySize = arraySize;

		if (bMipmap)
		{ 
	        unsigned int w = Math::LogTwo(width);
			unsigned int h = Math::LogTwo(height);
			MipLevel = w > h ? h : w;
		}
		else
		    MipLevel = 1;
     
		// format 
		GLenum glFormat = 0;
		GLenum glType = 0; //
		GLenum storageFormat = GLRenderEngine::GetPixelFormat(pixelFormat, glFormat, glType);
		if (viewFlag & SV_DepthStencil)  //depth view ...
		{
			glFormat = GL_DEPTH_COMPONENT;
			if (storageFormat == GL_R32F)
				storageFormat = GL_DEPTH_COMPONENT32F;
			else if (storageFormat == GL_R16)
				storageFormat = GL_DEPTH_COMPONENT16;
		}

		if (arraySize == 6 && (flag & SF_AsCubeMap) )
		{
			_texFormat = GL_TEXTURE_CUBE_MAP;
		}
		else if (arraySize > 1)
		{
			_texFormat = GL_TEXTURE_2D_ARRAY;
			if (bMultiSample)
				_texFormat = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		}
		else if (arraySize == 1)
		{
			_texFormat = GL_TEXTURE_2D;
			if (bMultiSample)
				_texFormat = GL_TEXTURE_2D_MULTISAMPLE;
		}
		else
			BOOST_ASSERT(0);

		void *pTextureData = NULL;
		if (pData && pData->Data)
			pTextureData = (void*)pData->Data;

		// texture create.
		// for layer texture rendering
		if (_texFormat == GL_TEXTURE_2D_ARRAY)
		{
			glBindTexture(_texFormat, _texHandle);			 
			if (bMultiSample)
				glTexStorage3DMultisample(_texFormat, GConfig->pRenderConfig->MultiSampleCount, storageFormat, width, height, arraySize, false);
			else
				glTexStorage3D(_texFormat, MipLevel, storageFormat, width, height, arraySize);
			
		}
		else if (_texFormat == GL_TEXTURE_CUBE_MAP)
		{
			glBindTexture(_texFormat, _texHandle);
			 
			glTexStorage2D(_texFormat, MipLevel, storageFormat, width, height);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			if (pTextureData)
			{
				for (unsigned int i = 0; i < 6; i++)
				{
					void *offset = (BYTE*)pData->Data + pData->SlicePitch * i;
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, glFormat, glType, offset);
				}
			}
		}
		else if (_texFormat == GL_TEXTURE_2D)
		{
			glBindTexture(_texFormat, _texHandle);
			 
			if (bMultiSample)
				glTexStorage2DMultisample(_texFormat, GConfig->pRenderConfig->MultiSampleCount, storageFormat, width, height, false);
			else
				glTexStorage2D(_texFormat,MipLevel, storageFormat, width, height);

			if (pTextureData != NULL)
				glTexSubImage2D(_texFormat, 0, 0, 0, width, height, glFormat, glType, pTextureData);
		}
		else
		{
			BOOST_ASSERT(0);
		}
		
 
		if (bMipmap)
			glGenerateMipmap(_texFormat);
		
	}

	GLRenderTexture2D::GLRenderTexture2D(int arraySize)
	{
		glGenTextures(1,&_texHandle);
		
	}

	GLRenderTexture2D::~GLRenderTexture2D()
	{
		glDeleteTextures(1,&_texHandle);
	}

	void* GLRenderTexture2D::GetHandle()
	{
		return (void*)_texHandle;
	}
}