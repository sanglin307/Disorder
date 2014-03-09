#include "GLRenderInclude.h"

namespace Disorder
{
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

	bool GLShaderObject::LoadShaderFromFile(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType)
	{
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












}