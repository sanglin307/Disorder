#ifndef _DISORDER_LIGHT_H_
#define _DISORDER_LIGHT_H_

namespace Disorder
{

	enum LightType
	{
		LT_Point,
		LT_Parallel,
		LT_Spot
	};

	class Light
	{
	public:
		LightType Type;
		Vector3   LightPos;
		Vector3   LightDirection;
		Vector4   Ambient;
		Vector4   Diffuse;
		Vector4   Spec;
		Vector3   Attenuate;  // for point light 
		float     SpotPower;
		float     Range;
	};
}






#endif