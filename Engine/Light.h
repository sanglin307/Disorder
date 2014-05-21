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
 
		glm::mat4 ShadowViewMatrix;
		glm::mat4 ShadowProjMatrix;

		virtual bool Touch(RendererPtr renderObject) = 0;
		virtual bool Overlaps(const Frustrum& frustrum) = 0;
		 

		virtual void DebugDraw(){};
		 

		//visible bounding is the camera view visible ones
		virtual void UpdateVisibleBounding(const BoxBounds& bb){};

	protected:
		Light(std::string const& name);
	};

	class DirectionLight : public Light
	{
	protected:
		DirectionLight(std::string const& name);
		
	public:
		static glm::vec3 DefaultDirection;

		static DirectionLightPtr Create(std::string const& name);
		glm::vec3 GetDirection();
		virtual bool Touch(RendererPtr renderObject);
		virtual bool Overlaps(const Frustrum& frustrum);
		virtual void DebugDraw();

		virtual void UpdateVisibleBounding(const BoxBounds& bb);
	};

	class PointLight : public Light
	{
	protected:
		PointLight(std::string const& name);
	
	public:
		float Range;
		glm::vec3 GetPosition();
		static PointLightPtr Create(std::string const& name);
		virtual bool Touch(RendererPtr renderObject);
		virtual bool Overlaps(const Frustrum& frustrum);
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
		glm::vec3 GetDirection();
		glm::vec3 GetPosition();
		static SpotLightPtr Create(std::string const& name);
		virtual bool Touch(RendererPtr renderObject);
		virtual bool Overlaps(const Frustrum& frustrum);
		virtual void DebugDraw();
 
	};
}






#endif