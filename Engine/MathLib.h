#ifndef _DISORDER_MATHLIB_H_
#define _DISORDER_MATHLIB_H_

namespace Disorder
{
	class Vector3;

	template< class T > inline T Abs( const T A )
	{
		return (A>=(T)0) ? A : -A;
	}
	template< class T > inline T Sgn( const T A )
	{
		return (A>0) ? 1 : ((A<0) ? -1 : 0);
	}
	template< class T > inline T Max( const T A, const T B )
	{
		return (A>=B) ? A : B;
	}
	template< class T > inline T Min( const T A, const T B )
	{
		return (A<=B) ? A : B;
	}
	template< class T > inline T Max3( const T A, const T B, const T C )
	{
		return Max ( Max( A, B ), C );
	}
	template< class T > inline T Min3( const T A, const T B, const T C )
	{
		return Min ( Min( A, B ), C );
	}
	template< class T > inline T Square( const T A )
	{
		return A*A;
	}
	template< class T > inline T Clamp( const T X, const T Min, const T Max )
	{
		return X<Min ? Min : X<Max ? X : Max;
	}
	 
	template< class T > inline void Swap( T& A, T& B )
	{
		const T Temp = A;
		A = B;
		B = Temp;
	}

	class Math
	{
	public:

		/** Returns the closest power-of-two number greater or equal to value.
            @note 0 and 1 are powers of two, so 
                firstPO2From(0)==0 and firstPO2From(1)==1.
        */
        static inline unsigned int FirstPO2From(unsigned int n)
        {
            --n;            
            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;
            ++n;
            return n;
        }

        /** Determines whether the number is power-of-two or not.
            @note 0 and 1 are tread as power of two.
        */
        template<typename T>
        static inline bool IsPO2(T n)
        {
            return (n & (n-1)) == 0;
        }

		static inline float Sqrtf(float f)
		{
			return sqrtf(f);
		}
	 
		static inline float Sinf(float a)
		{
			return sinf(a);
		}

		static inline float Cosf(float a)
		{
			return cosf(a);
		}

		static inline float ASinf(float a)
		{
			return asinf(a);
		}

		static inline float ACosf(float a)
		{
			return acosf(a);
		}
 
		static inline float Lerp(float t, float v1, float v2)
		{
           return (1.f - t) * v1 + t * v2;
        }

		static inline float ATan2f (float fY, float fX) 
		{ 
			return atan2(fY,fX); 
		}

		static inline float Tan(float fx)
		{
			return tan(fx);
		}

		static inline bool IsNaN(float fx)
		{
			return _isnan(fx) != 0;
		}

		static inline bool IsFinite(float fx)
		{
			return _finite(fx) != 0;
		}

		static inline bool FloatEqual( float a, float b, float tolerance = std::numeric_limits<float>::epsilon() )
		{
			if (fabs(b-a) <= tolerance)
				return true;
			else
				return false;
		}

		static inline float UnitRandom()
		{
			return float(rand()) / (float)RAND_MAX;
		}
 
		static  void ConvertToSphericalCoord(const Vector3& pos,const Vector3& origin,float &radius,float &yAngle,float &zAngle);	
		static  void ConvertFromSphericalCoord(float radius,float yAngle,float zAngle,const Vector3& origin,Vector3& pos);
		

		static const float Math::POS_INFINITY;
        static const float Math::NEG_INFINITY;
		static const float PI;
        static const float TWO_PI;
        static const float HALF_PI;
		static const float fDeg2Rad;
		static const float fRad2Deg;
	};
}









#endif