#ifndef _DISORDER_DX11RENDERTECHNIQUE_H_
#define _DISORDER_DX11RENDERTECHNIQUE_H_


namespace Disorder
{
	struct ShaderVariableDesc
	{
		ShaderVariableDesc( D3D11_SHADER_VARIABLE_DESC const& desc )
		{
			Name = desc.Name==0?"":desc.Name;
			StartOffset = desc.StartOffset;
			Size = desc.Size;
			uFlags = desc.uFlags;
			StartTexture = desc.StartTexture;
			TextureSize = desc.TextureSize;
			StartSampler = desc.StartSampler;
			SamplerSize = desc.SamplerSize;
		}

		std::string             Name;           // Name of the variable
		UINT                    StartOffset;    // Offset in constant buffer's backing store
		UINT                    Size;           // Size of variable (in bytes)
		UINT                    uFlags;         // Variable flags
		UINT                    StartTexture;   // First texture index (or -1 if no textures used)
		UINT                    TextureSize;    // Number of texture slots possibly used.
		UINT                    StartSampler;   // First sampler index (or -1 if no textures used)
		UINT                    SamplerSize;    // Number of sampler slots possibly used.
	};

	struct ShaderTypeDesc
	{
		ShaderTypeDesc(D3D11_SHADER_TYPE_DESC const& desc)
		{
			Class = desc.Class;
			Type = desc.Type;
			Rows = desc.Rows;
			Columns = desc.Columns;
			Elements = desc.Elements;
			Members = desc.Members;
			Offset = desc.Offset;
			Name = desc.Name==0?"":desc.Name;
		};

		D3D_SHADER_VARIABLE_CLASS   Class;          // Variable class (e.g. object, matrix, etc.)
		D3D_SHADER_VARIABLE_TYPE    Type;           // Variable type (e.g. float, sampler, etc.)
		UINT                        Rows;           // Number of rows (for matrices, 1 for other numeric, 0 if not applicable)
		UINT                        Columns;        // Number of columns (for vectors & matrices, 1 for other numeric, 0 if not applicable)
		UINT                        Elements;       // Number of elements (0 if not an array)
		UINT                        Members;        // Number of members (0 if not a structure)
		UINT                        Offset;         // Offset from the start of structure (0 if not a structure member)
		std::string                 Name;           // Name of type, can be NULL
	};

	struct ConstantBufferDesc
	{
		 std::string             CBName;           // Name of the constant buffer
         D3D_CBUFFER_TYPE        CBType;           // Indicates type of buffer content
         UINT                    CBVariables;      // Number of member variables
         UINT                    CBSize;           // Size of CB (in bytes)
         UINT                    CBuFlags;         // Buffer description flags
		std::vector<ShaderVariableDesc>	        Variables;
		std::vector<ShaderTypeDesc>		        Types;
		std::vector<ShaderPropertyPtr>		    Parameters;
		ShaderPropertyPtr					    BufferParamRef;
	};

	struct ShaderInputBindDesc
	{
		ShaderInputBindDesc( D3D11_SHADER_INPUT_BIND_DESC const& desc )
		{
			Name = std::string(desc.Name);
			Type = desc.Type;
			BindPoint = desc.BindPoint;
			BindCount = desc.BindCount;
			uFlags = desc.uFlags;
			ReturnType = desc.ReturnType;
			Dimension = desc.Dimension;
			NumSamples = desc.NumSamples;
			ParamRef = 0;
		};
		
		ShaderInputBindDesc()
		{
			Name = "";
			Type = D3D_SIT_CBUFFER;
			BindPoint = 0;
			BindCount = 0;
			uFlags = 0;
			ReturnType = D3D11_RETURN_TYPE_UNORM;
			Dimension = D3D_SRV_DIMENSION_UNKNOWN;
			NumSamples = 0;
			ParamRef = 0;
		};

		std::string Name;
		D3D_SHADER_INPUT_TYPE Type;
		UINT BindPoint;
		UINT BindCount;
		UINT uFlags;
		D3D11_RESOURCE_RETURN_TYPE ReturnType;
		D3D_SRV_DIMENSION Dimension;
		UINT NumSamples;
		ShaderPropertyPtr ParamRef;
	};

	struct ShaderSignatureDesc
	{
		ShaderSignatureDesc(D3D11_SIGNATURE_PARAMETER_DESC const& desc)
		{
			SemanticName = desc.SemanticName;
			SemanticIndex = desc.SemanticIndex;
			ComponentType = desc.ComponentType;
			Mask = desc.Mask;
		};

		UINT GetElementSize()
		{
			if( Mask == 1)
				return 4;
			else if( Mask == 3)
				return 4*2;
			else if( Mask == 7)
				return 4*3;
			else if( Mask == 15)
				return 4*4;

			return 0;
		};

