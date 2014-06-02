#include "DX11RenderInclude.h"

namespace Disorder
{
 
	DX11RenderResourceManagerPtr DX11RenderResourceManager::Create()
	{
		DX11RenderResourceManager *pManager = new DX11RenderResourceManager;
		return DX11RenderResourceManagerPtr(pManager);
	}

    ShaderObjectPtr DX11RenderResourceManager::CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint)
	{
		std::string shaderKey = fileName + entryPoint;
		if( _shaderMap.find(shaderKey) != _shaderMap.end() )
			return _shaderMap.at(shaderKey);
 
		std::string strFilePath = GConfig->sResourceFXPath + "HLSL\\" + fileName + ".hlsl";

		DX11ShaderObjectPtr shaderObject = DX11ShaderObject::Create(fileName,entryPoint,type,shaderModel); 
	    shaderObject->LoadShaderFromFile(strFilePath,entryPoint,type);
	 
		_shaderMap.insert(std::pair<std::string,ShaderObjectPtr>(shaderKey,shaderObject));

		return shaderObject;
		 
	}

	DepthStencilStatePtr DX11RenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		std::wstring hashKey;
		Math::HashBuffer(pDepthStencilDesc, sizeof(DepthStencilDesc), hashKey);
		if (_depthStencilMap.find(hashKey) != _depthStencilMap.end())
			return _depthStencilMap.at(hashKey);

		DepthStencilStatePtr state = DX11DepthStencilState::Create(pDepthStencilDesc,stencilRef);

		_depthStencilMap.insert(std::pair<std::wstring, DepthStencilStatePtr>(hashKey, state));

		return state;
	}

	RasterizeStatePtr DX11RenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		std::wstring hashKey;
		Math::HashBuffer(pDesc, sizeof(RasterizeDesc), hashKey);
		if (_rasterizeStateMap.find(hashKey) != _rasterizeStateMap.end())
			return _rasterizeStateMap.at(hashKey);

		RasterizeStatePtr state = DX11RasterizeState::Create(pDesc);

		_rasterizeStateMap.insert(std::pair<std::wstring, RasterizeStatePtr>(hashKey, state));
		return state;
	}

	void DX11RenderResourceManager::UpdateRenderLayout(const std::wstring& hashKey, ID3D11InputLayoutPtr layout)
	{
		if (_renderLayoutMap.find(hashKey) != _renderLayoutMap.end())
			return;

		_renderLayoutMap.insert(std::pair<std::wstring, ID3D11InputLayoutPtr>(hashKey, layout));
	}

	ID3D11InputLayoutPtr DX11RenderResourceManager::GetRenderLayout(const std::wstring& hashKey)
	{
		if (_renderLayoutMap.find(hashKey) != _renderLayoutMap.end())
			return _renderLayoutMap.at(hashKey);

		return NULL;
	}

	RenderLayoutPtr DX11RenderResourceManager::CreateRenderLayout(RenderEffectPtr const& renderEffect,TopologyType topologyType,bool soloBuffer)
	{

		RenderLayoutPtr renderLayout = DX11RenderLayout::Create(renderEffect,topologyType,soloBuffer);
		return renderLayout;
	}
 
	void DX11RenderResourceManager::CreateBufferArray(GeometryPtr const& data,BufferUsage bufferUsage,RenderEffectPtr const& renderEffect,std::vector<RenderBufferPtr> & bufferArray )
	{		 
		DX11ShaderObjectPtr shader = boost::dynamic_pointer_cast<DX11ShaderObject>(renderEffect->GetVertexShader());
		//vertex buffer first
		for(unsigned int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
		{
			RenderBufferPtr renderBuffer = DX11RenderBuffer::Create(RBT_Vertex,data,shader->ShaderReflect->InputSignatureParameters[i].SemanticName,bufferUsage,shader);
			bufferArray.push_back(renderBuffer);
		}

		RenderBufferPtr indexBuffer = DX11RenderBuffer::Create(RBT_Index,data,"",bufferUsage,shader);
		bufferArray.push_back(indexBuffer);
	}

	RenderBufferPtr DX11RenderResourceManager::CreateBuffer(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint)
	{
		RenderBufferPtr renderBuffer = DX11RenderBuffer::Create(type,bufferUsage,elementSize,size,pData);
	 
		return renderBuffer;
	}
 
	RenderSurfacePtr DX11RenderResourceManager::CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap)
	{
		return DX11RenderSurface::Create(viewMap);
	}

	SurfaceViewPtr DX11RenderResourceManager::CreateSurfaceView(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat format, unsigned int flag)
	{
		DX11SurfaceViewPtr view = DX11SurfaceView::Create(type, resource, format, flag);
		return view;
	}
 

	RenderTexture2DPtr DX11RenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample,ImagePtr image)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat,bMultiSample,image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<ImagePtr>& image)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat, bMultiSample, image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag,int arraySize, BufferInitData const* pData)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat,width,height,bMipmap,bMultiSample,viewFlag,arraySize,pData);
		texture->Sampler = sampler;
		return texture;

	}

	SamplerStatePtr DX11RenderResourceManager::CreateSamplerState(SamplerDesc* pSamplerDesc)
	{
		std::wstring hashKey;
		Math::HashBuffer(pSamplerDesc, sizeof(SamplerDesc), hashKey);
		if (_samplerStateMap.find(hashKey) != _samplerStateMap.end())
			return _samplerStateMap.at(hashKey);

		SamplerStatePtr sampler = DX11SamplerState::Create(pSamplerDesc);

		_samplerStateMap.insert(std::pair<std::wstring, SamplerStatePtr>(hashKey, sampler));

		return sampler;
	}

	 

	BlendStatePtr DX11RenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		std::wstring hashKey;
		Math::HashBuffer(pBlendDescArray, sizeof(BlendDesc)*BlendArraySize, hashKey);
		if (_blendStateMap.find(hashKey) != _blendStateMap.end())
			return _blendStateMap.at(hashKey);

		DX11BlendStatePtr blendState = DX11BlendState::Create(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);
		
		_blendStateMap.insert(std::pair<std::wstring, BlendStatePtr>(hashKey, blendState));

		return blendState;
	}

	void DX11RenderResourceManager::RegisterPropertyManager(std::string const& name)
	{
		if( _propertyManagerMap.find(name) != _propertyManagerMap.end() )
			return;

		ShaderPropertyManagerPtr manger = DX11ShaderPropertyManager::Create(name);
		_propertyManagerMap.insert(std::pair<std::string,ShaderPropertyManagerPtr>(name,manger));
	}

	RenderEffectPtr DX11RenderResourceManager::CreateRenderEffect()
	{
		return RenderEffect::Create();
	}
}