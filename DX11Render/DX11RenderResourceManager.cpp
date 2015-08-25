#include "DX11RenderInclude.h"

namespace Disorder
{

    ShaderObject* DX11RenderResourceManager::CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint)
	{
		std::string shaderKey = fileName + entryPoint;
		if( _shaderMap.find(shaderKey) != _shaderMap.end() )
			return _shaderMap.at(shaderKey);
 
		std::string strFilePath = GConfig->ResourceFXPath + "HLSL\\" + fileName + ".hlsl";

		DX11ShaderObject* shaderObject = new DX11ShaderObject(fileName,entryPoint,type,shaderModel); 
	    shaderObject->LoadShaderFromFile(strFilePath,entryPoint,type);
	 
		_shaderMap.insert(std::pair<std::string,ShaderObject*>(shaderKey,shaderObject));

		return shaderObject;
		 
	}

	DepthStencilState* DX11RenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		std::wstring hashKey;
		Math::HashBuffer(pDepthStencilDesc, sizeof(DepthStencilDesc), hashKey);
		if (_depthStencilMap.find(hashKey) != _depthStencilMap.end())
			return _depthStencilMap.at(hashKey);

		DepthStencilState* state = new DX11DepthStencilState(pDepthStencilDesc,stencilRef);

		_depthStencilMap.insert(std::pair<std::wstring, DepthStencilState*>(hashKey, state));

		return state;
	}

	RasterizeState* DX11RenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		std::wstring hashKey;
		Math::HashBuffer(pDesc, sizeof(RasterizeDesc), hashKey);
		if (_rasterizeStateMap.find(hashKey) != _rasterizeStateMap.end())
			return _rasterizeStateMap.at(hashKey);

		RasterizeState* state = new DX11RasterizeState(pDesc);

		_rasterizeStateMap.insert(std::pair<std::wstring, RasterizeState*>(hashKey, state));
		return state;
	}
 
	RenderLayout* DX11RenderResourceManager::CreateRenderLayout(RenderEffect* renderEffect, TopologyType topologyType, bool soloBuffer)
	{
		std::string name = renderEffect->GetVertexShader()->GetShaderName();
		if (_renderLayoutMap.find(name) != _renderLayoutMap.end())
			return _renderLayoutMap.at(name);

		RenderLayout* renderLayout = new DX11RenderLayout(renderEffect, topologyType, soloBuffer);

		_renderLayoutMap.insert(std::pair<std::string, RenderLayout*>(name, renderLayout));
		return renderLayout;
	}
 
	void DX11RenderResourceManager::CreateBufferArray(const std::string& bufferName, Geometry* data, BufferUsage bufferUsage, RenderEffect* renderEffect, std::vector<RenderBuffer*> & bufferArray)
	{		 
		DX11ShaderObject* shader = (DX11ShaderObject*)renderEffect->GetVertexShader();
		//vertex buffer first
		for(unsigned int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
		{
			std::string name = bufferName + "_" + shader->ShaderReflect->InputSignatureParameters[i].SemanticName;
			if (_renderBufferMap.find(name) == _renderBufferMap.end())
			{
				RenderBuffer* renderBuffer = new DX11RenderBuffer(name, RBT_Vertex, data, shader->ShaderReflect->InputSignatureParameters[i].SemanticName, bufferUsage, shader);
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
			RenderBuffer* indexBuffer = new DX11RenderBuffer(indexName, RBT_Index, data, "", bufferUsage, shader);
			bufferArray.push_back(indexBuffer);
			_renderBufferMap.insert(std::pair<std::string, RenderBuffer*>(indexName, indexBuffer));
		}
		else
		{
			bufferArray.push_back(_renderBufferMap.at(indexName));
		}
		
	}

	RenderBuffer* DX11RenderResourceManager::CreateBuffer(const std::string& bufferName, RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint)
	{
		if (_renderBufferMap.find(bufferName) != _renderBufferMap.end())
			return _renderBufferMap.at(bufferName);

		RenderBuffer* renderBuffer = new DX11RenderBuffer(bufferName,type,bufferUsage,elementSize,size,pData);
	 
		_renderBufferMap.insert(std::pair<std::string, RenderBuffer*>(bufferName, renderBuffer));
		return renderBuffer;
	}
 
	RenderSurface* DX11RenderResourceManager::CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap)
	{
		return new DX11RenderSurface(viewMap);
	}

	SurfaceView* DX11RenderResourceManager::CreateSurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat format, unsigned int flag)
	{
		DX11SurfaceView* view = new DX11SurfaceView(type, resource, format, flag);
		return view;
	}
 

	RenderTexture2D* DX11RenderResourceManager::CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, Image* image)
	{
		if (_renderTexture2DMap.find(name) != _renderTexture2DMap.end())
			return _renderTexture2DMap.at(name);

		RenderTexture2D* texture = new DX11RenderTexture2D(pixelFormat, bMultiSample, image);
		texture->Sampler = sampler;

		_renderTexture2DMap.insert(std::pair<std::string, RenderTexture2D*>(name, texture));
		return texture;
	}

	RenderTexture2D* DX11RenderResourceManager::CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag)
	{
		if (_renderTexture2DMap.find(name) != _renderTexture2DMap.end())
			return _renderTexture2DMap.at(name);

		RenderTexture2D* texture = new DX11RenderTexture2D(pixelFormat, bMultiSample, image, flag);
		texture->Sampler = sampler;

		_renderTexture2DMap.insert(std::pair<std::string, RenderTexture2D*>(name, texture));
		return texture;
	}

	RenderTexture2D* DX11RenderResourceManager::CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag)
	{
		if (_renderTexture2DMap.find(name) != _renderTexture2DMap.end())
			return _renderTexture2DMap.at(name);

		RenderTexture2D* texture = new DX11RenderTexture2D(pixelFormat, width, height, bMipmap, bMultiSample, viewFlag, arraySize, pData, flag);
		texture->Sampler = sampler;

		_renderTexture2DMap.insert(std::pair<std::string, RenderTexture2D*>(name, texture));
		return texture;

	}

	SamplerState* DX11RenderResourceManager::CreateSamplerState(SamplerDesc* pSamplerDesc)
	{
		std::wstring hashKey;
		Math::HashBuffer(pSamplerDesc, sizeof(SamplerDesc), hashKey);
		if (_samplerStateMap.find(hashKey) != _samplerStateMap.end())
			return _samplerStateMap.at(hashKey);

		SamplerState* sampler = new DX11SamplerState(pSamplerDesc);

		_samplerStateMap.insert(std::pair<std::wstring, SamplerState*>(hashKey, sampler));

		return sampler;
	}

	DX11RenderResourceManager::DX11RenderResourceManager()
	{
		RasterizeDesc rasterDesc;
		DefaultRasterizeState = CreateRasterizeState(&rasterDesc);

		BlendDesc blendDesc;
		DefaultBlentState = CreateBlendState(&blendDesc, 1);

		DepthStencilDesc dsDesc;
		DefaultDepthStencilState = CreateDepthStencilState(&dsDesc, 0);

		SamplerDesc samplerDesc;
		DefaultSamplerState = CreateSamplerState(&samplerDesc);

		unsigned int pixelData[16];
		memset(pixelData, 0xFF, 16 * sizeof(unsigned int));
		BufferInitData data;
		data.Data = pixelData;
		data.RowPitch = 4 * 4;
		data.SlicePitch = 0;
		RenderTexture2D* tex = CreateTexture2D("DefaultTex", NULL, PF_R8G8B8A8_UNORM, 4, 4, false, false, SV_ShaderResource, 1, &data, 0);
		DefaultWhiteTexture2D = CreateSurfaceView(SV_ShaderResource, tex, PF_R8G8B8A8_UNORM);

		RegisterPropertyManager(ShaderPropertyManager::sManagerCamera);
		RegisterPropertyManager(ShaderPropertyManager::sManagerObject);
		RegisterPropertyManager(ShaderPropertyManager::sManagerMaterial);
		RegisterPropertyManager(ShaderPropertyManager::sManagerDirectionLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerPointLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerSpotLight);
		RegisterPropertyManager(ShaderPropertyManager::sManagerScene);
		RegisterPropertyManager(ShaderPropertyManager::sManagerShadowMapGen);
		RegisterPropertyManager(ShaderPropertyManager::sManagerGlobal);
	}

	BlendState* DX11RenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		std::wstring hashKey;
		Math::HashBuffer(pBlendDescArray, sizeof(BlendDesc)*BlendArraySize, hashKey);
		if (_blendStateMap.find(hashKey) != _blendStateMap.end())
			return _blendStateMap.at(hashKey);

		DX11BlendState* blendState = new DX11BlendState(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);
		
		_blendStateMap.insert(std::pair<std::wstring, BlendState*>(hashKey, blendState));

		return blendState;
	}

	void DX11RenderResourceManager::RegisterPropertyManager(std::string const& name)
	{
		if( _propertyManagerMap.find(name) != _propertyManagerMap.end() )
			return;

		ShaderPropertyManager* manger = new DX11ShaderPropertyManager(name);
		_propertyManagerMap.insert(std::pair<std::string,ShaderPropertyManager*>(name,manger));
	}

	RenderEffect* DX11RenderResourceManager::CreateRenderEffect()
	{
		return new RenderEffect;
	}
}