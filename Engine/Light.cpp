#include "EngineInclude.h"

namespace Disorder
{

	Light::Light(std::string const& name)
		:Component(name,CT_Light)
	{
		LightType = LT_None;
		Color = glm::vec3(1.0f);
		Intensity = 0.8f; 
		CastShadows = false;
		ShadowColor = glm::vec3(0.f);
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
}