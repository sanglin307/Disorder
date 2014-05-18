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

		void PrepareRender(const glm::mat4& wldMat, const glm::mat4& viewMat, const glm::mat4& projMat);

	protected:
		ShadowMap(unsigned int width,unsigned int height);

		ShaderPropertyManagerPtr _propertyMgr;
		ShaderPropertyPtr _wldMatrix;
		ShaderPropertyPtr _viewMatrix;
		ShaderPropertyPtr _projMatrix;
 
		RenderTexture2DPtr _shadowDataTex;
		SurfaceViewPtr _targetView;
		SurfaceViewPtr _shaderView;
		RenderSurfacePtr _renderTarget;
		unsigned int _width;
		unsigned int _height;

		static SurfaceViewPtr sDepthBufferView;
		static RenderEffectPtr sDepthGenEffect;
	};

}

#endif