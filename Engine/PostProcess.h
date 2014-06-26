#ifndef _DISORDER_POSTPROCESS_H_
#define _DISORDER_POSTPROCESS_H_


namespace Disorder
{
	class FXAA
	{
	public:
		static FXAAPtr Create();
		void Render(const CameraPtr camera, const SurfaceViewPtr aaRenderView);

	private:
		FXAA();

		SimpleTile _ScreenTile;
		ShaderPropertyPtr _ScreenAATexture;
		ShaderPropertyPtr _ScreenAASampler;

		RenderEffectPtr _AAPrepareEffect;
		RenderEffectPtr _AAEffect;

		RenderTexture2DPtr _PrepareBufferTexture;
		RenderSurfacePtr _PrepareBufferSurface;
		SurfaceViewPtr _PrepareShaderResource;

		RenderTexture2DPtr _TargetBufferTexture;
		RenderSurfacePtr _TargetBufferSurface;
		SurfaceViewPtr _TargetShaderResource;

	};

}



#endif