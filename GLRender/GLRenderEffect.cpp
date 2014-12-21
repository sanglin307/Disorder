#include "GLRenderInclude.h"

namespace Disorder
{
	GLchar* GLShaderObject::_cachedShaderPath[64];  // suppose 64 length
	int GLShaderObject::_cachedShaderPathLengh[64];
	int GLShaderObject::_cachedShaderPathCount = 0;

	std::string GLShaderObject::GetShaderFileSuffix(ShaderType type)
	{
		if (type == ST_VertexShader)
		{
			return ".glvs";
		}
		else if (type == ST_PixelShader)
		{
			return ".glfs";
		}
		else if (type == ST_GeometryShader)
		{
			return ".glgs";
		}
		else
		{
			GLogger->Error("Unsupported shader type!");
			BOOST_ASSERT(0);
			return "";
		}
	}


	int GLShaderObject::GetOpenGLShaderType(ShaderType sType)
	{
		switch (sType)
		{
		case ST_VertexShader:
			return GL_VERTEX_SHADER;
		case ST_PixelShader:
			return GL_FRAGMENT_SHADER;
		case ST_GeometryShader:
			return GL_GEOMETRY_SHADER;
		case ST_ComputeShader:
			return GL_COMPUTE_SHADER;
		case ST_HullShader:
			return GL_TESS_CONTROL_SHADER;
		case ST_DomainShader:
			return GL_TESS_EVALUATION_SHADER;
		default:
			GLogger->Error("Unsupported shader Type for OpenGL!");
			break;
		}

		return 0;
	}

	void GLShaderObject::PrepareShaderPathInclude()
	{
		if (_cachedShaderPathCount == ShaderObject::sMapIncludeFiles.size())
			return;

		_cachedShaderPathCount = 0;
		std::map<std::string, std::string>::const_iterator iter = ShaderObject::sMapIncludeFiles.begin();
		while (iter != ShaderObject::sMapIncludeFiles.end())
		{
			if (_cachedShaderPath[_cachedShaderPathCount] != 0)
				delete[] _cachedShaderPath[_cachedShaderPathCount];
			_cachedShaderPath[_cachedShaderPathCount] = new char[iter->first.size() + 1];
			strncpy_s(_cachedShaderPath[_cachedShaderPathCount], iter->first.size() + 1, iter->first.c_str(), iter->first.size());
			_cachedShaderPath[_cachedShaderPathCount][iter->first.size()] = 0;
			_cachedShaderPathLengh[_cachedShaderPathCount] = iter->first.size();
			_cachedShaderPathCount++;
			++iter;
		}
	}

	bool GLShaderObject::LoadShaderFromFile(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType)
	{
		PrepareShaderPathInclude();

		FILE* fileHandle;
		if (fopen_s(&fileHandle, fileName.c_str(), "rt"))
		{
			BOOST_ASSERT(0);
			return false;
		}

		std::string shaderContent;
		char buff[65535];
		while (fgets(buff, 65535, fileHandle) != NULL)
		{
			shaderContent += buff;
		}
		fclose(fileHandle);

		_GLHandle = glCreateShader(GLShaderObject::GetOpenGLShaderType(shaderType));
		const char* pSource = shaderContent.c_str();
		glShaderSource(_GLHandle,1,&pSource,NULL);
		glCompileShaderIncludeARB(_GLHandle, _cachedShaderPathCount, _cachedShaderPath, _cachedShaderPathLengh);
		int result = 0;
		glGetShaderiv(_GLHandle,GL_COMPILE_STATUS,&result);
		if( result == GL_FALSE )
		{
			int logLength;
			glGetShaderiv(_GLHandle,GL_INFO_LOG_LENGTH,&logLength);
			if( logLength > 0 )
			{
				char *pLog = new char[logLength];
				glGetShaderInfoLog(_GLHandle,logLength,NULL,pLog);
				std::string strLog = "Shader Compile error: ";
				strLog += fileName;
				strLog += " -- ";
				strLog += pLog;
				GLogger->Error(strLog);
				delete[] pLog;
				BOOST_ASSERT(0);
				return false;
			}
		}

		return true;
	}


