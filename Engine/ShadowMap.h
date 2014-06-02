#ifndef _DISORDER_SHADOWMAP_H_
#define _DISORDER_SHADOWMAP_H_

namespace Disorder
{
	class ShadowMap
	{
	public:
		static ShadowMapPtr Create(unsigned int width,unsigned int height);

		const unsigned int GetWidth() const
		{
			return _width;
		}

		const unsigned int GetHeight() const
		{
			return _height;
		}

		void PrepareRenderDepth(const LightPtr& light);
		void RenderObject(const CameraPtr& camera, const LightPtr& light, const GeometryRendererPtr object);
		void PrepareRenderLight(const LightPtr& light);
	 
	protected:
		ShadowMap(unsigned int width,unsigned int height);

		ShaderPropertyManagerPtr _propertyMgr;

		ShaderPropertyPtr _viewMatrix;
		ShaderPropertyPtr _viewArrayMatrix;
		ShaderPropertyPtr _projMatrix;

		ShaderPropertyPtr _shadowSampler;
		ShaderPropertyPtr _shadowTexture2D;
		ShaderPropertyPtr _shadowTextureCube;
 
		// 2D texture used to direction light and spot light
		RenderTexture2DPtr _shadowDataTex2D;
		SurfaceViewPtr _depthView2D;
		SurfaceViewPtr _shaderView2D;

		// cube texture used to point light
		RenderTexture2DPtr _shadowDataTexCube;
		SurfaceViewPtr _depthViewCube;
		SurfaceViewPtr _shaderViewCube;

		SamplerStatePtr _shadowSamplerState;
	 
		unsigned int _width;
		unsigned int _height;
 
		RenderEffectPtr _DepthGenEffect;
		RenderEffectPtr _DepthCubeGenEffect;
	};

}

#endif