#include "GLRenderInclude.h"


namespace Disorder
{
	RenderLayoutPtr GLRenderResourceManager::CreateRenderLayout(RenderEffectPtr const& renderEffect, TopologyType topologyType, bool soloBuffer)
	{
		RenderLayoutPtr renderLayout = GLRenderLayout::Create(renderEffect,topologyType,soloBuffer);
		return renderLayout;
	}
	ShaderObjectPtr GLRenderResourceManager::CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel, std::string const& entryPoint)
	{
		std::string fileSuffix = GLShaderObject::GetShaderFileSuffix(type);
		std::string shaderKey = fileName + "_" + entryPoint + fileSuffix;
		if (_shaderMap.find(shaderKey) != _shaderMap.end())
			return _shaderMap.at(shaderKey);

		std::string strFilePath = GConfig->sResourceFXPath + "GLSL\\" + shaderKey;

		GLShaderObjectPtr shaderObject = GLShaderObject::Create(fileName, "main", type, shaderModel);
		shaderObject->LoadShaderFromFile(strFilePath, "main", type);

		_shaderMap.insert(std::pair<std::string, ShaderObjectPtr>(shaderKey, shaderObject));

		return shaderObject;
	}
	
	RenderEffectPtr GLRenderResourceManager::CreateRenderEffect()
	{
		return GLRenderEffect::Create();
	}
 
	RenderBufferPtr GLRenderResourceManager::CreateRenderBuffer(RenderBufferType type,BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData)
	{
		RenderBufferPtr renderBuffer = GLRenderBuffer::Create(type,bufferUsage,elementSize,size,pData);	 
		return renderBuffer;
	}

	void GLRenderResourceManager::CreateRenderBufferArray(GeometryPtr const& data, BufferUsage bufferUsage, RenderEffectPtr const& renderEffect, std::vector<RenderBufferPtr> & bufferArray)
	{
		GLRenderEffectPtr effect = boost::dynamic_pointer_cast<GLRenderEffect>(renderEffect);
	 
		for(unsigned int i=0; i< effect->EffectReflection->InputArray.size();++i)
		{
			RenderBufferPtr renderBuffer = GLRenderBuffer::Create(RBT_Vertex,data,effect->EffectReflection->InputArray[i].Location,bufferUsage,renderEffect);
			bufferArray.push_back(renderBuffer);
		}

		RenderBufferPtr indexBuffer = GLRenderBuffer::Create(RBT_Index,data,0,bufferUsage,effect);
		bufferArray.push_back(indexBuffer);
	}

	RenderTexture2DPtr GLRenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, unsigned int usage, BufferInitData const* pData)
	{
		GLRenderTexture2DPtr texture = GLRenderTexture2D::Create(pixelFormat,width,hight,bMipmap,usage,pData);
		texture->Sampler = sampler;
		return texture;
	}

	RenderTexture2DPtr GLRenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, ImagePtr image)
	{
		GLRenderTexture2DPtr texture = GLRenderTexture2D::Create(pixelFormat,image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderSurfacePtr GLRenderResourceManager::CreateRenderSurface(RenderTexture2DPtr const& texture, unsigned int usage, PixelFormat RenderTargetFormat, PixelFormat DepthFormat, PixelFormat ShaderResFormat, bool ReadOnlyDepth, bool ReadOnlyStencil)
	{
		return NULL;
	}

	SamplerStatePtr GLRenderResourceManager::CreateSamplerState(SamplerDesc* pSamplerDesc)
	{
		return GLSamplerState::Create(pSamplerDesc);
	}

	RasterizeStatePtr GLRenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		return GLRasterizeState::Create(pDesc);
	}

	DepthStencilStatePtr GLRenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef)
	{
		return GLDepthStencilState::Create(pDepthStencilDesc, stencilRef);
	}

	BlendStatePtr GLRenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
	{
		return GLBlendState::Create(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);
	}

	void GLRenderResourceManager::RegisterPropertyManager(std::string const& name)
	{
		if( _propertyManagerMap.find(name) != _propertyManagerMap.end() )
			return;

		ShaderPropertyManagerPtr manger = GLShaderPropertyManager::Create(name);
		_propertyManagerMap.insert(std::pair<std::string,ShaderPropertyManagerPtr>(name,manger));
	}

	GLRenderResourceManagerPtr GLRenderResourceManager::Create()
	{
		GLRenderResourceManager *pManager = new GLRenderResourceManager;
		return GLRenderResourceManagerPtr(pManager);
	}




}