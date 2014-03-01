#ifndef _DISORDER_QUATERNION_H_
#define _DISORDER_QUATERNION_H_


namespace Disorder
{
	/** Implementation of a Quaternion, i.e. a rotation around an axis.
		For more information about Quaternions and the theory behind it, we recommend reading:
		http://www.ogre3d.org/tikiwiki/Quaternion+and+Rotation+Primer
		http://www.cprogramming.com/tutorial/3d/quaternions.html
		http://www.gamedev.net/page/resources/_/reference/programming/math-and-physics/
		quaternions/quaternion-powers-r1095
    */

	template <typename T> class TVector3;
	template <typename T> class TMatrix3;

	template <typename T>
    class TQuaternion
    {
    public:
		/// Default constructor, initializes to identity rotation (aka 0бу)
		inline TQuaternion ()
			: w(1), x(0), y(0), z(0)
		{
		}
		/// Construct from an explicit list of values
		inline TQuaternion (
			T fW,
			T fX, T fY, T fZ)
			: w(fW), x(fX), y(fY), z(fZ)
		{
		}
        /// Construct a quaternion from a rotation matrix
        inline TQuaternion(const TMatrix3<T>& rot)
        {
            FromRotationMatrix(rot);
        }
        /// Construct a quaternion from an angle/axis
        inline TQuaternion(const T& rfAngle, const TVector3<T>& rkAxis)
        {
            FromAngleAxis(rfAngle, rkAxis);
        }
        /// Construct a quaternion from 3 orthonormal local axes
        inline TQuaternion(const TVector3<T>& xaxis, const TVector3<T>& yaxis, const TVector3<T>& zaxis)
        {
            FromAxes(xaxis, yaxis, zaxis);
        }
        /// Construct a quaternion from 3 orthonormal local axes
        inline TQuaternion(const TVector3<T>* akAxis)
        {
            FromAxes(akAxis);
        }
		/// Construct a quaternion from 4 manual w/x/y/z values
		inline TQuaternion(T* valptr)
		{
			memcpy(&w, valptr, sizeof(T)*4);
		}
 
		/// Array accessor operator
		inline T operator [] ( const size_t i ) const
		{
			return *(&w+i);
		}

		/// Array accessor operator
		inline T& operator [] ( const size_t i )
		{
			return *(&w+i);
		}

		/// Pointer accessor for direct copying
		inline T* Ptr()
		{
			return &w;
		}

		/// Pointer accessor for direct copying
		inline const T* Ptr() const
		{
			return &w;
		}

		inline void FromRotationMatrix (const TMatrix3<T>& kRot)
		{
			// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
			// article "Quaternion Calculus and Fast Animation".

			T fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
			T fRoot;

			if ( fTrace > 0.0 )
			{
				// |w| > 1/2, may as well choose w > 1/2
				fRoot = sqrt(fTrace + 1.0f);  // 2w
				w = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;  // 1/(4w)
				x = (kRot[2][1]-kRot[1][2])*fRoot;
				y = (kRot[0][2]-kRot[2][0])*fRoot;
				z = (kRot[1][0]-kRot[0][1])*fRoot;
			}
			else
			{
				// |w| <= 1/2
				static size_t s_iNext[3] = { 1, 2, 0 };
				size_t i = 0;
				if ( kRot[1][1] > kRot[0][0] )
					i = 1;
				if ( kRot[2][2] > kRot[i][i] )
					i = 2;
				size_t j = s_iNext[i];
				size_t k = s_iNext[j];

				fRoot = sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
				T* apkQuat[3] = { &x, &y, &z };
				*apkQuat[i] = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;
				w = (kRot[k][j]-kRot[j][k])*fRoot;
				*apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
				*apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
			}
		}