	GLShaderObject::~GLShaderObject()
	{ 
		glDeleteShader(_GLHandle);		 
	}

	 
	void GLRenderEffect::LinkShaders()
	{
		glLinkProgram(_GLHandle);
		int result;
		glGetProgramiv(_GLHandle, GL_LINK_STATUS,&result);
		if( result == GL_TRUE )
		    Reflection();
		else
		{
			int logLength;
			glGetProgramiv(_GLHandle,GL_INFO_LOG_LENGTH,&logLength);
			if( logLength > 0 )
			{
				char *pLog = new char[logLength];
				glGetProgramInfoLog(_GLHandle,logLength,NULL,pLog);
				GLogger->Error(pLog);
				delete[] pLog;
			}
			
		}
	}

	void GLRenderEffect::GetGLShaderPropertyTypeLength(GLint type, GLenum& propertyType, int &lenght)
	{
		if (type == GL_FLOAT)
		{
			propertyType = GL_FLOAT;
			lenght = 1;
		}
		else if (type == GL_FLOAT_VEC2)
		{
			propertyType = GL_FLOAT;
			lenght = 2;
		}
		else if (type == GL_FLOAT_VEC3)
		{
			propertyType = GL_FLOAT;
			lenght = 3;
		}
		else if (type == GL_FLOAT_VEC4)
		{
			propertyType = GL_FLOAT;
			lenght = 4;
		}
		else if (type == GL_DOUBLE)
		{
			propertyType = GL_DOUBLE;
			lenght = 1;
		}
		else if (type == GL_DOUBLE_VEC2)
		{
			propertyType = GL_DOUBLE;
			lenght = 2;
		}

		else if (type == GL_DOUBLE_VEC3)
		{
			propertyType = GL_DOUBLE;
			lenght = 3;
		}
		else if (type == GL_DOUBLE_VEC4)
		{
			propertyType = GL_DOUBLE;
			lenght = 4;
		}
		else if (type == GL_INT)
		{
			propertyType = GL_INT;
			lenght = 1;
		}
		else if (type == GL_INT_VEC2)
		{
			propertyType = GL_INT;
			lenght = 2;
		}
		else if (type == GL_INT_VEC3)
		{
			propertyType = GL_INT;
			lenght = 3;
		}
		else if (type == GL_INT_VEC4)
		{
			propertyType = GL_INT;
			lenght = 4;
		}
		else if (GLRenderEngine::IsTextureSamplerResouce(type))
		{
			propertyType = type;
			lenght = 1;
		}
		else
		{
			GLogger->Error("We don't support this shader property type now");
			BOOST_ASSERT(0);
		}
	}

