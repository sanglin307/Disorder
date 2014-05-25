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
		void SetDirectionLight(const DirectionLightPtr& directionLight);
		void SetFourLight(const std::vector<LightPtr>& lightArray);
		void SetPointLight(const PointLightPtr& pointLight);
		void SetSpotLight(const SpotLightPtr& spotLight);

		ShaderPropertyManagerPtr _DirectionLightPropertyManager;
		ShaderPropertyPtr _DirectionLightIntensityProperty;
		ShaderPropertyPtr _DirectionLightDirProperty;
		ShaderPropertyPtr _DirectionLightColorProperty;

		ShaderPropertyManagerPtr _PointLightPropertyManager;
		ShaderPropertyPtr _PointLightPosProperty;
		ShaderPropertyPtr _PointLightColorProperty;
		ShaderPropertyPtr _PointLightIntensityProperty;
		ShaderPropertyPtr _PointLightRangeRcpProperty;

		ShaderPropertyManagerPtr _SpotLightPropertyManager;
		ShaderPropertyPtr _SpotLightPosProperty;
		ShaderPropertyPtr _SpotLightDirProperty;
		ShaderPropertyPtr _SpotLightColorProperty;
		ShaderPropertyPtr _SpotLightIntensityProperty;
		ShaderPropertyPtr _SpotLightRangeRcpProperty;
		ShaderPropertyPtr _SpotLightCosOuterConeProperty;
		ShaderPropertyPtr _SpotLightCosInnerConeRcpProperty;
 
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
 
		void BasePassRender(const CameraPtr& camera, const std::vector<GeometryRendererPtr>& renderList);
		void RenderLights(const CameraPtr& camera, const std::vector<GeometryRendererPtr>& renderList);
		RenderEffectPtr _BasePassEffect;
		RenderEffectPtr _DirectionLightEffect;
		RenderEffectPtr _PointLightEffect;
		RenderEffectPtr _SpotLightEffect;
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