#include "GLRenderInclude.h"

namespace Disorder
{
	GLint GLRenderLayout::Location_Position = 0;
	GLint GLRenderLayout::Location_Color = 1;
	GLint GLRenderLayout::Location_Normal = 2;
	GLint GLRenderLayout::Location_Tex0 = 3;

	void * GLRenderLayout::GetHandle()
	{
		return (void*)_VAOHandle;
	}

	GLRenderLayout::~GLRenderLayout()
	{ 
	    glDeleteVertexArrays(1,&_VAOHandle);		 
	}

	GLint GLRenderLayout::GetGLTypeSize(GLint type)
	{
		switch (type)
		{
		case GL_FLOAT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			return 4;
		case GL_FLOAT_VEC2:
		case GL_INT_VEC2:
		case GL_UNSIGNED_INT_VEC2:
			return 8;
		case GL_FLOAT_VEC3:
		case GL_INT_VEC3:
		case GL_UNSIGNED_INT_VEC3:
			return 12;
		case GL_FLOAT_VEC4:
		case GL_INT_VEC4:
		case GL_UNSIGNED_INT_VEC4:
			return 16;
		case GL_DOUBLE:
			return 8;
		case GL_DOUBLE_VEC2:
			return 16;
		case GL_DOUBLE_VEC3:
			return 24;
		case GL_DOUBLE_VEC4:
			return 32;
		case GL_FLOAT_MAT2:
			return 4*4;
		case GL_FLOAT_MAT3:
			return 9*4;
		case GL_FLOAT_MAT4:
			return 16*4;
		default:
			BOOST_ASSERT(0);
			break;
		}

		return 0;
	}

	void GLRenderLayout::FinishBufferBinding(RenderEffectPtr const& renderEffect)
	{
		GLRenderEffectPtr glEffect = boost::dynamic_pointer_cast<GLRenderEffect>(renderEffect);
		BOOST_ASSERT(glEffect->EffectReflection->InputArray.size() > 0 );
		BOOST_ASSERT(_VAOHandle > 0 );
 
		glBindVertexArray(_VAOHandle);
		GLuint offset = 0;
		GLuint stride = 0;
		bool soloBuffer = _vertexBuffers.size() > 1 ? false : true;

		if (soloBuffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, (GLuint)_vertexBuffers[0]->GetHandle());
			stride = _vertexBuffers[0]->GetElementSize();
		}

		for(size_t attriIndex =0; attriIndex < glEffect->EffectReflection->InputArray.size(); attriIndex++ )
		{
			GLShaderSignatureDesc *pDesc = &(glEffect->EffectReflection->InputArray[attriIndex]);
			GLenum propertyType;
			int count = 0;
			int normalize = 0;
			GLRenderEffect::GetGLShaderPropertyTypeLength(pDesc->Type, propertyType, count);

			glEnableVertexAttribArray(pDesc->Location);

			if( !soloBuffer )
			{
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)_vertexBuffers[attriIndex]->GetHandle());
				glVertexAttribPointer(pDesc->Location, count, propertyType, GL_FALSE, _vertexBuffers[attriIndex]->GetElementSize(), 0);
			}
			else
			{
				glVertexAttribPointer(pDesc->Location, count, propertyType, GL_FALSE, stride, (GLvoid*)offset);
				offset += GLRenderLayout::GetGLTypeSize(pDesc->Type);
			}

			
		}

		if(_indexBuffer!= NULL )
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,(GLuint)_indexBuffer->GetHandle());
		}

		glBindVertexArray(0);
	}

	GLRenderLayoutPtr GLRenderLayout::Create(RenderEffectPtr const& renderEffect,TopologyType topologyType,bool soloBuffer)
	{
		GLRenderLayout *pLayout = new GLRenderLayout;
		pLayout->_topologyType = topologyType;
		glGenVertexArrays(1, &(pLayout->_VAOHandle)); 
		return GLRenderLayoutPtr(pLayout);
	}
}