        void ToRotationMatrix (TMatrix3<T>& kRot) const
		{
			T fTx  = x+x;
			T fTy  = y+y;
			T fTz  = z+z;
			T fTwx = fTx*w;
			T fTwy = fTy*w;
			T fTwz = fTz*w;
			T fTxx = fTx*x;
			T fTxy = fTy*x;
			T fTxz = fTz*x;
			T fTyy = fTy*y;
			T fTyz = fTz*y;
			T fTzz = fTz*z;

			kRot[0][0] = 1.0f-(fTyy+fTzz);
			kRot[0][1] = fTxy-fTwz;
			kRot[0][2] = fTxz+fTwy;
			kRot[1][0] = fTxy+fTwz;
			kRot[1][1] = 1.0f-(fTxx+fTzz);
			kRot[1][2] = fTyz-fTwx;
			kRot[2][0] = fTxz-fTwy;
			kRot[2][1] = fTyz+fTwx;
			kRot[2][2] = 1.0f-(fTxx+fTyy);
		}

		/** Setups the quaternion using the supplied vector, and "roll" around
			that vector by the specified radians.
		*/
        void FromAngleAxis (const T rfAngle, const TVector3<T>& rkAxis)
		{
			// assert:  axis[] is unit length
			//
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

			T fHalfAngle ( 0.5f*rfAngle );
			T fSin = sin(fHalfAngle);
			w = cos(fHalfAngle);
			x = fSin*rkAxis.x;
			y = fSin*rkAxis.y;
			z = fSin*rkAxis.z;
		}

        inline void RadianToAngleAxis (T& rfAngle, TVector3<T>& rkAxis) const
		{
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

			T fSqrLength = x*x+y*y+z*z;
			if ( fSqrLength > 0.0 )
			{
				rfAngle = 2.0f*acos(w);
				T fInvLength = 1.0f/sqrt(fSqrLength);
				rkAxis.x = x*fInvLength;
				rkAxis.y = y*fInvLength;
				rkAxis.z = z*fInvLength;
			}
			else
			{
				// angle is 0 (mod 2*pi), so any axis will do
				rfAngle = 0.0f;
				rkAxis.x = 1.0;
				rkAxis.y = 0.0;
				rkAxis.z = 0.0;
			}

		}

        inline void DegreeToAngleAxis (int& dAngle, TVector3<T>& rkAxis) const 
		{
            T rAngle;
            RadianToAngleAxis ( rAngle, rkAxis );
			dAngle = (int)(Math::fRad2Deg * rAngle);
        }

		/** Constructs the quaternion using 3 axes, the axes are assumed to be orthonormal
			@see FromAxes
		*/
        void FromAxes (const TVector3<T>* akAxis)
		{
			TMatrix3<T> kRot;

			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kRot[0][iCol] = akAxis[iCol].x;
				kRot[1][iCol] = akAxis[iCol].y;
				kRot[2][iCol] = akAxis[iCol].z;
			}

			FromRotationMatrix(kRot);
		}

        void FromAxes (const TVector3<T>& xAxis, const TVector3<T>& yAxis, const TVector3<T>& zAxis)
		{
			TMatrix3<T> kRot;

			kRot[0][0] = xAxis.x;
			kRot[1][0] = xAxis.y;
			kRot[2][0] = xAxis.z;

			kRot[0][1] = yAxis.x;
			kRot[1][1] = yAxis.y;
			kRot[2][1] = yAxis.z;

			kRot[0][2] = zAxis.x;
			kRot[1][2] = zAxis.y;
			kRot[2][2] = zAxis.z;

			FromRotationMatrix(kRot);
		}

