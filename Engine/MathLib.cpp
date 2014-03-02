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
  
	// From Eigen implement
	glm::vec3 Math::EulerAngles(const glm::mat4& mat, int a0, int a1, int a2) 
	{
			/* Implemented from Graphics Gems IV */
			glm::vec3 res;
			 
			const int odd = ((a0 + 1) % 3 == a1) ? 0 : 1;
			const int i = a0;
			const int j = (a0 + 1 + odd) % 3;
			const int k = (a0 + 2 - odd) % 3;

			if (a0 == a2)
			{
				res[0] = atan2(mat[i][j], mat[i][k]);
				if ((odd && res[0]< 0) || ((!odd) && res[0]> 0))
				{
					res[0] = (res[0] > 0) ? res[0] - Math::PI : res[0] + Math::PI;
					float s2 = glm::length(glm::vec2(mat[i][j], mat[i][k]));
					res[1] = -atan2(s2, mat[i][i]);
				}
				else
				{
					float s2 = glm::length(glm::vec2(mat[i][j], mat[i][k]));
					res[1] = atan2(s2, mat[i][i]);
				}

				// With a=(0,1,0), we have i=0; j=1; k=2, and after computing the first two angles,
				// we can compute their respective rotation, and apply its inverse to M. Since the result must
				// be a rotation around x, we have:
				//
				//  c2  s1.s2 c1.s2                   1  0   0 
				//  0   c1    -s1       *    M    =   0  c3  s3
				//  -s2 s1.c2 c1.c2                   0 -s3  c3
				//
				//  Thus:  m11.c1 - m21.s1 = c3  &   m12.c1 - m22.s1 = s3

				float s1 = sin(res[0]);
				float c1 = cos(res[0]);
				res[2] = atan2(c1*mat[k][j] - s1*mat[k][k], c1*mat[j][j] - s1 * mat[j][k]);
			}
			else
			{
				res[0] = atan2(mat[k][j], mat[k][k]);
				float c2 = glm::length(glm::vec2(mat[i][i], mat[j][i]));
				if ((odd && res[0]<0 ) || ((!odd) && res[0]>0 )) {
					res[0] = (res[0] > 0) ? res[0] - Math::PI : res[0] + Math::PI;
					res[1] = atan2(-mat[k][i], -c2);
				}
				else
					res[1] = atan2(-mat[k][i], c2);
				float s1 = sin(res[0]);
				float c1 = cos(res[0]);
				res[2] = atan2(s1*mat[i][k] - c1*mat[i][j], c1*mat[j][j] - s1 * mat[j][k]);
			}
			if (!odd)
				res = -res;

			return res;
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