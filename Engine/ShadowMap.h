#ifndef _DISORDER_SHADOWMAP_H_
#define _DISORDER_SHADOWMAP_H_
 
namespace Disorder
{
	class ShadowMap
	{
	public:
		ShadowMap(unsigned int width, unsigned int height);

		const unsigned int GetWidth() const
		{
			return _width;
		}

		const unsigned int GetHeight() const
		{
			return _height;
		}

		void RenderDepth(Camera* camera, const std::vector<GeometryRenderer*>& geometryList, Light* light);
		void PrepareRenderLight(const Light* light);

	protected:
		

		ShaderPropertyManager* _propertyMgr;

		ShaderProperty* _viewMatrix;
		ShaderProperty* _viewArrayMatrix;
		ShaderProperty* _projMatrix;

		ShaderProperty* _shadowSampler;
		ShaderProperty* _shadowTexture2D;
		ShaderProperty* _shadowTextureCube;
 
		// 2D texture used to direction light and spot light
		RenderTexture2D* _shadowDataTex2D;
		SurfaceView* _depthView2D;
		SurfaceView* _shaderView2D;
		RenderSurface* _depthSurface2D;

		// cube texture used to point light
		RenderTexture2D* _shadowDataTexCube;
		SurfaceView* _depthViewCube;
		SurfaceView* _shaderViewCube;

		RenderTexture2D* _shadowRenderTexCube;
		SurfaceView* _renderViewCube;
		SurfaceView* _shaderRenderViewCube;

		RenderSurface* _depthSurfaceCube;

		SamplerState* _shadowSamplerState;
	 
		unsigned int _width;
		unsigned int _height;
 
		RenderEffect* _DepthGenEffect;
		RenderEffect* _DepthCubeGenEffect;

		bool _bUseGeometryShader;
	};

}

#endif