#include "GLRenderInclude.h"


namespace Disorder
{
	RenderLayout* GLRenderResourceManager::CreateRenderLayout(RenderEffect* renderEffect, TopologyType topologyType, bool soloBuffer)
	{
		std::string name = renderEffect->GetVertexShader()->GetShaderName();
		if (_renderLayoutMap.find(name) != _renderLayoutMap.end())
			return _renderLayoutMap.at(name);

		RenderLayout* renderLayout = new GLRenderLayout(renderEffect, topologyType, soloBuffer);

		_renderLayoutMap.insert(std::pair<std::string, RenderLayout*>(name, renderLayout));
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
		if (_renderBufferMap.find(bufferName) != _renderBufferMap.end())
			return _renderBufferMap.at(bufferName);

		RenderBuffer* renderBuffer = new GLRenderBuffer(type, bufferUsage, elementSize, size, pData, bindingPoint);

		_renderBufferMap.insert(std::pair<std::string, RenderBuffer*>(bufferName, renderBuffer));
		return renderBuffer;
	}

	void GLRenderResourceManager::CreateBufferArray(const std::string& bufferName, Geometry* data, BufferUsage bufferUsage, RenderEffect* renderEffect, std::vector<RenderBuffer*> & bufferArray)
	{
		GLRenderEffect* effect = (GLRenderEffect*)renderEffect;
	 
		for(unsigned int i=0; i< effect->EffectReflection->InputArray.size();++i)
		{
			std::string name = bufferName + "_VertexBuffer";
			if (_renderBufferMap.find(name) == _renderBufferMap.end())
			{
				RenderBuffer* renderBuffer = new GLRenderBuffer(RBT_Vertex, data, effect->EffectReflection->InputArray[i].Location, bufferUsage, renderEffect);
				bufferArray.push_back(renderBuffer);
				_renderBufferMap.insert(std::pair<std::string, RenderBuffer*>(name, renderBuffer));
			}
			else
			{
				bufferArray.push_back(_renderBufferMap.at(name));
			}
		}

		std::string indexName = bufferName + "_IndexBuffer";
		if (_renderBufferMap.find(indexName) == _renderBufferMap.end())
		{
			RenderBuffer* indexBuffer = new GLRenderBuffer(RBT_Index, data, 0, bufferUsage, effect);
			bufferArray.push_back(indexBuffer);
			_renderBufferMap.insert(std::pair<std::string, RenderBuffer*>(indexName, indexBuffer));
		}
		else
		{
			bufferArray.push_back(_renderBufferMap.at(indexName));
		}
	
	}
 
	SurfaceView* GLRenderResourceManager::CreateSurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat format, unsigned int flag)
	{
		return new GLSurfaceView(type,resource,format,flag);
	}

	RenderTexture2D* GLRenderResourceManager::CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, Image* image)
	{
		if (_renderTexture2DMap.find(name) != _renderTexture2DMap.end())
			return _renderTexture2DMap.at(name);

		GLRenderTexture2D* texture = new GLRenderTexture2D(pixelFormat, bMultiSample, image);
		texture->Sampler = sampler;

		_renderTexture2DMap.insert(std::pair<std::string, RenderTexture2D*>(name, texture));
		return texture;
	}

	RenderTexture2D* GLRenderResourceManager::CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag)
	{
		if (_renderTexture2DMap.find(name) != _renderTexture2DMap.end())
			return _renderTexture2DMap.at(name);

		GLRenderTexture2D* texture = new GLRenderTexture2D(pixelFormat, bMultiSample, image, flag);
		texture->Sampler = sampler;

		_renderTexture2DMap.insert(std::pair<std::string, RenderTexture2D*>(name, texture));
		return texture;
	}


	RenderTexture2D* GLRenderResourceManager::CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, unsigned int width, unsigned int hight, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		if (_renderTexture2DMap.find(name) != _renderTexture2DMap.end())
			return _renderTexture2DMap.at(name);

		GLRenderTexture2D* texture = new GLRenderTexture2D(pixelFormat, width, hight, bMipmap, bMultiSample, viewFlag, arraySize, pData, flag);
		texture->Sampler = sampler;

		_renderTexture2DMap.insert(std::pair<std::string, RenderTexture2D*>(name, texture));
		return texture;
	}

	RenderSurface* GLRenderResourceManager::CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap)
	{
	    return new GLRenderSurface(viewMap);
	}


	SamplerState* GLRenderResourceManager::CreateSamplerState(SamplerDesc* pSamplerDesc)
	{
		std::wstring hashKey;
		Math::HashBuffer(pSamplerDesc, sizeof(SamplerDesc), hashKey);
		if (_samplerStateMap.find(hashKey) != _samplerStateMap.end())
			return _samplerStateMap.at(hashKey);

		SamplerState* sampler = new GLSamplerState(pSamplerDesc);

		_samplerStateMap.insert(std::pair<std::wstring, SamplerState*>(hashKey, sampler));

		return sampler;
	}

	RasterizeState* GLRenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		std::wstring hashKey;
		Math::HashBuffer(pDesc, sizeof(RasterizeDesc), hashKey);
		if (_rasterizeStateMap.find(hashKey) != _rasterizeStateMap.end())
			return _rasterizeStateMap.at(hashKey);

		RasterizeState* state = new GLRasterizeState(pDesc);

		_rasterizeStateMap.insert(std::pair<std::wstring, RasterizeState*>(hashKey, state));
		return state;
	}

	DepthStencilState* GLRenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef)
	{
		std::wstring hashKey;
		Math::HashBuffer(pDepthStencilDesc, sizeof(DepthStencilDesc), hashKey);
		if (_depthStencilMap.find(hashKey) != _depthStencilMap.end())
			return _depthStencilMap.at(hashKey);

		DepthStencilState* state = new GLDepthStencilState(pDepthStencilDesc, stencilRef);

		_depthStencilMap.insert(std::pair<std::wstring, DepthStencilState*>(hashKey, state));

		return state;
	}

	BlendState* GLRenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
	{
		std::wstring hashKey;
		Math::HashBuffer(pBlendDescArray, sizeof(BlendDesc)*BlendArraySize, hashKey);
		if (_blendStateMap.find(hashKey) != _blendStateMap.end())
			return _blendStateMap.at(hashKey);

		GLBlendState* blendState = new GLBlendState(pBlendDescArray, BlendArraySize, AlphaToCoverageEnable, IndependentBlendEnable);

		_blendStateMap.insert(std::pair<std::wstring, BlendState*>(hashKey, blendState));

		return blendState;
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