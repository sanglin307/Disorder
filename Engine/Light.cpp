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
	}

	DirectionLightPtr DirectionLight::Create(std::string const& name)
	{
		DirectionLight *pLight = new DirectionLight(name);
		return DirectionLightPtr(pLight);
	}

	bool DirectionLight::Touch(RendererPtr renderObject)
	{
		return true;
	}
 
	bool DirectionLight::Overlaps(const Frustrum& frustrum)
	{
		return true;
	}

	void DirectionLight::UpdateVisibleBounding(const BoxBounds& bb)
	{
		//// calc centriod of light box 
		glm::vec3 boxSize = bb.BMax - bb.BMin;
		//Vector3 centroid = lightBox.Min + (boxSize / 2.0f);
		glm::vec3 center = bb.GetCenter();
		//// calc distance of centroid to one of the box's corners 
		float distance = glm::distance(center, bb.BMin);
		glm::vec3 dir = GetDirection();
		glm::vec3 lightPosition = center - (dir * distance);

		// create lights view matrix, so that it is looking right to the center of the bounding box 
		ShadowViewMatrix = Math::ViewMatrixRH(lightPosition, center, glm::vec3(0, 1, 0));
		std::vector<glm::vec3> points;
		bb.GetCorners(points);

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
		GameObjectPtr go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	void DirectionLight::DebugDraw()
	{
		GameObjectPtr go = GetBase();
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
		GameObjectPtr lightGo = GetBase();
		return lightGo->GetWorldPosition();
	}

	PointLightPtr PointLight::Create(std::string const& name)
	{
		PointLight *pLight = new PointLight(name);
		return PointLightPtr(pLight);
	}

	bool PointLight::Touch(RendererPtr renderObject)
	{
		GameObjectPtr lightGo = GetBase();
		GameObjectPtr renderGo = renderObject->GetBase(); 
		return Range * Range > glm::distance2(lightGo->GetWorldPosition(),renderGo->GetWorldPosition());
	}

	void PointLight::DebugDraw()
	{
	}

	bool PointLight::Overlaps(const Frustrum& frustrum)
	{
		GameObjectPtr lightGo = GetBase();
		SphereBounds bound(lightGo->GetWorldPosition(),Range);
		return frustrum.Overlaps(bound);
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
		GameObjectPtr lightGo = GetBase();
		return lightGo->GetWorldPosition();
	}

	SpotLightPtr SpotLight::Create(std::string const& name)
	{
		SpotLight *pLight = new SpotLight(name);
		return SpotLightPtr(pLight);
	}

	glm::vec3 SpotLight::GetDirection()
	{
		GameObjectPtr go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	void SpotLight::DebugDraw()
	{
	}

	bool SpotLight::Touch(RendererPtr renderObject)
	{
		GameObjectPtr lightGo = GetBase();
		GameObjectPtr renderGo = renderObject->GetBase();
		 
		glm::vec3 renderPos = renderGo->GetWorldPosition();
		glm::vec3 lightPos = lightGo->GetWorldPosition();
		bool bRange = Range * Range > glm::distance2(lightPos,renderPos);
		if( bRange == false )
			return false;

		glm::vec3 renderDirect = renderPos - lightPos;
		glm::vec3 Dir = GetDirection();
		float angle = glm::angle(Dir, renderDirect);  
		return angle < SpotOuterAngle;
	}

	bool SpotLight::Overlaps(const Frustrum& frustrum)
	{
		//rough calculate
		GameObjectPtr lightGo = GetBase();
		float BoundsRadius = Math::Sqrtf(1.25f * Range * Range - Range * Range * SpotOuterAngle);
		SphereBounds bound(lightGo->GetWorldPosition() + .5f * GetDirection() * Range, BoundsRadius);

		return frustrum.Overlaps(bound);
	}
}