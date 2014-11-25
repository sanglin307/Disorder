#ifndef _DISORDER_POSTPROCESS_H_
#define _DISORDER_POSTPROCESS_H_


namespace Disorder
{
	class FXAA
	{
	public:
		FXAA();
		void Render(Camera* camera, SurfaceView* aaRenderView);
	private:
		SimpleTile _ScreenTile;
		ShaderProperty* _ScreenAATexture;
		ShaderProperty* _ScreenAASampler;

		RenderEffect* _AAPrepareEffect;
		RenderEffect* _AAEffect;

		RenderTexture2D* _PrepareBufferTexture;
		RenderSurface* _PrepareBufferSurface;
		SurfaceView* _PrepareShaderResource;

		RenderTexture2D* _TargetBufferTexture;
		RenderSurface* _TargetBufferSurface;
		SurfaceView* _TargetShaderResource;

	};

}



#endif