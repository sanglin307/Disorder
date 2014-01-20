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
   Matrix4 Math::PerspectiveMatrix(float fovy, float aspect, float zNear, float zFar)
   {
       float tanHalfFovy = tan(fovy / 2);
	   Matrix4 Result = Matrix4::ZERO;
       Result[0][0] = 1 / (aspect * tanHalfFovy);
       Result[1][1] = 1 / (tanHalfFovy);
       Result[2][2] = - (zFar + zNear) / (zFar - zNear);
       Result[2][3] = - 1;
       Result[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
       return Result;
   }

   Matrix4 Math::LookAtMatrix(Vector3 &eye,Vector3 &center,Vector3 &up)
   {
	   Vector3 f = (center - eye);
	   f.Normalise();
	   up.Normalise();
	   Vector3 u = up;
	   Vector3 s = f.Cross(u);
	   s.Normalise();
	   u = s.Cross(f);

	   Matrix4 Result = Matrix4::IDENTITY;
	   Result[0][0] = s.x;
	   Result[1][0] = s.y;
	   Result[2][0] = s.z;
	   Result[0][1] = u.x;
	   Result[1][1] = u.y;
	   Result[2][1] = u.z;
	   Result[0][2] =-f.x;
	   Result[1][2] =-f.y;
	   Result[2][2] =-f.z;
	   Result[3][0] =-s.Dot(eye);
	   Result[3][1] =-u.Dot(eye);
	   Result[3][2] = f.Dot(eye);
	   return Result;
	 }
}