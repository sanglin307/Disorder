#include "EngineInclude.h"

namespace Disorder
{
	Light::Light()
	{
		Type = LT_Parallel;
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;

		Intensity = 100.0f; 
		SpotAngle = 30;
		Range = 10.0f;
	}
}