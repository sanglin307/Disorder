#ifndef _DISORDER_LIGHT_H_
#define _DISORDER_LIGHT_H_

namespace Disorder
{

	enum ELightType
	{
		LT_None = 0,
		LT_Directional,
		LT_Point,
		LT_Spot
	};

	enum ELightDecayType
	{
		LDT_None = 0,
		LDT_Linear,
		LDT_Quadratic,
		LDT_Cubic
	};

	class Light : public Component
	{
	public:
		
		ELightType LightType;
		Eigen::Vector3f   Color;
		float     Intensity; 
		bool      CastShadows;
		Eigen::Vector3f   ShadowColor;
		ELightDecayType DecayType;

		virtual bool Touch(RendererPtr renderObject) = 0;
		virtual void DebugDraw(){};
		
	protected:
		Light(std::string const& name);
	};

	class DirectionLight : public Light
	{
	protected:
		DirectionLight(std::string const& name);
		
	public:
		static Eigen::Vector3f DefaultDirection;

		static DirectionLightPtr Create(std::string const& name);
		Eigen::Vector3f GetDirection();
		virtual bool Touch(RendererPtr renderObject);
		virtual void DebugDraw();
	};

	class PointLight : public Light
	{
	protected:
		PointLight(std::string const& name);
	
	public:
		float Range;
		Eigen::Vector3f GetPosition();
		static PointLightPtr Create(std::string const& name);
		virtual bool Touch(RendererPtr renderObject);
        virtual void DebugDraw();
	};

	class SpotLight : public Light
	{
	protected:
		SpotLight(std::string const& name);

	public:
		float Range;
		float SpotInnerAngle;
		float SpotOuterAngle;
		Eigen::Vector3f GetDirection();
		Eigen::Vector3f GetPosition();
		static SpotLightPtr Create(std::string const& name);
		virtual bool Touch(RendererPtr renderObject);
		virtual void DebugDraw();
 
	};
}






#endif