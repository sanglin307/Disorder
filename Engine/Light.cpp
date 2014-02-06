#include "EngineInclude.h"

namespace Disorder
{

	Light::Light(std::string const& name)
		:Component(name,CT_Light)
	{
		LightType = LT_None;
		Color = Eigen::Vector3f::Constant(1.0f);
		Intensity = 0.8f; 
		CastShadows = false;
		ShadowColor = Eigen::Vector3f::Constant(0.f);
	}
 
	//////////////////////////////////////////////////////////////////////////////////

	Eigen::Vector3f DirectionLight::DefaultDirection(0.0f,0.0f,-1.0f);
 

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

	Eigen::Vector3f DirectionLight::GetDirection()
	{
		GameObjectPtr go = GetBase();
		return go->GetWorldRotation() * DirectionLight::DefaultDirection;
	}

	void DirectionLight::DebugDraw()
	{
		GameObjectPtr go = GetBase();
		Eigen::Vector3f beginPos = go->GetWorldPosition();
		Eigen::Vector3f endPos = beginPos + GetDirection() * 2;
			
		GEngine->GameCanvas->DrawLine(beginPos,Vector4f::ONE,endPos,Vector4f(0,0,0,1.f));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	PointLight::PointLight(std::string const& name)
		:Light(name)
	{
		LightType = LT_Point;
		Range = 10.0f;
	}

	Eigen::Vector3f PointLight::GetPosition()
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
		return Range * Range > (lightGo->GetWorldPosition() - renderGo->GetWorldPosition()).squaredNorm();
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

	Eigen::Vector3f SpotLight::GetPosition()
	{
		GameObjectPtr lightGo = GetBase();
		return lightGo->GetWorldPosition();
	}

	SpotLightPtr SpotLight::Create(std::string const& name)
	{
		SpotLight *pLight = new SpotLight(name);
		return SpotLightPtr(pLight);
	}

	Eigen::Vector3f SpotLight::GetDirection()
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
		 
		Eigen::Vector3f renderPos = renderGo->GetWorldPosition();
		Eigen::Vector3f lightPos = lightGo->GetWorldPosition();
		bool bRange = Range * Range > (lightPos - renderPos).squaredNorm();
		if( bRange == false )
			return false;

		Eigen::Vector3f renderDirect = renderPos - lightPos;
		Eigen::Vector3f Dir = GetDirection();
		float angle = Math::ACosf(Dir.dot(renderDirect)/(Dir.norm()*renderDirect.norm()));
		return angle < SpotOuterAngle;
 
	}
}