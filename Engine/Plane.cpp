#include "EngineInclude.h"


namespace Disorder
{
	//-----------------------------------------------------------------------
	Plane::Plane ()
	{
		Normal = glm::vec3(0);
		D = 0.0f;
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Plane& rhs)
	{
		Normal = rhs.Normal;
		D = rhs.D;
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const glm::vec3& rkNormal, float fConstant)
	{
		Normal = rkNormal;
		D = -fConstant;
	}
	//---------------------------------------------------------------------
	Plane::Plane (float a, float b, float c, float _d)
		: Normal(a, b, c), D(_d)
	{
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const glm::vec3& rkNormal, const glm::vec3& rkPoint)
	{
		Redefine(rkNormal, rkPoint);
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const glm::vec3& rkPoint0, const glm::vec3& rkPoint1,
		const glm::vec3& rkPoint2)
	{
		Redefine(rkPoint0, rkPoint1, rkPoint2);
	}
	//-----------------------------------------------------------------------
	float Plane::GetDistance(const glm::vec3& rkPoint) const
	{
		return glm::dot(Normal,rkPoint) + D;
	}
	//-----------------------------------------------------------------------
	Plane::Side Plane::GetSide(const glm::vec3& rkPoint) const
	{
		float fDistance = GetDistance(rkPoint);

		if ( fDistance < 0.0 )
			return Plane::NEGATIVE_SIDE;

		if ( fDistance > 0.0 )
			return Plane::POSITIVE_SIDE;

		return Plane::NO_SIDE;
	}


	//-----------------------------------------------------------------------
	Plane::Side Plane::GetSide (const BoxBounds& box) const
	{
		if( box.IsValid() == false )
			return Plane::NO_SIDE;

        return GetSide(box.GetCenter(), box.GetExtent());
	}
    //-----------------------------------------------------------------------
	Plane::Side Plane::GetSide(const glm::vec3& centre, float fRadius) const
	{
		 float dist = GetDistance(centre);
		 if( dist >= fRadius )
			 return Plane::POSITIVE_SIDE;

		 if( dist <= 0-fRadius )
			 return Plane::NEGATIVE_SIDE;

		 return Plane::BOTH_SIDE;
	}

	Plane::Side Plane::GetSide(const glm::vec3& centre, const glm::vec3& halfSize) const
    {
        // Calculate the distance between box centre and the plane
        float dist = GetDistance(centre);

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
		float maxAbsDist = abs(Normal.x * halfSize.x) + abs(Normal.y * halfSize.y) + abs(Normal.z * halfSize.z);

        if (dist < -maxAbsDist)
            return Plane::NEGATIVE_SIDE;

        if (dist > +maxAbsDist)
            return Plane::POSITIVE_SIDE;

        return Plane::BOTH_SIDE;
    }
	//-----------------------------------------------------------------------
	void Plane::Redefine(const glm::vec3& rkPoint0, const glm::vec3& rkPoint1,
		const glm::vec3& rkPoint2)
	{
		glm::vec3 kEdge1 = rkPoint1 - rkPoint0;
		glm::vec3 kEdge2 = rkPoint2 - rkPoint0;
		Normal = glm::cross(kEdge1,kEdge2);
		Normal = glm::normalize(Normal);
		D = glm::dot(-Normal,rkPoint0);
	}
	//-----------------------------------------------------------------------
	void Plane::Redefine(const glm::vec3& rkNormal, const glm::vec3& rkPoint)
	{
		Normal = rkNormal;
		D = glm::dot(-rkNormal,rkPoint);
	}
	//-----------------------------------------------------------------------
	glm::vec3 Plane::ProjectVector(const glm::vec3& p) const
	{
		// We know plane normal is unit length, so use simple method
		glm::mat3 xform;
		xform[0][0] = 1.0f - Normal.x * Normal.x;
		xform[0][1] = -Normal.x * Normal.y;
		xform[0][2] = -Normal.x * Normal.z;
		xform[1][0] = -Normal.y * Normal.x;
		xform[1][1] = 1.0f - Normal.y * Normal.y;
		xform[1][2] = -Normal.y * Normal.z;
		xform[2][0] = -Normal.z * Normal.x;
		xform[2][1] = -Normal.z * Normal.y;
		xform[2][2] = 1.0f - Normal.z * Normal.z;
		return xform * p;

	}
	//-----------------------------------------------------------------------
    float Plane::Normalise(void)
    {
        float fLength = glm::length(Normal);

        if ( fLength > float(0.0f) )
        {
            float fInvLength = 1.0f / fLength;
            Normal *= fInvLength;
            D *= fInvLength;
        }

        return fLength;
    }
}