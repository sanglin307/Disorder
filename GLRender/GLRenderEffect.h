#ifndef _DISORDER_GLRENDEREFFECT_H_
#define _DISORDER_GLRENDEREFFECT_H_


namespace Disorder
{
	class GLShaderObject : public ShaderObject
	{

	public:
		~GLShaderObject();

		virtual void  UpdateShaderParameter() { ; }
		virtual void* GetLowInterface(){ return (void*)_GLHandle; }
		virtual void* GetDataInterface() { return 0; }

		virtual bool LoadShaderFromFile(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType);
		virtual void ShaderReflection(){};

		static std::string GetShaderFileSuffix(ShaderType type);
		static int GetOpenGLShaderType(ShaderType sType);
		static void PrepareShaderPathInclude();

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

		static GLchar* _cachedShaderPath[64];  
		static int _cachedShaderPathLengh[64];
		static int _cachedShaderPathCount;

	};

	struct GLShaderSignatureDesc
	{
		std::string Name;
		GLint Type;
		GLint Location;
	};

	struct GLShaderResourceBinding
	{
		std::string Name;
		GLint Type;
		GLint Location;
		GLint BlockIndex;
		GLint Offset;
		ShaderPropertyPtr ParamRef;
	};

	struct GLShaderUniformBlock
	{
		std::string         Name;
		GLint               BlockIndex;
		GLint               BlockBinding;
		GLint               BlockSize;
		ShaderPropertyPtr	BufferParamRef;
		std::vector<GLShaderResourceBinding> Members;
		std::vector<ShaderPropertyPtr> MembersRef;
	};

	class GLProgramReflection
	{
	public:
		static GLProgramReflectionPtr Create();
		std::vector<GLShaderSignatureDesc> InputArray;
		std::vector<GLShaderUniformBlock> UniformBlockArray;
		std::vector<GLShaderResourceBinding> ResourceArray;

	private:
		GLProgramReflection(){};
	};

	

	class GLRenderEffect : public RenderEffect
	{
	public:
		~GLRenderEffect();

		virtual void BindShader(ShaderObjectPtr const& shaderObject);
		virtual void LinkShaders();
		 
		static GLRenderEffectPtr Create();
		GLProgramReflectionPtr EffectReflection;

		static void GetShaderPropertyTypeLength(GLint type, EShaderProperty& propertyType, int &lenght);
		static void GetGLShaderPropertyTypeLength(GLint type, GLenum& propertyType, int &lenght);

	private:
		GLRenderEffect();
		void Reflection();

		GLuint _GLHandle;
	};

	class GLShaderPropertyManager : public ShaderPropertyManager
	{
	public:
		~GLShaderPropertyManager();
		virtual void UpdateShaderProperty();
	 
		static GLShaderPropertyManagerPtr Create(const std::string& name);
 
	protected:
		GLShaderPropertyManager(const std::string& name);
		

		
	};
}


#endif