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

 
	void Math::ConvertToSphericalCoord(const Vector3& pos,const Vector3& origin,float &radius,float &yAngle,float &zAngle)
	{
		Vector3 deltaVec = pos - origin;
		radius = deltaVec.Length();
		yAngle = ACosf(deltaVec.y/radius);
		zAngle = ATan2f(deltaVec.x,deltaVec.z);
	}

	void Math::ConvertFromSphericalCoord(float radius,float yAngle,float zAngle,const Vector3& origin,Vector3& pos)
	{
		BOOST_ASSERT(radius > 0);
		float temp = radius * Sinf(yAngle);
		pos.z = temp * Cosf(zAngle);
		pos.x = temp * Sinf(zAngle);
		pos.y = radius * Cosf(yAngle);

		pos += origin;
	}

	// fov use rad, opengl
   Matrix4 Math::PerspectiveFovRH(float fieldOfViewY,float aspectRatio,float znearPlane,float zfarPlane)
   {
	   float h = 1.0f / Math::Tan(fieldOfViewY / 2);
	   float w = h / aspectRatio;
      
	   return Matrix4(  w,  0,  0,                                             0,
                        0,  h,  0,                                             0,
                        0,  0,  zfarPlane/(znearPlane-zfarPlane),              -1,
                        0,  0,  znearPlane*zfarPlane/(znearPlane-zfarPlane),   0);
   }

   Matrix4 Math::ViewMatrixRH(const Vector3 &eye,const Vector3 &center,const Vector3 &up)
   {
	   Vector3 zaxis = eye - center;
	   zaxis.Normalise();
	   Vector3 xaxis = up.Cross(zaxis);
	   xaxis.Normalise();
	   Vector3 yaxis = zaxis.Cross(xaxis);

	   return Matrix4(  xaxis.x,         yaxis.x,           zaxis.x,          0,
                        xaxis.y,         yaxis.y,           zaxis.y,          0,
                        xaxis.z,         yaxis.z,           zaxis.z,          0,
						-xaxis.Dot(eye), -yaxis.Dot(eye),   -zaxis.Dot(eye),  1 );
   }

     
}