	void GLRenderEffect::GetShaderPropertyTypeLength(GLint type, EShaderProperty& propertyType, int &length,int &size)
	{
		if (type == GL_FLOAT)
		{
			propertyType = eSP_Float;
			length = 1;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_FLOAT_VEC2)
		{
			propertyType = eSP_Float;
			length = 2;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_FLOAT_VEC3)
		{
			propertyType = eSP_Float;
			length = 3;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_FLOAT_VEC4)
		{
			propertyType = eSP_Float;
			length = 4;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_FLOAT_MAT4)
		{
			propertyType = eSP_Float;
			length = 16;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_FLOAT_MAT3)
		{
			propertyType = eSP_Float;
			length = 9;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_FLOAT_MAT2)
		{
			propertyType = eSP_Float;
			length = 4;
			size = sizeof(GLfloat)* length;
		}
		else if (type == GL_DOUBLE)
		{
			propertyType = eSP_Double;
			length = 1;
			size = sizeof(GLdouble)* length;
		}
		else if (type == GL_DOUBLE_VEC2)
		{
			propertyType = eSP_Double;
			length = 2;
			size = sizeof(GLdouble)* length;
		}

		else if (type == GL_DOUBLE_VEC3)
		{
			propertyType = eSP_Double;
			length = 3;
			size = sizeof(GLdouble)* length;
		}
		else if (type == GL_DOUBLE_VEC4)
		{
			propertyType = eSP_Double;
			length = 4;
			size = sizeof(GLdouble)* length;
		}
		else if (type == GL_DOUBLE_MAT2)
		{
			propertyType = eSP_Double;
			length = 4;
			size = sizeof(GLdouble)* length;
		}
		else if (type == GL_DOUBLE_MAT3)
		{
			propertyType = eSP_Double;
			length = 9;
			size = sizeof(GLdouble)* length;
		}
		else if (type == GL_DOUBLE_MAT4)
		{
			propertyType = eSP_Double;
			length = 16;
			size = sizeof(GLdouble)* length;
		}
		else if (type == GL_INT)
		{
			propertyType = eSP_Int;
			length = 1;
			size = sizeof(GLint)* length;
		}
		else if (type == GL_INT_VEC2)
		{
			propertyType = eSP_Int;
			length = 2;
			size = sizeof(GLint)* length;
		}
		else if (type == GL_INT_VEC3)
		{
			propertyType = eSP_Int;
			length = 3;
			size = sizeof(GLint)* length;
		}
		else if (type == GL_INT_VEC4)
		{
			propertyType = eSP_Int;
			length = 4;
			size = sizeof(GLint)* length;
		}
		else if (type == GL_BOOL)
		{
			propertyType = eSP_Bool;
			length = 1;
			size = sizeof(GLboolean)* length;
		}
		else if (type == GL_BOOL_VEC2)
		{
			propertyType = eSP_Bool;
			length = 2;
			size = sizeof(GLboolean)* length;
		}
		else if (type == GL_BOOL_VEC3)
		{
			propertyType = eSP_Bool;
			length = 3;
			size = sizeof(GLboolean)* length;
		}
		else if (type == GL_BOOL_VEC4)
		{
			propertyType = eSP_Bool;
			length = 4;
			size = sizeof(GLboolean)* length;
		}
		else if (GLRenderEngine::IsTextureSamplerResouce(type))
		{
			propertyType = eSP_ShaderResource;
			length = 1;
			size = length;
		}		
		else
		{
			GLogger->Error("We don't support this shader property type now");
			BOOST_ASSERT(0);
		}
	}

	void GLRenderEffect::BindShader(ShaderObject* shaderObject)
	{
		if( shaderObject->GetType() == ST_VertexShader )
			_vertexShader = shaderObject;
		else if (shaderObject->GetType() == ST_PixelShader)
			_pixelShader = shaderObject;
		else if (shaderObject->GetType() == ST_GeometryShader)
			_geometryShader = shaderObject;
		else
			BOOST_ASSERT(0);

		GLuint sHandle = (GLuint)shaderObject->GetHandle();
		glAttachShader(_GLHandle,sHandle);
	}

	void GLRenderEffect::Reflection()
	{
		EffectReflection = new GLProgramReflection;

		glUseProgram(_GLHandle);
		// input
		GLint inputNumber;
		glGetProgramInterfaceiv(_GLHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputNumber);
		if( inputNumber > 0 )
		{
			const GLenum props[] = { GL_TYPE, GL_LOCATION };
		
			for (int i = 0; i < inputNumber; i++)
			{
				GLShaderSignatureDesc inputDesc;
				char name[64];
				glGetProgramResourceName(_GLHandle, GL_PROGRAM_INPUT, i, sizeof(name), NULL, name);

				GLint params[2];
				glGetProgramResourceiv(_GLHandle, GL_PROGRAM_INPUT, i, 2, props, 2, NULL, params);
				inputDesc.Name = name;
				inputDesc.Type = params[0];
				inputDesc.Location = params[1];
				EffectReflection->InputArray.push_back(inputDesc);
			}

			class InputArraySort
			{
			public:
				bool operator () (GLShaderSignatureDesc& a, GLShaderSignatureDesc& b) const
				{
					return a.Location < b.Location;
				};
			};

			std::sort(EffectReflection->InputArray.begin(), EffectReflection->InputArray.end(), InputArraySort());
		}
 
		// uniform block
		ShaderPropertyManager* GlobalPropertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);

