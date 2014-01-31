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
   Eigen::Matrix4f Math::ProjFovRH(float fieldOfViewY,float aspectRatio,float znearPlane,float zfarPlane)
   {
	   float h = 1.0f / Math::Tan(fieldOfViewY / 2);
	   float w = h / aspectRatio;
      
	   Eigen::Matrix4f ProjMat;
	   ProjMat <<   w,  0,  0,                                             0,
                    0,  h,  0,                                             0,
                    0,  0,  zfarPlane/(znearPlane-zfarPlane),              znearPlane*zfarPlane/(znearPlane-zfarPlane),
                    0,  0,  -1,   0;

	   return ProjMat;
   }

   Eigen::Matrix4f Math::ViewMatrixRH(const Vector3 &eye,const Vector3 &center,const Vector3 &up)
   {
	   Vector3 zaxis = eye - center;
	   zaxis.Normalise();
	   Vector3 xaxis = up.Cross(zaxis);
	   xaxis.Normalise();
	   Vector3 yaxis = zaxis.Cross(xaxis);

	   Eigen::Matrix4f ViewMat;
	   ViewMat <<  xaxis.x,         xaxis.y,           xaxis.z,          -xaxis.Dot(eye),
                   yaxis.x,         yaxis.y,           yaxis.z,          -yaxis.Dot(eye),
                   zaxis.x,         zaxis.y,           zaxis.z,          -zaxis.Dot(eye),
				   0,               0,                 0,                1 ;

	   return ViewMat;
   }
	

     
}