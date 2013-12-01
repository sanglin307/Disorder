#ifndef _DISORDER_LIGHT_H_
#define _DISORDER_LIGHT_H_

namespace Disorder
{

	enum ELightType
	{
		LT_Directional = 0,
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
		float     SpotAngle;
		float     Range;
		bool      CastShadows;
		Vector3   ShadowColor;

		ELightDecayType DecayType;

		Vector3 GetDirection();

		bool Touch(RendererPtr renderObject);

		static Vector3 DefaultLightDirection;

		 static LightPtr Create(std::string const& name);
	protected:
		Light(std::string const& name);
	};
}






#endif