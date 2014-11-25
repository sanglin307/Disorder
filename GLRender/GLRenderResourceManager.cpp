#include "GLRenderInclude.h"


namespace Disorder
{
	RenderLayout* GLRenderResourceManager::CreateRenderLayout(RenderEffect* renderEffect, TopologyType topologyType, bool soloBuffer)
	{
		RenderLayout* renderLayout = new GLRenderLayout(renderEffect,topologyType,soloBuffer);
		return renderLayout;
	}
	ShaderObject* GLRenderResourceManager::CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel, std::string const& entryPoint)
	{
		std::string fileSuffix = GLShaderObject::GetShaderFileSuffix(type);
		std::string shaderKey = fileName + "\\" + entryPoint + fileSuffix;
		if (_shaderMap.find(shaderKey) != _shaderMap.end())
			return _shaderMap.at(shaderKey);

		std::string strFilePath = GConfig->ResourceFXPath + "GLSL\\" + shaderKey;

		GLShaderObject* shaderObject = new GLShaderObject(fileName, "main", type, shaderModel);
		shaderObject->LoadShaderFromFile(strFilePath, "main", type);

		_shaderMap.insert(std::pair<std::string, ShaderObject*>(shaderKey, shaderObject));

		return shaderObject;
	}
	
	RenderEffect* GLRenderResourceManager::CreateRenderEffect()
	{
		return new GLRenderEffect;
	}
 
	RenderBuffer* GLRenderResourceManager::CreateBuffer(const std::string& bufferName, RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint)
	{
		RenderBuffer* renderBuffer = new GLRenderBuffer(type,bufferUsage,elementSize,size,pData,bindingPoint);	 
		return renderBuffer;
	}

	void GLRenderResourceManager::CreateBufferArray(const std::string& bufferName, Geometry* data, BufferUsage bufferUsage, RenderEffect* renderEffect, std::vector<RenderBuffer*> & bufferArray)
	{
		GLRenderEffect* effect = (GLRenderEffect*)renderEffect;
	 
		for(unsigned int i=0; i< effect->EffectReflection->InputArray.size();++i)
		{
			RenderBuffer* renderBuffer = new GLRenderBuffer(RBT_Vertex,data,effect->EffectReflection->InputArray[i].Location,bufferUsage,renderEffect);
			bufferArray.push_back(renderBuffer);
		}

		RenderBuffer* indexBuffer = new GLRenderBuffer(RBT_Index,data,0,bufferUsage,effect);
		bufferArray.push_back(indexBuffer);
	}

	RenderTexture2D* GLRenderResourceManager::CreateTexture2D(SamplerState* sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		GLRenderTexture2D* texture = new GLRenderTexture2D(pixelFormat, width, hight, bMipmap, bMultiSample,viewFlag, arraySize, pData,flag);
		texture->Sampler = sampler;
		return texture;
	}

	SurfaceView* GLRenderResourceManager::CreateSurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat format, unsigned int flag)
	{
		return new GLSurfaceView(type,resource,format,flag);
	}

	RenderTexture2D* GLRenderResourceManager::CreateTexture2D(SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample,Image* image)
	{
		GLRenderTexture2D* texture = new GLRenderTexture2D(pixelFormat, bMultiSample,image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderSurface* GLRenderResourceManager::CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap)
	{
	    return new GLRenderSurface(viewMap);
	}

	RenderTexture2D* GLRenderResourceManager::CreateTexture2D(SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag)
	{
		GLRenderTexture2D* texture = new GLRenderTexture2D(pixelFormat, bMultiSample, image,flag);
		texture->Sampler = sampler;
		return texture;
	}

	SamplerState* GLRenderResourceManager::CreateSamplerState(SamplerDesc* pSamplerDesc)
	{
		return new GLSamplerState(pSamplerDesc);
	}

	RasterizeState* GLRenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		return new GLRasterizeState(pDesc);
	}

	DepthStencilState* GLRenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef)
	{
		return new GLDepthStencilState(pDepthStencilDesc, stencilRef);
	}

	BlendState* GLRenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
	{
		return new GLBlendState(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);
	}

	void GLRenderResourceManager::RegisterPropertyManager(std::string const& name)
	{
		if( _propertyManagerMap.find(name) != _propertyManagerMap.end() )
			return;

		GLShaderPropertyManager* manager = new GLShaderPropertyManager(name);
		if (name != ShaderPropertyManager::sManagerGlobal)
			manager->BindingPoint = _propertyManagerMap.size();
		else
			manager->BindingPoint = -1;

		_propertyManagerMap.insert(std::pair<std::string, ShaderPropertyManager*>(name, manager));
	}

	 


}