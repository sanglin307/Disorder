#ifndef _DISORDER_LIGHT_H_
#define _DISORDER_LIGHT_H_

namespace Disorder
{

	enum ELightType
	{
		LT_None = 0,
		LT_Directional,
		LT_Point,
		LT_Spot
	};

	enum ELightDecayType
	{
		LDT_None = 0,
		LDT_Linear,
		LDT_Quadratic,
		LDT_Cubic
	};

	class Light : public Component
	{
	public:
		
		ELightType LightType;
		Vector3   Color;
		float     Intensity; 
		bool      CastShadows;
		Vector3   ShadowColor;
		ELightDecayType DecayType;

		virtual bool Touch(RendererPtr renderObject) = 0;
		
	protected:
		Light(std::string const& name);
	};

	class DirectionLight : public Light
	{
	protected:
		DirectionLight(std::string const& name);
		
	public:
		static Vector3 DefaultDirection;

		static DirectionLightPtr Create(std::string const& name);
		Vector3 GetDirection();
		virtual bool Touch(RendererPtr renderObject);
		void DebugDraw();
	};

	class PointLight : public Light
	{
	protected:
		PointLight(std::string const& name);
	
	public:
		float Range;
		static PointLightPtr Create(std::string const& name);
		virtual bool Touch(RendererPtr renderObject);
 
	};

	class SpotLight : public Light
	{
	protected:
		SpotLight(std::string const& name);

	public:
		float Range;
		float SpotAngle;
		Vector3 GetDirection();
		static SpotLightPtr Create(std::string const& name);
		virtual bool Touch(RendererPtr renderObject);
 
	};
}






#endif