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
		glm::vec3   Color;
		float     Intensity; 
		bool      CastShadows;
		glm::vec3   ShadowColor;
		ELightDecayType DecayType;

		virtual bool Touch(GeometryRendererPtr renderObject) = 0;
		virtual bool Overlaps(const Frustrum& frustrum) = 0;
		virtual void DebugDraw(){};
		virtual void CalculateShadowMatrix() = 0;

	protected:
		Light(std::string const& name);
	};

	class DirectionLight : public Light
	{
	protected:
		DirectionLight(std::string const& name);
		
	public:
		static glm::vec3 DefaultDirection;

		glm::mat4 ShadowViewMatrix;
		glm::mat4 ShadowProjMatrix;

		static DirectionLightPtr Create(std::string const& name);
		glm::vec3 GetDirection();
		virtual bool Touch(GeometryRendererPtr renderObject);
		virtual bool Overlaps(const Frustrum& frustrum);
		virtual void DebugDraw();

		virtual void CalculateShadowMatrix();
	};

	class PointLight : public Light
	{
	protected:
		PointLight(std::string const& name);
	
	public:

		glm::mat4 ShadowViewMatrix[6];
		glm::mat4 ShadowProjMatrix;

		float Range;
		glm::vec3 GetPosition();
		static PointLightPtr Create(std::string const& name);
		virtual bool Touch(GeometryRendererPtr renderObject);
		virtual bool Overlaps(const Frustrum& frustrum);
        virtual void DebugDraw();
		virtual void CalculateShadowMatrix();
	};

	class SpotLight : public Light
	{
	protected:
		SpotLight(std::string const& name);

	public:

		glm::mat4 ShadowViewMatrix;
		glm::mat4 ShadowProjMatrix;

		float Range;
		float SpotInnerAngle;
		float SpotOuterAngle;
		glm::vec3 GetDirection();
		glm::vec3 GetPosition();
		static SpotLightPtr Create(std::string const& name);
		virtual bool Touch(GeometryRendererPtr renderObject);
		virtual bool Overlaps(const Frustrum& frustrum);
		virtual void CalculateShadowMatrix();
		virtual void DebugDraw();
 
	};
}






#endif