		GLint uniformBlockNumber;
		glGetProgramInterfaceiv(_GLHandle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlockNumber);
		if (uniformBlockNumber > 0)
		{
			for (int i = 0; i < uniformBlockNumber; i++)
			{
				GLShaderUniformBlock desc;
				char name[128];
				glGetProgramResourceName(_GLHandle, GL_UNIFORM_BLOCK, i, sizeof(name), NULL, name);

				desc.Name = name;
				desc.BlockIndex = glGetUniformBlockIndex(_GLHandle, name);
				glGetActiveUniformBlockiv(_GLHandle, desc.BlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &desc.BlockSize);
				//glGetActiveUniformBlockiv(_GLHandle, desc.BlockIndex, GL_UNIFORM_BLOCK_BINDING, &desc.BlockBinding);
				GLShaderPropertyManager* propertyManager = (GLShaderPropertyManager*)GRenderResourceMgr->GetPropertyManager(name);
				BOOST_ASSERT(propertyManager->Name != ShaderPropertyManager::sManagerGlobal);  // don't registe uniform block name ,please registe it in ShaderPropertyManager

				desc.BufferParamRef = GlobalPropertyManager->GetProperty(name);
				desc.BlockBinding = propertyManager->BindingPoint;
				if (desc.BufferParamRef == NULL)
				{
					RenderBuffer* constBuffer = GRenderResourceMgr->CreateBuffer(name,RBT_Constant, BU_DynamicDraw, desc.BlockSize, desc.BlockSize, NULL, desc.BlockBinding);
					desc.BufferParamRef = GlobalPropertyManager->CreateProperty(name, eSP_ConstBuffer);
					desc.BufferParamRef->SetData(constBuffer);					
				}
			 
				// binging block to GLSL
				glUniformBlockBinding(_GLHandle, desc.BlockIndex, desc.BlockBinding);

				EffectReflection->UniformBlockArray.push_back(desc);
			}
		}

		//uniform var
		GLint uniformNumber;
		glGetProgramInterfaceiv(_GLHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformNumber);
		if( uniformNumber > 0 )
		{
			// for block var : location invalid
			// for golbal var : block_index,offset invalid
			const GLenum props[] = { GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX, GL_OFFSET, GL_ARRAY_SIZE };
		
			for (int i = 0; i < uniformNumber; i++)
			{
				GLShaderResourceBinding desc;
				char name[128];
				glGetProgramResourceName(_GLHandle, GL_UNIFORM, i, sizeof(name), NULL, name);

				GLint params[5];
				memset(params, 0, sizeof(params));
				glGetProgramResourceiv(_GLHandle, GL_UNIFORM, i, 5, props, 5, NULL, params);
				int arraySize = params[4];
				std::string pureName = name;
				if (arraySize > 0)
				{
					int pos = pureName.find('[');
					if (pos != std::string::npos)
					{
						pureName = pureName.substr(0, pos);
					}
				}
				desc.Name = pureName;
				desc.Type = params[0];
				desc.Location = params[1];
				desc.BlockIndex = params[2];
				desc.Offset = params[3];

				

				GLShaderUniformBlock *pBlock = NULL;
				ShaderPropertyManager* manager = NULL;
				if (desc.BlockIndex >= 0)
				{
					for (size_t k = 0; k < EffectReflection->UniformBlockArray.size(); k++)
					{
						if (EffectReflection->UniformBlockArray[k].BlockIndex == desc.BlockIndex)
						{
							pBlock = &(EffectReflection->UniformBlockArray[k]);
							break;
						}
					}

					BOOST_ASSERT(pBlock);
					manager = GRenderResourceMgr->GetPropertyManager(pBlock->Name);
				}
				else
					manager = GlobalPropertyManager;

				EShaderProperty shaderProperty;
				int length;
				GLRenderEffect::GetShaderPropertyTypeLength(desc.Type, shaderProperty, length,desc.Size);

				if (arraySize > 0)
				{
					length *= arraySize;
					desc.Size *= arraySize;
				}

				desc.ParamRef = manager->CreateProperty(desc.Name, shaderProperty, length);

				if (pBlock)
				{
				   pBlock->Members.push_back(desc);
				   pBlock->MembersRef.push_back(desc.ParamRef);
			    }
 
				EffectReflection->ResourceArray.push_back(desc);
			}

			class ResourceSorter
			{
			public:
				bool operator () (GLShaderResourceBinding& a, GLShaderResourceBinding& b) const
				{
					return a.Location < b.Location;
				};
			};

			std::sort(EffectReflection->ResourceArray.begin(), EffectReflection->ResourceArray.end(), ResourceSorter());

			//bind texture
			int bindTexUnit = 0;
			for (size_t i = 0; i < EffectReflection->ResourceArray.size(); i++)
			{
				if (GLRenderEngine::IsTextureSamplerResouce(EffectReflection->ResourceArray[i].Type))
				{
					glUniform1i(EffectReflection->ResourceArray[i].Location, bindTexUnit);
					bindTexUnit++;
				}
			}

		}

