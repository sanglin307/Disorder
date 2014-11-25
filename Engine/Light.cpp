#include "EngineInclude.h"

namespace Disorder
{

	Light::Light(std::string const& name)
		:Component(name, CT_Light), Color(glm::vec3(1.0f)), ShadowColor(glm::vec3(0.f))
	{
		LightType = LT_None;
		Intensity = 0.8f; 
		CastShadows = true;
	}
 
	//////////////////////////////////////////////////////////////////////////////////

	glm::vec3 DirectionLight::DefaultDirection(0.0f, 0.0f, -1.0f);
 

	DirectionLight::DirectionLight(std::string const& name)
		:Light(name)
	{
		LightType = LT_Directional;
		CastShadows = false;
	}

 
	bool DirectionLight::Touch(GeometryRenderer* renderObject)
	{
		return true;
	}
 
	bool DirectionLight::Overlaps(const Frustrum& frustrum)
	{
		return true;
	}

	void DirectionLight::CalculateShadowMatrix()
	{
		const BoxBounds& sceneBounds = GSceneManager->GetSceneBoundingBox();
		//// calc centriod of light box 
		glm::vec3 boxSize = sceneBounds.BMax - sceneBounds.BMin;
	
		glm::vec3 center = sceneBounds.GetCenter();
		float distance = glm::distance(center, sceneBounds.BMin);
		glm::vec3 dir = GetDirection();
		glm::vec3 lightPosition = center - (dir * distance);

		// create lights view matrix, so that it is looking right to the center of the bounding box 
		ShadowViewMatrix = Math::ViewMatrixRH(lightPosition, center, glm::vec3(0, 1, 0));
		std::vector<glm::vec3> points;
		sceneBounds.GetCorners(points);

		// bring light box points into light's view space             
		for (size_t i = 0; i < points.size(); i++)
			Math::Transform(ShadowViewMatrix, points[i]);
		
		glm::vec3 min;
		glm::vec3 max;
		Math::CalcMinMax(points,min,max);

		float clipDistance = Abs(max.z - min.z);

		//// create ortographic projection  You want to use -max.Z as your zNear, and -min.Z as your zFar.  Otherwise your zFar won't be large enough. 
		ShadowProjMatrix = Math::OrthoRH(max.x - min.x, max.y - min.y, 0, clipDistance);
	}

