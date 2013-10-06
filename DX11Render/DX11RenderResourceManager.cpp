#include "DX11RenderInclude.h"

namespace Disorder
{
 
    RenderEffectPtr DX11RenderResourceManager::CreateRenderEffect(std::string const& fileName,ShaderModel shaderModel,std::string const& entryPointVS,std::string const& entryPointPS )
	{

		if( _effectMap.find(fileName) != _effectMap.end() )
			return _effectMap.at(fileName);
 
		std::string strFilePath = GConfig->sResourceFXPath + fileName;
		RenderEffectPtr technique = boost::make_shared<DX11RenderEffect>(shaderModel); 
	    technique->LoadShaderFromFile(strFilePath,entryPointVS,ST_VertexShader);
		technique->LoadShaderFromFile(strFilePath,entryPointPS,ST_PixelShader);

		_effectMap.insert(std::pair<std::string,RenderEffectPtr>(fileName,technique));

		return technique;
		 
	}

	DepthStencilStatePtr DX11RenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		DepthStencilStatePtr state = boost::make_shared<DX11DepthStencilState>();
		bool result = state->Create(pDepthStencilDesc,stencilRef);
		if( result )
			return state;
		else
			return NULL;
	}

	RasterizeStatePtr DX11RenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		RasterizeStatePtr state = boost::make_shared<DX11RasterizeState>();

		bool result = state->Create(pDesc);
		if( result )
			return state;
		else
			return NULL;
	}


	RenderLayoutPtr DX11RenderResourceManager::CreateRenderLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer)
	{
 
		RenderLayoutPtr renderLayout = boost::make_shared<DX11RenderLayout>();
		bool result = renderLayout->CreateLayout(vertexShader,topologyType,soloBuffer);
	 
		if(result)
		    return renderLayout;
		else
			return NULL;
	}

	RenderBufferPtr DX11RenderResourceManager::CreateConstBuffer(unsigned int size, unsigned int accessHint)
	{
		RenderBufferPtr renderBuffer = boost::make_shared<DX11RenderBuffer>();
		renderBuffer->CreateConstBuffer(size,accessHint);

		return renderBuffer;
	}

	RenderBufferPtr DX11RenderResourceManager::CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,GeometryPtr const& data,ShaderObjectPtr const& vertexShader)
	{
		RenderBufferPtr renderBuffer = boost::make_shared<DX11RenderBuffer>();
		renderBuffer->CreateBuffer(type,data,accessHint,vertexShader);

		return renderBuffer;
	}

	void DX11RenderResourceManager::CreateVertexBufferArray(GeometryPtr const& data, unsigned int accessHint,ShaderObjectPtr const& vertexShader,std::vector<RenderBufferPtr> & bufferArray )
	{		 
		DX11ShaderObjectPtr shader = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader);
		for(unsigned int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
		{
			RenderBufferPtr renderBuffer = boost::make_shared<DX11RenderBuffer>();
			renderBuffer->CreateVertexBuffer(data,shader->ShaderReflect->InputSignatureParameters[i].SemanticName,accessHint,vertexShader);
			bufferArray.push_back(renderBuffer);
		}
	}

    RenderBufferPtr DX11RenderResourceManager::CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData) 
	{
		RenderBufferPtr renderBuffer = boost::make_shared<DX11RenderBuffer>();
		renderBuffer->CreateBuffer(type,accessHint,elementSize,size,pData);

		return renderBuffer;
	}

	RenderViewPtr DX11RenderResourceManager::CreateTexture2DViewFromFile(std::string const& fileName)
	{
		DX11ShaderResourceViewPtr shaderView = boost::make_shared<DX11ShaderResourceView>();
		std::string path =  GConfig->sResourceTexPath + fileName;
		shaderView->CreateTexture2DViewFromFile(path);

		return shaderView;
	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,BufferInitData const* pData)
	{
		RenderTexture2DPtr texture = boost::make_shared<DX11RenderTexture2D>();
		texture->Create(pixelFormat,width,height,bMipmap,pData);
		texture->Sampler = sampler;
		return texture;

	}

	SamplerStatePtr DX11RenderResourceManager::CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy)
	{
		SamplerStatePtr sampler = boost::make_shared<DX11SamplerState>();
		bool result = sampler->Create(filter,addressUVW,maxAnisotropy); 
 
		return sampler;
	}

	RenderViewPtr DX11RenderResourceManager::CreateTexture2DView(RenderTexture2DPtr const& textureResource)
	{
		DX11ShaderResourceViewPtr shaderView = boost::make_shared<DX11ShaderResourceView>();
		bool result = shaderView->CreateTexture2DView(textureResource);

		if( result )
			return shaderView;

		return RenderViewPtr();

	}

	BlendStatePtr DX11RenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		DX11BlendStatePtr blendState = boost::make_shared<DX11BlendState>();
		bool result = blendState->Create(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);

		return blendState;
	}

	
}