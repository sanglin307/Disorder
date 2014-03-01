#ifndef _DISORDER_RENDERPATH_H_
#define _DISORDER_RENDERPATH_H_


namespace Disorder
{
	class RenderPath
	{
	public:
		virtual void Render() = 0;

		RenderPathType GetType()
		{
			return _type;
		}

	protected:

		RenderPath();
		void SetDirectionLight(const std::vector<DirectionLightPtr>& directionLightArray);
		void SetFourLight(const std::vector<LightPtr>& lightArray);

		ShaderPropertyManagerPtr _DirectionLightPropertyManager;
		ShaderPropertyPtr _DirectionLightIntensityProperty;
		ShaderPropertyPtr _DirectionLightDirProperty;
		ShaderPropertyPtr _DirectionLightColorProperty;

		ShaderPropertyManagerPtr _LightFourPropertyManager;
		ShaderPropertyPtr ForwardLightPosX;
		ShaderPropertyPtr ForwardLightPosY;
		ShaderPropertyPtr ForwardLightPosZ;
		ShaderPropertyPtr ForwardLightDirX;
		ShaderPropertyPtr ForwardLightDirY;
		ShaderPropertyPtr ForwardLightDirZ;
		ShaderPropertyPtr ForwardLightRangeRcp;
		ShaderPropertyPtr ForwardSpotCosOuterCone;
		ShaderPropertyPtr ForwardSpotCosInnerConeRcp;
		ShaderPropertyPtr ForwardCapsuleLen;
		ShaderPropertyPtr ForwardLightColorR;
		ShaderPropertyPtr ForwardLightColorG;
		ShaderPropertyPtr ForwardLightColorB;

		RenderPathType _type;

	};

	class ForwardRenderPath : public RenderPath
	{
	public:
		virtual void Render();
		static ForwardRenderPathPtr Create();

	protected:

		ForwardRenderPath();
 
		RenderEffectPtr _DirectionLightEffect;
		RenderEffectPtr _FourLightEffect;

	};

	class DeferredShading : public RenderPath
	{
	public:
	 
		virtual void Render();
		static DeferredShadingPtr Create();
	protected:
		DeferredShading();
        void RenderScene(const CameraPtr& mainCamera);

		RenderEffectPtr _RenderSceneEffect;

		SimpleTile _LightingTile;
		RenderEffectPtr _LightingEffect;
		RenderEffectPtr _FourLightEffect;
	};

}


#endif