#include "EngineInclude.h"

namespace Disorder
{
	Vector3 Light::DirectLight = Vector3::NEGATIVE_UNIT_Z;

	Light::Light()
	{
		Type = LT_Parallel;
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;

		Intensity = 0.8f; 
		SpotAngle = 30;
		Range = 10.0f;
	}

	Vector3 Light::GetDirect()
	{
		if( Type == LT_Parallel )
		{
			GameObjectPtr go = GetBase();
			return go->GetRotation() * Light::DirectLight;
		}
		else
			return Vector3::ZERO;
	}
}