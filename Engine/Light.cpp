#include "EngineInclude.h"

namespace Disorder
{

	Light::Light(std::string const& name)
		:Component(name,CT_Light)
	{
		LightType = LT_None;
		Color = Vector3::UNIT_SCALE;
		Intensity = 0.8f; 
		CastShadows = false;
		ShadowColor = Vector3::ZERO;
	}
 
	//////////////////////////////////////////////////////////////////////////////////

	Vector3 DirectionLight::DefaultDirection(0.0f,0.0f,-1.0f);
 

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

	Vector3 DirectionLight::GetDirection()
	{
		GameObjectPtr go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	void DirectionLight::DebugDraw()
	{
		GameObjectPtr go = GetBase();
		Vector3 beginPos = go->GetWorldPosition();
		Vector3 endPos = beginPos + GetDirection();
			
		GEngine->GameCanvas->DrawLine(beginPos,Vector4::ONE,endPos,Vector4(Color));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	PointLight::PointLight(std::string const& name)
		:Light(name)
	{
		LightType = LT_Point;
		Range = 10.0f;
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
		return Range * Range > lightGo->GetWorldPosition().SquaredDistance(renderGo->GetWorldPosition());
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////

	SpotLight::SpotLight(std::string const& name)
		:Light(name)
	{
		LightType = LT_Spot;
		SpotAngle = 30;
		
	}

	SpotLightPtr SpotLight::Create(std::string const& name)
	{
		SpotLight *pLight = new SpotLight(name);
		return SpotLightPtr(pLight);
	}

	Vector3 SpotLight::GetDirection()
	{
		GameObjectPtr go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	bool SpotLight::Touch(RendererPtr renderObject)
	{
		GameObjectPtr lightGo = GetBase();
		GameObjectPtr renderGo = renderObject->GetBase();
		 
		Vector3 renderPos = renderGo->GetWorldPosition();
		Vector3 lightPos = lightGo->GetWorldPosition();
		bool bRange = Range * Range > lightPos.SquaredDistance(renderPos);
		if( bRange == false )
			return false;

		float angle = GetDirection().AngleBetween(renderPos - lightPos);
		return angle * Math::fRad2Deg < SpotAngle;
 
	}
}