		/** Gets the 3 orthonormal axes defining the quaternion. @see FromAxes */
        void ToAxes (TVector3<T>* akAxis) const
		{
			TMatrix3<T> kRot;

			ToRotationMatrix(kRot);

			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				akAxis[iCol].x = kRot[0][iCol];
				akAxis[iCol].y = kRot[1][iCol];
				akAxis[iCol].z = kRot[2][iCol];
			}
		}

        void ToAxes (TVector3<T>& xAxis, TVector3<T>& yAxis, TVector3<T>& zAxis) const
		{
			TMatrix3<T> kRot;

			ToRotationMatrix(kRot);

			xAxis.x = kRot[0][0];
			xAxis.y = kRot[1][0];
			xAxis.z = kRot[2][0];

			yAxis.x = kRot[0][1];
			yAxis.y = kRot[1][1];
			yAxis.z = kRot[2][1];

			zAxis.x = kRot[0][2];
			zAxis.y = kRot[1][2];
			zAxis.z = kRot[2][2];
		}

		/** Returns the X orthonormal axis defining the quaternion. Same as doing
			xAxis = Vector3::UNIT_X * this. Also called the local X-axis
		*/
        TVector3<T> xAxis(void) const
		{
			 //Real fTx  = 2.0*x;
			T fTy  = 2.0f*y;
			T fTz  = 2.0f*z;
			T fTwy = fTy*w;
			T fTwz = fTz*w;
			T fTxy = fTy*x;
			T fTxz = fTz*x;
			T fTyy = fTy*y;
			T fTzz = fTz*z;

			return TVector3<T>(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
		}

        /** Returns the Y orthonormal axis defining the quaternion. Same as doing
			yAxis = Vector3::UNIT_Y * this. Also called the local Y-axis
		*/
        TVector3<T> yAxis(void) const
		{
			T fTx  = 2.0f*x;
			T fTy  = 2.0f*y;
			T fTz  = 2.0f*z;
			T fTwx = fTx*w;
			T fTwz = fTz*w;
			T fTxx = fTx*x;
			T fTxy = fTy*x;
			T fTyz = fTz*y;
			T fTzz = fTz*z;

			return TVector3<T>(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
		}

		/** Returns the Z orthonormal axis defining the quaternion. Same as doing
			zAxis = Vector3::UNIT_Z * this. Also called the local Z-axis
		*/
        TVector3<T> zAxis(void) const
		{
			T fTx  = 2.0f*x;
			T fTy  = 2.0f*y;
			T fTz  = 2.0f*z;
			T fTwx = fTx*w;
			T fTwy = fTy*w;
			T fTxx = fTx*x;
			T fTxz = fTz*x;
			T fTyy = fTy*y;
			T fTyz = fTz*y;

			return TVector3<T>(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
		}

        inline TQuaternion& operator= (const TQuaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}

        inline TQuaternion operator+ (const TQuaternion& rkQ) const
		{
			return TQuaternion(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
		}

        inline TQuaternion operator- (const TQuaternion& rkQ) const
		{
			return Quaternion(w-rkQ.w,x-rkQ.x,y-rkQ.y,z-rkQ.z);
		}

        inline TQuaternion operator* (const TQuaternion& rkQ) const
		{
			// NOTE:  Multiplication is not generally commutative, so in most
			// cases p*q != q*p.

			return TQuaternion
			(
				w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
				w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
				w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
				w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
			);
		}

        inline TQuaternion operator* (T fScalar) const
		{
			 return Quaternion(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
		}

        friend TQuaternion operator* (T fScalar, const TQuaternion& rkQ)
		{
			return TQuaternion(fScalar*rkQ.w,fScalar*rkQ.x,fScalar*rkQ.y, fScalar*rkQ.z);
		}

        inline TQuaternion operator- () const
		{
			return TQuaternion(-w,-x,-y,-z);
		}

        inline bool operator== (const TQuaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
		}

        inline bool operator!= (const TQuaternion& rhs) const
		{
			return !operator==(rhs);
		}
        // functions of a quaternion
        /// Returns the dot product of the quaternion
        inline T Dot (const TQuaternion& rkQ) const
		{
			return w*rkQ.w+x*rkQ.x+y*rkQ.y+z*rkQ.z;
		}
        /* Returns the normal length of this quaternion.
            @note This does <b>not</b> alter any values.
        */
        inline T Norm () const
		{
			return w*w+x*x+y*y+z*z;
		}

        /// Normalises this quaternion, and returns the previous length
        inline T Normalise(void)
		{
			T len = Norm();
			T factor = 1.0f / sqrt(len);
			*this = *this * factor;
			return len;
		}

        inline TQuaternion Inverse () const  // apply to non-zero quaternion
		{
			T fNorm = w*w+x*x+y*y+z*z;
			if ( fNorm > 0.0 )
			{
				T fInvNorm = 1.0f/fNorm;
				return TQuaternion(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);
			}
			else
			{
				// return an invalid result to flag the error
				return TQuaternion<T>(0,0,0,0);
			}
		}

        TQuaternion UnitInverse () const  // apply to unit-length quaternion
		{
			// assert:  'this' is unit length
            return Quaternion(w,-x,-y,-z);
		}

        TQuaternion Exp () const
		{
			// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
			// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

			T fAngle ( Math::Sqrtf(x*x+y*y+z*z) );
			T fSin = Math::Sinf(fAngle);

			TQuaternion<T> kResult;
			kResult.w = Math::Cosf(fAngle);

			if ( Abs(fSin) >= 1e-3f )
			{
				float fCoeff = fSin/(fAngle);
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
			}
			else
			{
				kResult.x = x;
				kResult.y = y;
				kResult.z = z;
			}

			return kResult;
		}

        TQuaternion Log () const
		{
			// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
			// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

			TQuaternion kResult;
			kResult.w = 0.0;

			if ( Abs(w) < 1.0 )
			{
				float fAngle ( Math::ACosf(w) );
				float fSin = Math::Sinf(fAngle);
				if ( Abs(fSin) >= 1e-03f )
				{
					T fCoeff = fAngle/fSin;
					kResult.x = fCoeff*x;
					kResult.y = fCoeff*y;
					kResult.z = fCoeff*z;
					return kResult;
				}
			}

			kResult.x = x;
			kResult.y = y;
			kResult.z = z;

			return kResult;
		}

        /// Rotation of a vector by a quaternion
        TVector3<T> operator* (const TVector3<T>& rkVector) const
		{
			// nVidia SDK implementation
			TVector3<T> uv, uuv;
			TVector3<T> qvec(x, y, z);
			uv = qvec.Cross(rkVector);
			uuv = qvec.Cross(uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;

			return rkVector + uv + uuv;
		}

   	 
		/// Equality with tolerance (tolerance is max angle difference)
		bool Equals(const TQuaternion& rhs, const T& tolerance) const
		{
			T fCos = Dot(rhs);
			T angle = acos(fCos);

			return (Abs(angle) <= tolerance) || Math::FloatEqual(angle, Math::PI, tolerance);
		}
		
	    /** Performs Spherical linear interpolation between two quaternions, and returns the result.
			Slerp ( 0.0f, A, B ) = A
			Slerp ( 1.0f, A, B ) = B
			@return Interpolated quaternion
			@remarks
			Slerp has the proprieties of performing the interpolation at constant
			velocity, and being torque-minimal (unless shortestPath=false).
			However, it's NOT commutative, which means
			Slerp ( 0.75f, A, B ) != Slerp ( 0.25f, B, A );
			therefore be careful if your code relies in the order of the operands.
			This is specially important in IK animation.
		*/
        static TQuaternion Slerp (T fT, const TQuaternion<T>& rkP,
            const TQuaternion& rkQ, bool shortestPath = false)
		{
			T fCos = rkP.Dot(rkQ);
			TQuaternion rkT;

			// Do we need to invert rotation?
			if (fCos < 0.0f && shortestPath)
			{
				fCos = -fCos;
				rkT = -rkQ;
			}
			else
			{
				rkT = rkQ;
			}

			if (Abs(fCos) < 1 - msEpsilon)
			{
				// Standard case (slerp)
				T fSin = Math::Sqrtf(1 - Square(fCos));
				T fAngle = Math::ATan2f(fSin, fCos);
				T fInvSin = 1.0f / fSin;
				T fCoeff0 = Math::Sinf((1.0f - fT) * fAngle) * fInvSin;
				T fCoeff1 = Math::Sinf(fT * fAngle) * fInvSin;
				return fCoeff0 * rkP + fCoeff1 * rkT;
			}
			else
			{
				// There are two situations:
				// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
				//    interpolation safely.
				// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
				//    are an infinite number of possibilities interpolation. but we haven't
				//    have method to fix this case, so just use linear interpolation here.
				TQuaternion t = (1.0f - fT) * rkP + fT * rkT;
				// taking the complement requires renormalisation
				t.Normalise();
				return t;
			}
		}

		/** @see Slerp. It adds extra "spins" (i.e. rotates several times) specified
			by parameter 'iExtraSpins' while interpolating before arriving to the
			final values
		*/
        static TQuaternion SlerpExtraSpins (T fT,
            const TQuaternion& rkP, const TQuaternion& rkQ,
            int iExtraSpins)
		{
			T fCos = rkP.Dot(rkQ);
			T fAngle ( Math::ACosf(fCos) );

			if ( Abs(fAngle) < 1e-03f )
				return rkP;

			T fSin = sin(fAngle);
			T fPhase ( Math::PI*iExtraSpins*fT );
			T fInvSin = 1.0f/fSin;
			T fCoeff0 = Math::Sinf((1.0f-fT)*fAngle - fPhase)*fInvSin;
			T fCoeff1 = Math::Sinf(fT*fAngle + fPhase)*fInvSin;
			return fCoeff0*rkP + fCoeff1*rkQ;
		}

        // setup for spherical quadratic interpolation
        static void Intermediate (const TQuaternion& rkQ0,
            const TQuaternion& rkQ1, const TQuaternion& rkQ2,
            TQuaternion& rka, TQuaternion& rkB)
		{
			 // assert:  q0, q1, q2 are unit quaternions

			TQuaternion kQ0inv = rkQ0.UnitInverse();
			TQuaternion kQ1inv = rkQ1.UnitInverse();
			TQuaternion rkP0 = kQ0inv*rkQ1;
			TQuaternion rkP1 = kQ1inv*rkQ2;
			TQuaternion kArg = 0.25*(rkP0.Log()-rkP1.Log());
			TQuaternion kMinusArg = -kArg;

			rkA = rkQ1*kArg.Exp();
			rkB = rkQ1*kMinusArg.Exp();
		}


        // spherical quadratic interpolation
        static TQuaternion Squad (T fT, const TQuaternion& rkP,
            const TQuaternion& rkA, const TQuaternion& rkB,
            const TQuaternion& rkQ, bool shortestPath = false)
		{
			T fSlerpT = 2.0f*fT*(1.0f-fT);
			TQuaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
			TQuaternion kSlerpQ = Slerp(fT, rkA, rkB);
			return Slerp(fSlerpT, kSlerpP ,kSlerpQ);
		}

        /** Performs Normalised linear interpolation between two quaternions, and returns the result.
			nlerp ( 0.0f, A, B ) = A
			nlerp ( 1.0f, A, B ) = B
			@remarks
			Nlerp is faster than Slerp.
			Nlerp has the proprieties of being commutative (@see Slerp;
			commutativity is desired in certain places, like IK animation), and
			being torque-minimal (unless shortestPath=false). However, it's performing
			the interpolation at non-constant velocity; sometimes this is desired,
			sometimes it is not. Having a non-constant velocity can produce a more
			natural rotation feeling without the need of tweaking the weights; however
			if your scene relies on the timing of the rotation or assumes it will point
			at a specific angle at a specific weight value, Slerp is a better choice.
		*/
        static TQuaternion NLerp(T fT, const TQuaternion& rkP, 
            const TQuaternion& rkQ, bool shortestPath = false)
		{
			TQuaternion result;
			T fCos = rkP.Dot(rkQ);
			if (fCos < 0.0f && shortestPath)
			{
				result = rkP + fT * ((-rkQ) - rkP);
			}
			else
			{
				result = rkP + fT * (rkQ - rkP);
			}
			result.Normalise();
			return result;
		}

		T w, x, y, z;

		/// Check whether this quaternion contains valid values
		inline bool IsNaN() const
		{
			return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z) || Math::IsNaN(w);
		}

        
    };

	typedef TQuaternion<float> Quaternionf;
 

}

#endif