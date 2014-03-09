#ifndef _DISORDER_GLRENDEREFFECT_H_
#define _DISORDER_GLRENDEREFFECT_H_


namespace Disorder
{
	class GLShaderObject : public ShaderObject
	{

	public:
		~GLShaderObject();

		virtual void  UpdateShaderParameter() { ; }
		virtual void* GetLowInterface(){ return 0; }
		virtual void* GetDataInterface() { return 0; }

		virtual bool LoadShaderFromFile(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType);
		virtual void ShaderReflection(){};

		static std::string GetShaderFileSuffix(ShaderType type);

		static GLShaderObjectPtr Create(std::string const& effectName, std::string const& entryPoint, ShaderType shaderType, ShaderModel shaderMode);

	private:
		explicit GLShaderObject(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType, ShaderModel shaderModel)
		{
			_GLHandle = 0;
			_fileName = fileName;
			_entryPoint = entryPoint;
			_type = shaderType;
			_shaderModel = shaderModel;
		}

		GLuint _GLHandle;

	};
}


#endif