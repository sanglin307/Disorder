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
		std::string shaderKey = fileName + "\\" + entryPoint + fileSuffix;
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
 
	RenderBufferPtr GLRenderResourceManager::CreateBuffer(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint)
	{
		RenderBufferPtr renderBuffer = GLRenderBuffer::Create(type,bufferUsage,elementSize,size,pData,bindingPoint);	 
		return renderBuffer;
	}

	void GLRenderResourceManager::CreateBufferArray(GeometryPtr const& data, BufferUsage bufferUsage, RenderEffectPtr const& renderEffect, std::vector<RenderBufferPtr> & bufferArray)
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

	RenderTexture2DPtr GLRenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample,unsigned int viewFlag,int arraySize, BufferInitData const* pData)
	{
		GLRenderTexture2DPtr texture = GLRenderTexture2D::Create(pixelFormat, width, hight, bMipmap, bMultiSample,viewFlag, arraySize, pData);
		texture->Sampler = sampler;
		return texture;
	}

	SurfaceViewPtr GLRenderResourceManager::CreateSurfaceView(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat format, unsigned int flag)
	{
		return GLSurfaceView::Create(type,resource,format,flag);
	}

	RenderTexture2DPtr GLRenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample,ImagePtr image)
	{
		GLRenderTexture2DPtr texture = GLRenderTexture2D::Create(pixelFormat, bMultiSample,image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderSurfacePtr GLRenderResourceManager::CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap)
	{
	    return GLRenderSurface::Create(viewMap);
	}

	RenderTexture2DPtr GLRenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<ImagePtr>& image)
	{
		GLRenderTexture2DPtr texture = GLRenderTexture2D::Create(pixelFormat, bMultiSample, image);
		texture->Sampler = sampler;
		return texture;
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

		GLShaderPropertyManagerPtr manager = GLShaderPropertyManager::Create(name);
		if (name != ShaderPropertyManager::sManagerGlobal)
			manager->BindingPoint = _propertyManagerMap.size();
		else
			manager->BindingPoint = -1;

		_propertyManagerMap.insert(std::pair<std::string, ShaderPropertyManagerPtr>(name, manager));
	}

	GLRenderResourceManagerPtr GLRenderResourceManager::Create()
	{
		GLRenderResourceManager *pManager = new GLRenderResourceManager;
		return GLRenderResourceManagerPtr(pManager);
	}




}