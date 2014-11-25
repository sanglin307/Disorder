#ifndef _DISORDER_GLRENDEREFFECT_H_
#define _DISORDER_GLRENDEREFFECT_H_


namespace Disorder
{
	class GLShaderObject : public ShaderObject
	{

	public:
		~GLShaderObject();

		virtual void  UpdateShaderParameter() { ; }
		virtual void* GetHandle(){ return (void*)_GLHandle; }
		virtual void* GetDataInterface() { return 0; }

		virtual bool LoadShaderFromFile(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType);
		virtual void ShaderReflection(){};

		static std::string GetShaderFileSuffix(ShaderType type);
		static int GetOpenGLShaderType(ShaderType sType);
		static void PrepareShaderPathInclude();

		explicit GLShaderObject(std::string const& fileName, std::string const& entryPoint, ShaderType shaderType, ShaderModel shaderModel)
		{
			_GLHandle = 0;
			_fileName = fileName;
			_entryPoint = entryPoint;
			_type = shaderType;
			_shaderModel = shaderModel;
		}
	private:
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
		GLint Size;
		ShaderProperty* ParamRef;
	};

	struct GLShaderUniformBlock
	{
		std::string         Name;
		GLint               BlockIndex;
		GLint               BlockBinding;
		GLint               BlockSize;
		ShaderProperty*	    BufferParamRef;
		std::vector<GLShaderResourceBinding> Members;
		std::vector<ShaderProperty*> MembersRef;
	};

	class GLProgramReflection
	{
	public:
		std::vector<GLShaderSignatureDesc> InputArray;
		std::vector<GLShaderUniformBlock> UniformBlockArray;
		std::vector<GLShaderResourceBinding> ResourceArray;
 
	};

	

	class GLRenderEffect : public RenderEffect
	{
	public:
		GLRenderEffect();
		~GLRenderEffect();

		virtual void BindShader(ShaderObject* shaderObject);
		virtual void LinkShaders();
		 
		virtual void* GetHandle(){ return (void*)_GLHandle; }

		GLProgramReflection* EffectReflection;

		static void GetShaderPropertyTypeLength(GLint type, EShaderProperty& propertyType, int &lenght,  int &size);
		static void GetGLShaderPropertyTypeLength(GLint type, GLenum& propertyType, int &lenght);

	private:
		void Reflection();

		GLuint _GLHandle;
	};

	class GLShaderPropertyManager : public ShaderPropertyManager
	{
	public:
		~GLShaderPropertyManager();
		virtual void UpdateShaderProperty();
	  
		bool Validate(GLShaderUniformBlock *pUniformBlock);
		GLShaderPropertyManager(const std::string& name);

		GLint BindingPoint;

	protected:
		
		
		GLShaderUniformBlock* _uniformBlock;

		
	};
}


#endif