	    std::string             SemanticName;   // Name of the semantic
        UINT                    SemanticIndex;  // Index of the semantic
 
        D3D_REGISTER_COMPONENT_TYPE ComponentType; 
        BYTE                        Mask;         
 
	};

	class DX11ShaderReflection
	{
	public:
		D3D11_SHADER_DESC								ShaderDescription;
		std::vector<ShaderSignatureDesc>		        InputSignatureParameters;
		std::vector<ShaderSignatureDesc>		        OutputSignatureParameters;
		std::vector<ConstantBufferDesc>				    ConstantBuffers;
		std::vector<ShaderInputBindDesc>				ResourceBindings;

		static DX11ShaderReflectionPtr Create(); 

	private:
		DX11ShaderReflection(){}
	};

	class DX11ShaderInclude : public ID3DInclude
	{
	public:
		STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			char* pData = NULL;
			int length = 0;
			if( ShaderObject::sMapIncludeFiles.find(pFileName) != ShaderObject::sMapIncludeFiles.end() )
			{
				std::string& content = ShaderObject::sMapIncludeFiles[pFileName];
				length = content.length();
				pData = new char[length + 1];
				memcpy(pData,content.c_str(),sizeof(char)*length);
				pData[length] = '\0';
			}
			else
			{
				std::string strFilePath = GConfig->sResourceFXPath + "HLSL\\" + std::string(pFileName);
				FileObjectPtr fileptr = GEngine->FileManager->OpenTextFile(strFilePath, eF_Read);
		        std::string shaderContent = fileptr->ReadText();
				ShaderObject::sMapIncludeFiles.insert(std::pair<std::string,std::string>(pFileName,shaderContent));
				length = shaderContent.length();
				pData = new char[shaderContent.length() + 1];
				memcpy(pData,shaderContent.c_str(),sizeof(char)*length);
				pData[length] = '\0';
			}
 
			*ppData = (LPCVOID)(pData);
			*pBytes = length;

			return S_OK;
		}

		STDMETHOD(Close)(THIS_ LPCVOID pData)
		{
			if( pData )
				delete (char*)pData;

			return S_OK;
		}
	};

	class DX11ShaderObject : public ShaderObject
	{
	public:
 
		virtual void UpdateShaderParameter();
		virtual bool LoadShaderFromFile(std::string const& fileName,std::string const& entryPoint,ShaderType shaderType); 
		virtual void ShaderReflection();

		virtual void* GetHandle();
		virtual void* GetDataInterface();
		
		static DX11ShaderObjectPtr Create(std::string const& effectName,std::string const& entryPoint,ShaderType shaderType,ShaderModel shaderMode);

	public:
		DX11ShaderReflectionPtr  ShaderReflect;
		ID3D11VertexShaderPtr VertexShaderInterface;
		ID3D11PixelShaderPtr  PixelShaderInterface;
		ID3D11GeometryShaderPtr GeometryShaderInterface;
		ID3DBlobPtr DataInterface;

		std::vector<ID3D11Buffer*> CachedConstBuffer;
		std::vector<ID3D11SamplerState*> CachedSamplerState;
		std::vector<ID3D11ShaderResourceView*> CachedShaderResourceView;

	private:

		bool _bCacheRefresh;

		HRESULT CompileShaderFromFile(std::string const& fileName, std::string const& entryPoint, std::string const& shaderModel, ID3DBlob** ppBlobOut);

		explicit DX11ShaderObject(std::string const& fileName,std::string const& entryPoint,ShaderType shaderType,ShaderModel shaderModel)
		{
			_fileName = fileName;
			_entryPoint = entryPoint;
			_type = shaderType;
			_shaderModel = shaderModel;

			_bCacheRefresh = true;
		}

		inline std::string GetShaderModelDes(ShaderType type)
		{
			if( _shaderModel == SM_4_0 )
			{
				if( type == ST_VertexShader )
				    return "vs_4_0";
				else if (type == ST_PixelShader)
					return "ps_4_0";
				else if (type == ST_GeometryShader)
					return "gs_4_0";
			}

			BOOST_ASSERT(0);

			return "";
		};
	};

	class DX11ShaderPropertyManager : public ShaderPropertyManager
	{
	public:
		~DX11ShaderPropertyManager();
		virtual void UpdateShaderProperty();
		bool Validate(ConstantBufferDesc *pConstBuffer);
		static DX11ShaderPropertyManagerPtr Create(const std::string& name);

		ConstantBufferDesc* ConstantBuffer;

	protected:
		DX11ShaderPropertyManager(const std::string& name);
		

		
	};
 
}

#endif