	glm::vec3 DirectionLight::GetDirection()
	{
		GameObject* go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	void DirectionLight::DebugDraw()
	{
		GameObject* go = GetBase();
		glm::vec3 beginPos = go->GetWorldPosition();
		glm::vec3 endPos = beginPos + GetDirection() * 2.f;
			
		GEngine->GameCanvas->DrawLine(beginPos, glm::vec4(1.f, 1.f, 1.f, 1.f), endPos, glm::vec4(0, 0, 0, 1.f));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	PointLight::PointLight(std::string const& name)
		:Light(name)
	{
		LightType = LT_Point;
		Range = 10.0f;


	}

	glm::vec3 PointLight::GetPosition()
	{
		GameObject* lightGo = GetBase();
		return lightGo->GetWorldPosition();
	}
	bool PointLight::Touch(GeometryRenderer* renderObject)
	{
		GameObject* lightGo = GetBase();
		GameObject* renderGo = renderObject->GetBase(); 
		float objRadius = renderObject->GetGeometry()->BoundingBox.SphereRadius;
		return (Range + objRadius) * (Range + objRadius) > glm::distance2(lightGo->GetWorldPosition(), renderGo->GetWorldPosition());
	}

	void PointLight::DebugDraw()
	{
	}

	bool PointLight::Overlaps(const Frustrum& frustrum)
	{
		GameObject* lightGo = GetBase();
		SphereBounds bound(lightGo->GetWorldPosition(),Range);
		return frustrum.Overlaps(bound);
	}

	void PointLight::CalculateShadowMatrix()
	{
		GameObject* lightGo = GetBase();
		glm::vec3 lightPos = lightGo->GetWorldPosition();
	
		// +X
		glm::vec3 vLookDir(lightPos.x + 1,lightPos.y,lightPos.z);
		glm::vec3 vUpDir(0,1,0);
		ShadowViewMatrix[0] = Math::ViewMatrixRH(lightPos, vLookDir, vUpDir);
		
		// -X
		vLookDir = glm::vec3(lightPos.x - 1, lightPos.y, lightPos.z);
		ShadowViewMatrix[1] = Math::ViewMatrixRH(lightPos, vLookDir, vUpDir);

		// +Y
		vLookDir = glm::vec3(lightPos.x, lightPos.y + 1, lightPos.z);
		vUpDir = glm::vec3(0,0,1);
		ShadowViewMatrix[2] = Math::ViewMatrixRH(lightPos, vLookDir, vUpDir);

		// -Y
		vLookDir = glm::vec3(lightPos.x, lightPos.y - 1, lightPos.z);
		vUpDir = glm::vec3(0, 0, 1);
		ShadowViewMatrix[3] = Math::ViewMatrixRH(lightPos, vLookDir, vUpDir);

		// +Z
		vLookDir = glm::vec3(lightPos.x, lightPos.y, lightPos.z + 1);
		vUpDir = glm::vec3(0, 1, 0);
		ShadowViewMatrix[4] = Math::ViewMatrixRH(lightPos, vLookDir, vUpDir);

		// -Z
		vLookDir = glm::vec3(lightPos.x, lightPos.y, lightPos.z - 1);
		vUpDir = glm::vec3(0, 1, 0);
		ShadowViewMatrix[5] = Math::ViewMatrixRH(lightPos, vLookDir, vUpDir);

		ShadowProjMatrix = Math::ProjFovRH(Math::HALF_PI, 1, 0.01f, Range);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////

	SpotLight::SpotLight(std::string const& name)
		:Light(name)
	{
		LightType = LT_Spot;
		SpotInnerAngle = 30 * Math::fDeg2Rad;
		SpotOuterAngle = 90 * Math::fDeg2Rad;
		
	}

	glm::vec3 SpotLight::GetPosition()
	{
		GameObject* lightGo = GetBase();
		return lightGo->GetWorldPosition();
	}

	glm::vec3 SpotLight::GetDirection()
	{
		GameObject* go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	void SpotLight::DebugDraw()
	{
	}

	bool SpotLight::Touch(GeometryRenderer* renderObject)
	{
		GameObject* lightGo = GetBase();
		GameObject* renderGo = renderObject->GetBase();
		 
		glm::vec3 renderPos = renderGo->GetWorldPosition();
		glm::vec3 lightPos = lightGo->GetWorldPosition();
		float objRadius = renderObject->GetGeometry()->BoundingBox.SphereRadius;
		bool bRange = (objRadius + Range) * (objRadius + Range) > glm::distance2(lightPos, renderPos);
		if( bRange == false )
			return false;

		return true;
	}

	bool SpotLight::Overlaps(const Frustrum& frustrum)
	{
		//rough calculate
		GameObject* lightGo = GetBase();
		float BoundsRadius = Math::Sqrtf(1.25f * Range * Range - Range * Range * SpotOuterAngle);
		SphereBounds bound(lightGo->GetWorldPosition() + .5f * GetDirection() * Range, BoundsRadius);

		return frustrum.Overlaps(bound);
	}

	void SpotLight::CalculateShadowMatrix()
	{
		GameObject* lightGo = GetBase();
		glm::vec3 lightPos = lightGo->GetWorldPosition();
		glm::vec3 Dir = GetDirection();

		ShadowViewMatrix = Math::ViewMatrixRH(lightPos, lightPos + Dir, glm::vec3(0, 1, 0));
		ShadowProjMatrix = Math::ProjFovRH(SpotOuterAngle, 1, 0.1f, Range);
	}
}