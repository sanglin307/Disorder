#ifndef _DISORDER_SKYBOX_H_
#define _DISORDER_SKYBOX_H_


namespace Disorder
{
	class Skybox
	{
	public:
		void Render();
		static SkyboxPtr Create();
	private:
		Skybox();

		RenderTexture2DPtr _skyTexture;
		SurfaceViewPtr _skyCubeView;
		SamplerStatePtr _skyCubeSampler;

		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;

		ShaderPropertyPtr _skyTextureProperty;
		ShaderPropertyPtr _skySamplerProperty;
	};
}



#endif