#include "GLRenderInclude.h"

namespace Disorder
{
	GLRenderBufferPtr GLRenderBuffer::Create(RenderBufferType type,GeometryPtr const& data,GLint location,BufferUsage bufferUsage,RenderEffectPtr const& effect)
	{
		GLRenderEffectPtr glEffect = boost::dynamic_pointer_cast<GLRenderEffect>(effect);
		
		int elementSize = 0;
		int bufferSize = 0;

		void *pData = NULL;
		std::vector<float> vData;
		GLRenderBuffer *pBuffer = new GLRenderBuffer;
		pBuffer->_type = type;
		pBuffer->_bufferUsage = bufferUsage;
		pBuffer->_elementSize = elementSize;

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
				delete pBuffer;
				return NULL;
			}
 
			if( location == GLRenderLayout::Location_Position )
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
			else if(location == GLRenderLayout::Location_Color )
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
			else if(location == GLRenderLayout::Location_Normal )
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
			else if(location == GLRenderLayout::Location_Tex0 )
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
		else if( type == RBT_Index )
		{
			if( data->Indices.size() > 0 )
			{
				pBuffer->_elementSize = sizeof(unsigned int);
				pBuffer->_bufferSize = sizeof(unsigned int) * data->Indices.size();
				pData = data->Indices.data();
			}
			else
			{
				BOOST_ASSERT(0);
				delete pBuffer;
				return NULL;
			}
		}
		else
		{
			BOOST_ASSERT(0);
			delete pBuffer;
			return NULL;
		}

		pBuffer->DoCreateBuffer(pData);

		return GLRenderBufferPtr(pBuffer);
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
			glBindBuffer(GL_UNIFORM_BUFFER,_bufferHandle);
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

	GLRenderBufferPtr GLRenderBuffer::Create(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData)
	{
		GLRenderBuffer *pBuffer = new GLRenderBuffer;
		pBuffer->_type = type;
		pBuffer->_bufferUsage = bufferUsage;
		pBuffer->_elementSize = elementSize;
		pBuffer->_bufferSize = size;

		pBuffer->DoCreateBuffer(pData);
		 

		return GLRenderBufferPtr(pBuffer);
	}

	GLRenderBuffer::GLRenderBuffer()
	{
		glGenBuffers(1,&_bufferHandle);
	}

	GLRenderBuffer::~GLRenderBuffer()
	{
		glDeleteBuffers(1,&_bufferHandle);
 
	}

	////////////////////////////////////////////////////////////////////////////////////////

	GLRenderTexture2DPtr GLRenderTexture2D::Create(PixelFormat pixelFormat, bool bMultiSample,ImagePtr const& image)
	{
		const ImageSpec &spec = image->GetSpec();
		BufferInitData data;
		data.Data = image->GetImageData();
		data.RowPitch = RenderEngine::ComputePixelSizeBits(pixelFormat)/8 * spec.width;
		data.SlicePitch = 0;
		std::vector<ESurfaceLocation> loc;
		return Create(pixelFormat,spec.width,spec.height,false,bMultiSample,&data);
	}

	GLRenderTexture2DPtr GLRenderTexture2D::Create(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, BufferInitData const* pData)
	{
		GLRenderTexture2D *pTexture = new GLRenderTexture2D;

		pTexture->Format = pixelFormat;
		pTexture->Height = height;
		pTexture->Width = width;
	
		glBindTexture(GL_TEXTURE_2D, pTexture->_texHandle);

		if (bMipmap)
		{
	 
	        unsigned int w = Math::LogTwo(width);
			unsigned int h = Math::LogTwo(height);

			pTexture->MipLevel = w > h ? h : w;
		}
		else
			pTexture->MipLevel = 1;
     
		GLenum glFormat = 0;
		GLenum glType = 0;
        glTexStorage2D(GL_TEXTURE_2D, pTexture->MipLevel,GLRenderEngine::GetPixelFormat(pixelFormat,glFormat,glType), width, height);  

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, glFormat, glType, pData->Data);
 
		return GLRenderTexture2DPtr(pTexture);
	}

	GLRenderTexture2D::GLRenderTexture2D()
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