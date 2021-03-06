#ifndef _DISORDER_RENDERRESOURCEMANAGER_H_
#define _DISORDER_RENDERRESOURCEMANAGER_H_
 

namespace Disorder
{
	class RenderResourceManager 
	{
		typedef boost::unordered_map<std::string,ShaderObject*>  ShaderMap;
		typedef boost::unordered_map<std::string,ShaderPropertyManager*> ShaderPropertyManagerMap;

		typedef boost::unordered_map<std::wstring, DepthStencilState*> DepthStencilMap;		
		typedef boost::unordered_map<std::wstring, SamplerState*> SamplerStateMap;
		typedef boost::unordered_map<std::wstring, RasterizeState*> RaterizeStateMap;
		typedef boost::unordered_map<std::wstring, BlendState*> BlendStateMap;

		typedef boost::unordered_map<std::string, RenderLayout*> RenderLayoutMap;
		typedef boost::unordered_map<std::string, RenderBuffer*> RenderBufferMap;
		typedef boost::unordered_map<std::string, RenderTexture2D*> RenderTexture2DMap;
		typedef boost::unordered_map<std::string, SurfaceView*> SurfaceViewMap;
		typedef boost::unordered_map<std::string, RenderSurface*> RenderSurfaceMap;
		typedef boost::unordered_map<std::string, RenderEffect*> RenderEffectMap;


	public:
		virtual ~RenderResourceManager();

		virtual ShaderObject* CreateShader(ShaderType type, std::string const& fileName, ShaderModel shaderModel,std::string const& entryPoint) = 0;
		virtual RenderLayout* CreateRenderLayout(RenderEffect *renderEffect,TopologyType topologyType,bool soloBuffer) = 0;
		virtual RenderBuffer* CreateBuffer(const std::string& bufferName,RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData,int bindingPoint = 0) = 0;
		virtual void CreateBufferArray(const std::string& bufferName,Geometry* data, BufferUsage bufferUsage,RenderEffect* renderEffect, std::vector<RenderBuffer*> & bufferArray) = 0;

		virtual RenderTexture2D* CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag) = 0;
		virtual RenderTexture2D* CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, Image* image) = 0;
		virtual RenderTexture2D* CreateTexture2D(const std::string& name, SamplerState* sampler, PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag) = 0;
		
		virtual SurfaceView* CreateSurfaceView(ESurfaceViewType type,RenderTexture* resource, PixelFormat Format, unsigned int Flag = 0) = 0;
		virtual RenderSurface* CreateRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap) = 0;
		virtual RenderEffect* CreateRenderEffect() = 0;

		virtual SamplerState* CreateSamplerState(SamplerDesc* pSamplerDesc) = 0;
		virtual RasterizeState* CreateRasterizeState(RasterizeDesc *pDesc) = 0;
		virtual DepthStencilState* CreateDepthStencilState(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef) = 0;
		virtual BlendState* CreateBlendState(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false) = 0;

		virtual void RegisterPropertyManager(std::string const& name) = 0;
		ShaderPropertyManager* GetPropertyManager(std::string const& name);

		void DumpContent();
 

	public:
		RasterizeState* DefaultRasterizeState;
		BlendState* DefaultBlentState;
		DepthStencilState* DefaultDepthStencilState;		
		SurfaceView* DefaultWhiteTexture2D;
		SamplerState* DefaultSamplerState;

	protected:
		ShaderMap _shaderMap;
		ShaderPropertyManagerMap _propertyManagerMap;
		DepthStencilMap _depthStencilMap;	
		SamplerStateMap _samplerStateMap;
		RaterizeStateMap _rasterizeStateMap;
		BlendStateMap _blendStateMap;

		RenderLayoutMap _renderLayoutMap;
		RenderBufferMap _renderBufferMap;
		RenderTexture2DMap _renderTexture2DMap;
		SurfaceViewMap _surfaceViewMap;
		RenderSurfaceMap _renderSurfaceMap;
		RenderEffectMap _renderEffectMap;
		
 
	};

	
}


#endif