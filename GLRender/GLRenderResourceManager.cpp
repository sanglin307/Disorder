#include "GLRenderInclude.h"


namespace Disorder
{
	RenderLayoutPtr GLRenderResourceManager::CreateRenderLayout(ShaderObjectPtr const& vertexShader, TopologyType topologyType, bool soloBuffer)
	{
		return NULL;
	}
	ShaderObjectPtr GLRenderResourceManager::CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel, std::string const& entryPoint)
	{
		std::string fileSuffix = GLShaderObject::GetShaderFileSuffix(type);
		std::string shaderKey = fileName + fileSuffix;
		if (_shaderMap.find(shaderKey) != _shaderMap.end())
			return _shaderMap.at(shaderKey);

		std::string strFilePath = GConfig->sResourceFXPath + "GLSL\\" + fileName + fileSuffix;

		GLShaderObjectPtr shaderObject = GLShaderObject::Create(fileName, "main", type, shaderModel);
		shaderObject->LoadShaderFromFile(strFilePath, "main", type);

		_shaderMap.insert(std::pair<std::string, ShaderObjectPtr>(shaderKey, shaderObject));

		return shaderObject;
	}
	RenderBufferPtr GLRenderResourceManager::CreateRenderBuffer(RenderBufferType type, unsigned int accessHint, GeometryPtr const& data, std::string const& sematic, ShaderObjectPtr const& vertexShader)
	{
		return NULL;
	}
	RenderBufferPtr GLRenderResourceManager::CreateRenderBuffer(RenderBufferType type, unsigned int accessHint, unsigned int elementSize, unsigned int size, void *pData)
	{
		return NULL;
	}

	void GLRenderResourceManager::CreateRenderBufferArray(GeometryPtr const& data, unsigned int accessHint, ShaderObjectPtr const& vertexShader, std::vector<RenderBufferPtr> & bufferArray)
	{
	
	}

	RenderTexture2DPtr GLRenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, unsigned int usage, BufferInitData const* pData)
	{
		return NULL;
	}

	RenderTexture2DPtr GLRenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, ImagePtr image)
	{
		return NULL;
	}

	RenderSurfacePtr GLRenderResourceManager::CreateRenderSurface(RenderTexture2DPtr const& texture, unsigned int usage, PixelFormat RenderTargetFormat, PixelFormat DepthFormat, PixelFormat ShaderResFormat, bool ReadOnlyDepth, bool ReadOnlyStencil)
	{
		return NULL;
	}

	SamplerStatePtr GLRenderResourceManager::CreateSamplerState(SamplerFilter filter, TextureAddressMode addressUVW, UINT maxAnisotropy)
	{
		return NULL;
	}

	RasterizeStatePtr GLRenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		return NULL;
	}

	DepthStencilStatePtr GLRenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef)
	{
		return NULL;
	}

	BlendStatePtr GLRenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
	{
		return NULL;
	}

	void GLRenderResourceManager::RegisterPropertyManager(std::string const& name)
	{
		
	}

	GLRenderResourceManagerPtr GLRenderResourceManager::Create()
	{
		GLRenderResourceManager *pManager = new GLRenderResourceManager;
		return GLRenderResourceManagerPtr(pManager);
	}




}