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

		void PrepareRenderDepth(const glm::mat4& viewMat, const glm::mat4& projMat);
		void RenderObject(const CameraPtr& camera, const GeometryRendererPtr object);
		void PrepareRenderLight(const LightPtr& light);
		void UpdateShaderProperty();

	protected:
		ShadowMap(unsigned int width,unsigned int height);

		ShaderPropertyManagerPtr _propertyMgr;

		ShaderPropertyPtr _viewMatrix;
		ShaderPropertyPtr _projMatrix;
		ShaderPropertyPtr _shadowSampler;
		ShaderPropertyPtr _shadowTexture2D;
 
		RenderTexture2DPtr _shadowDataTex;
		SurfaceViewPtr _depthView;
		SurfaceViewPtr _shaderView;
		SamplerStatePtr _shadowSamplerState;
	 
		unsigned int _width;
		unsigned int _height;

		glm::mat4 _viewMat;
		glm::mat4 _projMat;

		static RenderEffectPtr sDepthGenEffect;
		 
	};

}

#endif