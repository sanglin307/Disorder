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
 
		std::string strFilePath = GConfig->sResourceFXPath + "HLSL\\" + fileName + ".dsf";

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

    RenderBufferPtr DX11RenderResourceManager::CreateBuffer(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData) 
	{
		RenderBufferPtr renderBuffer = DX11RenderBuffer::Create(type,bufferUsage,elementSize,size,pData);
	 
		return renderBuffer;
	}
 
	RenderSurfacePtr DX11RenderResourceManager::CreateShaderResource(RenderTexturePtr const& texture, PixelFormat format)
	{
		std::vector<sRenderSurfaceDes> vDes;
		sRenderSurfaceDes des;
		des.Format = format;
		des.Location = SL_ShaderResource;
		des.Resource = texture;
		RenderSurfacePtr surface = DX11RenderSurface::Create(vDes);

		return surface;
	}


	RenderSurfacePtr DX11RenderResourceManager::CreateRenderSurface(const std::vector<sRenderSurfaceDes>& surfaceDes)
	{
		DX11RenderSurfacePtr surface = DX11RenderSurface::Create(surfaceDes);
		return surface;
	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, bool bMultiSample,ImagePtr image)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat,bMultiSample,image);
		texture->Sampler = sampler;
		return texture;
	}

	RenderTexture2DPtr DX11RenderResourceManager::CreateTexture2D(SamplerStatePtr const& sampler, PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, const std::vector<ESurfaceLocation>& location, BufferInitData const* pData)
	{
		RenderTexture2DPtr texture = DX11RenderTexture2D::Create(pixelFormat,width,height,bMipmap,bMultiSample,location,pData);
		texture->Sampler = sampler;
		return texture;

	}

	SamplerStatePtr DX11RenderResourceManager::CreateSamplerState(SamplerDesc* pSamplerDesc)
	{
		SamplerStatePtr sampler = DX11SamplerState::Create(pSamplerDesc);
		return sampler;
	}

	 

	BlendStatePtr DX11RenderResourceManager::CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable,bool IndependentBlendEnable)
	{
		DX11BlendStatePtr blendState = DX11BlendState::Create(pBlendDescArray,BlendArraySize,AlphaToCoverageEnable,IndependentBlendEnable);
		 
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