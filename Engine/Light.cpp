#include "EngineInclude.h"

namespace Disorder
{
	Vector3 Light::DefaultLightDirection(0,0,-1);

	Light::Light()
	{
		Type = CT_Light;
		LType = LT_Parallel;
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;

		Intensity = 0.8f; 
		SpotAngle = 30;
		Range = 10.0f;
	}

	Vector3 Light::GetDirection()
	{
		if( Type == LT_Parallel || Type == LT_Spot )
		{
			GameObjectPtr go = GetBase();
			return go->GetWorldRotation() * Light::DefaultLightDirection;
		}
		else
			return Vector3::ZERO;
	}

	bool Light::Touch(RendererPtr renderObject)
	{
		if( LType == LT_Parallel )
			return true;

		GameObjectPtr lightGo = GetBase();
		GameObjectPtr renderGo = renderObject->GetBase();
		if( LType == LT_Point )
		{
			return Range * Range > lightGo->GetWorldPosition().SquaredDistance(renderGo->GetWorldPosition());
		}
		
		if( LType == LT_Spot )
		{
			Vector3 renderPos = renderGo->GetWorldPosition();
			Vector3 lightPos = lightGo->GetWorldPosition();
			bool bRange = Range * Range > lightPos.SquaredDistance(renderPos);
			if( bRange == false )
				return false;

			float angle = GetDirection().AngleBetween(renderPos - lightPos);
			return angle * Math::fRad2Deg < SpotAngle;
		}

		return false;
	}
}