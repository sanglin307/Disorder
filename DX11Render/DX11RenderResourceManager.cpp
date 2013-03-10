#include "DX11RenderInclude.h"

namespace Disorder
{
    RenderTechniquePtr DX11RenderResourceManager::CreateRenderTechnique(std::string const& fileName,ShaderModel shaderModel,std::string const& entryPointVS,std::string const& entryPointPS)
	{

		if( _techniqueMap.find(fileName) != _techniqueMap.end() )
			return _techniqueMap.at(fileName);
 
		std::string strFilePath = GConfig->sRunningDictioary + "\\Resource\\FX\\" + fileName;
		RenderTechniquePtr technique = boost::make_shared<DX11RenderTechnique>(shaderModel); 
	    technique->LoadVertexShaderFromFile(strFilePath,entryPointVS);
		technique->LoadPixelShaderFromFile(strFilePath,entryPointPS);

		_techniqueMap.insert(std::pair<std::string,RenderTechniquePtr>(fileName,technique));

		return technique;
		 
	}

	RasterizeStatePtr DX11RenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		RasterizeStatePtr state = boost::make_shared<DX11RasterizeState>();

		bool result = state->Create(pDesc);
		if( result )
			return state;
		else
			return boost::shared_ptr<RasterizeState>();
	}

	RenderLayoutPtr DX11RenderResourceManager::CreateRenderLayout(VertexShaderPtr const& vertexShader,const VertexInputDes* pVertexInputDes,unsigned int vertexInputDesLength,TopologyType topologyType)
	{
		// we should generate a vertexelement des string, and reuse the renderlayout if we can.
		std::string key = RenderLayout::GenerateMapKey(pVertexInputDes,vertexInputDesLength,topologyType);

		if( _renderLayoutMap.find(key) != _renderLayoutMap.end() )
			return _renderLayoutMap.at(key);

		RenderLayoutPtr renderLayout = boost::make_shared<DX11RenderLayout>();
		bool result = renderLayout->CreateLayout(vertexShader,pVertexInputDes,vertexInputDesLength,topologyType);
		if( result )
			_renderLayoutMap.insert(std::pair<std::string,RenderLayoutPtr>(key,renderLayout));

		return renderLayout;
	}

	RenderBufferPtr DX11RenderResourceManager::CreateRenderBuffer(RenderBufferType type,unsigned int elementSize,unsigned int bufferSize,unsigned int accessHint,BufferInitData const* pData)
	{
		RenderBufferPtr renderBuffer = boost::make_shared<DX11RenderBuffer>();
		renderBuffer->CreateBuffer(type,elementSize,bufferSize,accessHint,pData);

		return renderBuffer;
	}

	RenderViewPtr DX11RenderResourceManager::CreateTexture2DViewFromFile(std::string const& fileName)
	{
		DX11ShaderResourceViewPtr shaderView = boost::make_shared<DX11ShaderResourceView>();
		std::string path =  GConfig->sRunningDictioary + "\\Resource\\Texture\\" + fileName;
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