		for (size_t i = 0; i < EffectReflection->UniformBlockArray.size(); i++)
		{
			ShaderPropertyManager* mgr = GRenderResourceMgr->GetPropertyManager(EffectReflection->UniformBlockArray[i].Name);
			GLShaderPropertyManager* glMgr = (GLShaderPropertyManager*)(mgr);
			BOOST_ASSERT(glMgr->Validate(&(EffectReflection->UniformBlockArray[i])));
		}

		glUseProgram(0);
	}
 

	GLRenderEffect::GLRenderEffect()
		:EffectReflection(NULL)
	{
		_GLHandle = glCreateProgram();
		//glProgramParameteri(_GLHandle, GL_PROGRAM_SEPARABLE, GL_TRUE);
	}

	GLRenderEffect::~GLRenderEffect()
	{
		glDeleteProgram(_GLHandle);
	}
 
	GLShaderPropertyManager::GLShaderPropertyManager(const std::string& name)
		:ShaderPropertyManager(name)
	{
		_uniformBlock = NULL;
	}

 
	void GLShaderPropertyManager::UpdateShaderProperty()
	{
		if (!_uniformBlock)
			return;

		RenderBuffer* renderbuffer = _uniformBlock->BufferParamRef->GetDataAsConstBuffer();
		BYTE *constBuffer = (BYTE*)GRenderEngine->Map(renderbuffer, BA_Write_Only);
		memset(constBuffer, 0, _uniformBlock->BlockSize);
		BYTE* pDest = constBuffer;
		for (unsigned int j = 0; j<_uniformBlock->Members.size(); j++)
		{
			GLShaderResourceBinding* vaDesc = &(_uniformBlock->Members[j]);
			pDest = constBuffer + vaDesc->Offset;
			void *pSrc = vaDesc->ParamRef->GetData();
			memcpy(pDest, pSrc, vaDesc->Size);
		}

		GRenderEngine->UnMap(renderbuffer);

	}

	GLShaderPropertyManager::~GLShaderPropertyManager()
	{
		if (_uniformBlock)
			delete _uniformBlock;

		_uniformBlock = NULL;
	}


	bool GLShaderPropertyManager::Validate(GLShaderUniformBlock *pUniformBlock)
	{
		if (_uniformBlock == NULL)
		{
			_uniformBlock = new GLShaderUniformBlock;
			_uniformBlock->BlockBinding = pUniformBlock->BlockBinding;
			_uniformBlock->BlockIndex = pUniformBlock->BlockIndex;
			_uniformBlock->BlockSize = pUniformBlock->BlockSize;
			_uniformBlock->BufferParamRef = pUniformBlock->BufferParamRef;
			std::copy(pUniformBlock->Members.begin(), pUniformBlock->Members.end(), std::back_inserter(_uniformBlock->Members));
			std::copy(pUniformBlock->MembersRef.begin(), pUniformBlock->MembersRef.end(), std::back_inserter(_uniformBlock->MembersRef));
			 
			_uniformBlock->Name = pUniformBlock->Name;
			return true;
		}
		else
		{
			if (_uniformBlock->Name != pUniformBlock->Name)
				return false;

			if (_uniformBlock->BlockIndex != pUniformBlock->BlockIndex)
				return false;

			if (_uniformBlock->BlockSize != pUniformBlock->BlockSize)
				return false;

			if (_uniformBlock->Members.size() != pUniformBlock->Members.size())
				return false;

			return true;
		}

		return false;
	}
}