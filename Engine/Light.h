#ifndef _DISORDER_LIGHT_H_
#define _DISORDER_LIGHT_H_

namespace Disorder
{

	enum LightType
	{
		LT_Parallel = 0,
		LT_Point,
		LT_Spot
	};

	class Light : public Component
	{
	public:
		Light();
		LightType Type;
		Vector3   Color;
		float     Intensity; 
		float     SpotAngle;
		float     Range;

		static Vector3 DirectLight;
	};
}






#endif