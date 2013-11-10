#ifndef _DISORDER_LIGHT_H_
#define _DISORDER_LIGHT_H_

namespace Disorder
{

	enum ELightType
	{
		LT_Parallel = 0,
		LT_Point,
		LT_Spot
	};

	class Light : public Component
	{
	public:
		Light();
		ELightType LightType;
		Vector3   Color;
		float     Intensity; 
		float     SpotAngle;
		float     Range;

		Vector3 GetDirection();

		bool Touch(RendererPtr renderObject);

		static Vector3 DefaultLightDirection;
	};
}






#endif