#ifndef _DISORDER_SKYBOX_H_
#define _DISORDER_SKYBOX_H_


namespace Disorder
{
	class Skybox
	{
	public:
		void Render();
		Skybox();
	private:
		RenderTexture2D* _skyTexture;
		SurfaceView* _skyCubeView;
		SamplerState* _skyCubeSampler;

		RenderEffect* _renderEffect;
		RenderLayout* _renderLayout;

		ShaderProperty* _skyTextureProperty;
		ShaderProperty* _skySamplerProperty;
	};
}



#endif