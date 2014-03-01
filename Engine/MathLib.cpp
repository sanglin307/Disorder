#include "EngineInclude.h"

namespace Disorder
{
	const float Math::POS_INFINITY = std::numeric_limits<float>::infinity();
    const float Math::NEG_INFINITY = -std::numeric_limits<float>::infinity();
	const float Math::PI = float( 4.0 * atan( 1.0 ) );
    const float Math::TWO_PI = float( 2.0 * PI );
    const float Math::HALF_PI = float( 0.5 * PI );
	const float Math::fDeg2Rad = PI / float(180.0);
	const float Math::fRad2Deg = float(180.0) / PI;

 
	void Math::ConvertToSphericalCoord(const glm::vec3& pos,const glm::vec3& origin,float &radius,float &yAngle,float &zAngle)
	{
		glm::vec3 deltaVec = pos - origin;
		radius = glm::length(deltaVec);
		yAngle = ACosf(deltaVec.y/radius);
		zAngle = ATan2f(deltaVec.x,deltaVec.z);
	}

	void Math::ConvertFromSphericalCoord(float radius,float yAngle,float zAngle,const glm::vec3& origin,glm::vec3& pos)
	{
		BOOST_ASSERT(radius > 0);
		float temp = radius * Sinf(yAngle);

		pos.z = temp * Cosf(zAngle);
		pos.x = temp * Sinf(zAngle);
		pos.y = radius * Cosf(yAngle);

		pos += origin;
	}

	// fov use rad, opengl
   glm::mat4 Math::ProjFovRH(float fieldOfViewY,float aspectRatio,float znearPlane,float zfarPlane)
   {
	   float h = 1.0f / Math::Tan(fieldOfViewY / 2);
	   float w = h / aspectRatio;

	   return glm::mat4( w,  0,  0,                                         0,
                        0,  h,  0,                                         0,
                        0,  0,  zfarPlane/(znearPlane-zfarPlane),         -1,
                        0,  0,  znearPlane*zfarPlane/(znearPlane-zfarPlane),0);
   }

   glm::mat4 Math::ViewMatrixRH(const glm::vec3 &eye,const glm::vec3 &xAxis,const glm::vec3 &yAxis,const glm::vec3 &zAxis)
   {
 
	    return glm::mat4(
		           xAxis.x,         yAxis.x,           zAxis.x,          0,
                   xAxis.y,         yAxis.y,           zAxis.y,         0,
                   xAxis.z,         yAxis.z,           zAxis.z,          0,
				   glm::dot(-xAxis,eye),   glm::dot(-yAxis,eye),   glm::dot(-zAxis,eye),  1 );
   }

   glm::mat4 Math::ViewMatrixRH(const glm::vec3 &eye,const glm::vec3 &center,const glm::vec3 &up)
   {
	   glm::vec3 zaxis = eye - center;
	   zaxis = glm::normalize(zaxis);
	   glm::vec3 xaxis = glm::cross(up,zaxis);
	   xaxis = glm::normalize(xaxis);
	   glm::vec3 yaxis = glm::cross(zaxis,xaxis);

	   return ViewMatrixRH(eye,xaxis,yaxis,zaxis);
   }

     
}