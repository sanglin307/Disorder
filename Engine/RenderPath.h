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
		
		void SetDirectionLight(DirectionLight* directionLight);
		void SetPointLight(PointLight* pointLight);
		void SetSpotLight(SpotLight* spotLight);

		ShaderPropertyManager* _DirectionLightPropertyManager;
		ShaderProperty* _DirectionLightIntensityProperty;
		ShaderProperty* _DirectionLightDirProperty;
		ShaderProperty* _DirectionLightColorProperty;
		ShaderProperty* _DirectionLightCastShadowProperty;

		ShaderPropertyManager* _PointLightPropertyManager;
		ShaderProperty* _PointLightPosProperty;
		ShaderProperty* _PointLightColorProperty;
		ShaderProperty* _PointLightIntensityProperty;
		ShaderProperty* _PointLightRangeRcpProperty;
		ShaderProperty* _PointLightCastShadowProperty;

		ShaderPropertyManager* _SpotLightPropertyManager;
		ShaderProperty* _SpotLightPosProperty;
		ShaderProperty* _SpotLightDirProperty;
		ShaderProperty* _SpotLightColorProperty;
		ShaderProperty* _SpotLightIntensityProperty;
		ShaderProperty* _SpotLightRangeRcpProperty;
		ShaderProperty* _SpotLightCosOuterConeProperty;
		ShaderProperty* _SpotLightCosInnerConeProperty;
		ShaderProperty* _SpotLightCastShadowProperty;
 
	 
		RenderPathType _type;

		FXAA* _aaRender;

	};

	class ForwardRenderPath : public RenderPath
	{
	public:
		virtual void Render();
		ForwardRenderPath();
	protected:
		void BasePassRender(Camera* camera, const std::vector<GeometryRenderer*>& renderList);
		void RenderLights(Camera* camera, const std::vector<GeometryRenderer*>& renderList);
		RenderEffect* _BasePassEffect;
		RenderEffect* _DirectionLightEffect;
		RenderEffect* _PointLightEffect;
		RenderEffect* _SpotLightEffect;
	};

	class DeferredShading : public RenderPath
	{
	public:
	 
		virtual void Render();
		DeferredShading();
	protected:
		void RenderScene(Camera* mainCamera, const std::vector<GeometryRenderer*>& rendererList);
		void RenderLights(Camera* camera, const std::vector<GeometryRenderer*>& renderList);

		RenderEffect* _RenderSceneEffect;

		SimpleTile _LightingTile;
		RenderEffect* _BasePassEffect;
		RenderEffect* _DirectionLightEffect;
		RenderEffect* _PointLightEffect;
		RenderEffect* _SpotLightEffect;
	};

}


#endif