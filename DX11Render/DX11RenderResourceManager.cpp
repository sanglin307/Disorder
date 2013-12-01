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
 
		std::string strFilePath = GConfig->sResourceFXPath + fileName + ".fx";

		DX11ShaderObjectPtr shaderObject = DX11ShaderObject::Create(fileName,entryPoint,type,shaderModel); 
	    shaderObject->LoadShaderFromFile(strFilePath,entryPoint,type);
	 
		_shaderMap.insert(std::pair<std::string,ShaderObjectPtr>(shaderKey,shaderObject));

		return shaderObject;
		 
	}

	DepthStencilStatePtr DX11RenderResourceManager::CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef)
	{
		DepthStencilStatePtr state = DX11DepthStencilState::Create(pDepthStencilDesc,stencilRef);
		return state;
	}

	RasterizeStatePtr DX11RenderResourceManager::CreateRasterizeState(RasterizeDesc *pDesc)
	{
		RasterizeStatePtr state = DX11RasterizeState::Create(pDesc);

		return state;
	}


	RenderLayoutPtr DX11RenderResourceManager::CreateRenderLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer)
	{
 
		RenderLayoutPtr renderLayout = DX11RenderLayout::Create(vertexShader,topologyType,soloBuffer);
		return renderLayout;
	}
 
	RenderBufferPtr DX11RenderResourceManager::CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,GeometryPtr const& data,std::string const& sematic,ShaderObjectPtr const& vertexShader)
	{
		RenderBufferPtr renderBuffer = DX11RenderBuffer::Create(type,data,sematic,accessHint,vertexShader);
		 
		return renderBuffer;
	}

	void DX11RenderResourceManager::CreateRenderBufferArray(GeometryPtr const& data, unsigned int accessHint,ShaderObjectPtr const& vertexShader,std::vector<RenderBufferPtr> & bufferArray )
	{		 
		DX11ShaderObjectPtr shader = boost::dynamic_pointer_cast<DX11ShaderObject>(vertexShader);
		//vertex buffer first
		for(unsigned int i=0; i< shader->ShaderReflect->InputSignatureParameters.size();++i)
		{
			RenderBufferPtr renderBuffer = DX11RenderBuffer::Create(RBT_Vertex,data,shader->ShaderReflect->InputSignatureParameters[i].SemanticName,accessHint,vertexShader);
			bufferArray.push_back(renderBuffer);
		}

		RenderBufferPtr indexBuffer = DX11RenderBuffer::Create(RBT_Index,data,"",accessHint,vertexShader);
		bufferArray.push_back(indexBuffer);
	}

    RenderBufferPtr DX11RenderResourceManager::CreateRenderBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData) 
	{
		RenderBufferPtr renderBuffer = DX11RenderBuffer::Create(type,accessHint,elementSize,size,pData);
	 
		return renderBuffer;
	}
 
	RenderSurfacePtr DX11RenderResourceManager::CreateRenderSurface(RenderTexture2DPtr const& texture,unsigned int usage)
	{
		RenderSurfacePtr surface = DX11RenderSurface::Create(texture,usage);
		 
		return surface;

	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,ImagePtr image)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat,image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateRenderTexture2D(SamplerStatePtr const& sampler,PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,BufferInitData const* pData)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat,width,height,bMipmap,pData);
		texture->Sampler = sampler;
		return texture;

	}

	SamplerStatePtr DX11RenderResourceManager::CreateSamplerState(SamplerFilter filter,TextureAddressMode addressUVW,UINT maxAnisotropy)
	{
		SamplerStatePtr sampler = DX11SamplerState::Create(filter,addressUVW,maxAnisotropy);
		return sampler;
	}

	 

	BlendStatePtr DX11RenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		DX11BlendStatePtr blendState = DX11BlendState::Create(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);
		 
		return blendState;
	}

	
}