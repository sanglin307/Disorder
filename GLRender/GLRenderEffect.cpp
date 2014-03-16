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
		else
		{
			GLogger->Error("Unsupported shader type!");
			return "";
		}
	}

	GLShaderObjectPtr GLShaderObject::Create(std::string const& effectName, std::string const& entryPoint, ShaderType shaderType, ShaderModel shaderMode)
	{
		GLShaderObject *pObject = new GLShaderObject(effectName, entryPoint, shaderType, shaderMode);
		return GLShaderObjectPtr(pObject);
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
			strncpy(_cachedShaderPath[_cachedShaderPathCount], iter->first.c_str(), iter->first.size());
			_cachedShaderPath[_cachedShaderPathCount][iter->first.size()] = 0;
			_cachedShaderPathLengh[_cachedShaderPathCount] = iter->first.size();
			_cachedShaderPathCount++;
			++iter;
		}
	}

	bool GLShaderObject::LoadShaderFromFile(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType)
	{
		PrepareShaderPathInclude();
		FileObjectPtr file = GEngine->FileManager->OpenFile(fileName,"rt");
		std::string shaderContent = GEngine->FileManager->ReadFile(file);

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
				GLogger->Error(pLog);
				delete[] pLog;
			}
		}

		return true;
	}


	GLShaderObject::~GLShaderObject()
	{
		if (_GLHandle)
		{
			glDeleteShader(_GLHandle);
			_GLHandle = 0;
		}
	}

	GLProgramReflectionPtr GLProgramReflection::Create()
	{
		GLProgramReflection *Reflection = new GLProgramReflection;
		return GLProgramReflectionPtr(Reflection);
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

	void GLRenderEffect::GetShaderPropertyTypeLength(GLint type, EShaderProperty& propertyType, int &lenght)
	{
		if (type == GL_FLOAT)
		{
			propertyType = eSP_Float;
			lenght = 1;
		}
		else if (type == GL_FLOAT_VEC2)
		{
			propertyType = eSP_Float;
			lenght = 2;
		}
		else if (type == GL_FLOAT_VEC3)
		{
			propertyType = eSP_Float;
			lenght = 3;
		}
		else if (type == GL_FLOAT_VEC4)
		{
			propertyType = eSP_Float;
			lenght = 4;
		}
		else if (type == GL_DOUBLE)
		{
			propertyType = eSP_Double;
			lenght = 1;
		}
		else if (type == GL_DOUBLE_VEC2)
		{
			propertyType = eSP_Double;
			lenght = 2;
		}

		else if (type == GL_DOUBLE_VEC3)
		{
			propertyType = eSP_Double;
			lenght = 3;
		}
		else if (type == GL_DOUBLE_VEC4)
		{
			propertyType = eSP_Double;
			lenght = 4;
		}
		else if (type == GL_INT)
		{
			propertyType = eSP_Int;
			lenght = 1;
		}
		else if (type == GL_INT_VEC2)
		{
			propertyType = eSP_Int;
			lenght = 2;
		}
		else if (type == GL_INT_VEC3)
		{
			propertyType = eSP_Int;
			lenght = 3;
		}
		else if (type == GL_INT_VEC4)
		{
			propertyType = eSP_Int;
			lenght = 4;
		}
		else if (type == GL_SAMPLER_2D)
		{
			propertyType = eSP_SampleState;
			lenght = 1;
		}
		else
		{
			GLogger->Error("We don't support this shader property type now");
		}
	}

	void GLRenderEffect::BindShader(ShaderObjectPtr const& shaderObject)
	{
		if( shaderObject->GetType() == ST_VertexShader )
			_vertexShader = shaderObject;
		else if( shaderObject->GetType() == ST_PixelShader )
			_pixelShader = shaderObject;
		else
			BOOST_ASSERT(0);

		GLuint sHandle = (GLuint)shaderObject->GetLowInterface();
		glAttachShader(_GLHandle,sHandle);
	}

	void GLRenderEffect::Reflection()
	{
		EffectReflection = GLProgramReflection::Create();

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
		}
 
		// uniform block
		ShaderPropertyManagerPtr GlobalPropertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);

		GLint uniformBlockNumber;
		glGetProgramInterfaceiv(_GLHandle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlockNumber);
		if (uniformBlockNumber > 0)
		{
			for (int i = 0; i < uniformBlockNumber; i++)
			{
				GLShaderUniformBlock desc;
				char name[64];
				glGetProgramResourceName(_GLHandle, GL_UNIFORM_BLOCK, i, sizeof(name), NULL, name);

				desc.Name = name;
				desc.BlockIndex = glGetUniformBlockIndex(_GLHandle, name);
				glGetActiveUniformBlockiv(_GLHandle, desc.BlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &desc.BlockSize);
				glGetActiveUniformBlockiv(_GLHandle, desc.BlockIndex, GL_UNIFORM_BLOCK_BINDING, &desc.BlockBinding);
				GLShaderPropertyManagerPtr propertyManager = boost::dynamic_pointer_cast<GLShaderPropertyManager>(GEngine->RenderResourceMgr->GetPropertyManager(name));
				BOOST_ASSERT(propertyManager->Name != ShaderPropertyManager::sManagerGlobal);  // don't registe uniform block name ,please registe it in ShaderPropertyManager

				desc.BufferParamRef = GlobalPropertyManager->GetProperty(name);
				if (desc.BufferParamRef == NULL)
				{
					RenderBufferPtr constBuffer = GEngine->RenderResourceMgr->CreateRenderBuffer(RBT_Constant, BAH_GPU_Read, desc.BlockSize, desc.BlockSize, NULL);
					desc.BufferParamRef = GlobalPropertyManager->CreateProperty(name, eSP_ConstBuffer);
					desc.BufferParamRef->SetData(constBuffer);
				}

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
			const GLenum props[] = { GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX, GL_OFFSET };
		
			for (int i = 0; i < uniformNumber; i++)
			{
				GLShaderResourceBinding desc;
				char name[128];
				glGetProgramResourceName(_GLHandle, GL_UNIFORM, i, sizeof(name), NULL, name);

				GLint params[4];
				glGetProgramResourceiv(_GLHandle, GL_UNIFORM, i, 4, props, 4, NULL, params);
				desc.Name = name;
				desc.Type = params[0];
				desc.Location = params[1];
				desc.BlockIndex = params[2];
				desc.Offset = params[3];

				int pos = desc.Name.find_first_of('.');
				std::string blockName = "";
				std::string varName = name;
				ShaderPropertyManagerPtr manager = NULL; 
				if (pos != std::string::npos)
				{
					blockName = desc.Name.substr(0, pos);
					varName = desc.Name.substr(pos+1);
					manager = GEngine->RenderResourceMgr->GetPropertyManager(blockName);
				}
				else
					manager = GlobalPropertyManager;

				EShaderProperty shaderProperty;
				int length;
				GLRenderEffect::GetShaderPropertyTypeLength(desc.Type, shaderProperty, length);

				desc.ParamRef = manager->CreateProperty(varName, shaderProperty, length);
				if (desc.Location < 0 && blockName != "") // ? block
				{
					bool found = false;
					for (size_t j = 0; j < EffectReflection->UniformBlockArray.size(); j++)
					{
						if (EffectReflection->UniformBlockArray[j].Name == blockName)
						{
							found = true;
							EffectReflection->UniformBlockArray[j].Members.push_back(desc);
							EffectReflection->UniformBlockArray[j].MembersRef.push_back(desc.ParamRef);
						}
					}
					if (!found)
					{
						BOOST_ASSERT(0);
					}
				}
 
				EffectReflection->ResourceArray.push_back(desc);
			}
		}

		
	}

	GLRenderEffectPtr GLRenderEffect::Create()
	{
		GLRenderEffect *pEffect = new GLRenderEffect;
		return GLRenderEffectPtr(pEffect);
	}

	GLRenderEffect::GLRenderEffect()
		:EffectReflection(NULL)
	{
		_GLHandle = glCreateProgram();
	}

	GLRenderEffect::~GLRenderEffect()
	{
		glDeleteProgram(_GLHandle);
	}

	GLShaderPropertyManagerPtr GLShaderPropertyManager::Create(const std::string& name)
	{
		GLShaderPropertyManager *pManager = new GLShaderPropertyManager(name);
		return GLShaderPropertyManagerPtr(pManager);
	}

	GLShaderPropertyManager::GLShaderPropertyManager(const std::string& name)
		:ShaderPropertyManager(name)
	{
	}

	void GLShaderPropertyManager::UpdateShaderProperty()
	{
	}

	GLShaderPropertyManager::~GLShaderPropertyManager()
	{
	}


}