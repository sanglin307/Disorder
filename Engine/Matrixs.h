#ifndef _DISORDER_MATRIX_H_
#define _DISORDER_MATRIX_H_


namespace Disorder
{
 
	template<typename T>
    class TMatrix3
    {
    public:
        /** Default constructor.
            @note
                It does <b>NOT</b> initialize the matrix for efficiency.
        */
		inline TMatrix3 () {}
        inline explicit TMatrix3 (const T arr[3][3])
		{
			memcpy(m,arr,9*sizeof(T));
		}

        inline TMatrix3 (const TMatrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(T));
		}

        inline TMatrix3 (T fEntry00, T fEntry01, T fEntry02,
                  T fEntry10, T fEntry11, T fEntry12,
                  T fEntry20, T fEntry21, T fEntry22)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
		}

        // member access, allows use of construct mat[r][c]
        inline T* operator[] (size_t iRow) const
		{
			return (T*)m[iRow];
		}
 
        inline TVector3<T> GetColumn (size_t iCol) const
		{
            return TVector3<T>(m[0][iCol],m[1][iCol],m[2][iCol]);
		}

        inline void SetColumn(size_t iCol, const TVector3<T>& vec)
		{
			 m[0][iCol] = vec.x;
             m[1][iCol] = vec.y;
             m[2][iCol] = vec.z;
		}

        inline void FromAxes(const TVector3<T>& xAxis, const TVector3<T>& yAxis, const TVector3<T>& zAxis)
		{
			SetColumn(0,xAxis);
            SetColumn(1,yAxis);
            SetColumn(2,zAxis);
		}

        // assignment and comparison
        inline TMatrix3& operator= (const TMatrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(T));
			return *this;
		}

        /** Tests 2 matrices for equality.
         */
        inline bool operator== (const TMatrix3& rkMatrix) const
		{
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
				{
					if ( m[iRow][iCol] != rkMatrix.m[iRow][iCol] )
						return false;
				}
			}

			return true;
		}

        /** Tests 2 matrices for inequality.
         */
        inline bool operator!= (const TMatrix3& rkMatrix) const
		{
			return !operator==(rkMatrix);
		}

        // arithmetic operations
        /** Matrix addition.
         */
        inline TMatrix3 operator+ (const TMatrix3& rkMatrix) const
		{
			TMatrix3 kSum;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
				{
					kSum.m[iRow][iCol] = m[iRow][iCol] +
						rkMatrix.m[iRow][iCol];
				}
			}
			return kSum;
		}

        /** Matrix subtraction.
         */
        inline TMatrix3 operator- (const TMatrix3& rkMatrix) const
		{
			TMatrix3 kDiff;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
				{
					kDiff.m[iRow][iCol] = m[iRow][iCol] -
						rkMatrix.m[iRow][iCol];
				}
			}
			return kDiff;
		}

        /** Matrix concatenation using '*'.
         */
        inline TMatrix3 operator* (const TMatrix3& rkMatrix) const
		{
			TMatrix3 kProd;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
				{
					kProd.m[iRow][iCol] =
						m[iRow][0]*rkMatrix.m[0][iCol] +
						m[iRow][1]*rkMatrix.m[1][iCol] +
						m[iRow][2]*rkMatrix.m[2][iCol];
				}
			}
			return kProd;
		}

        inline TMatrix3 operator- () const
		{
			TMatrix3 kNeg;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
					kNeg[iRow][iCol] = -m[iRow][iCol];
			}
			return kNeg;
		}

        /// Matrix * vector [3x3 * 3x1 = 3x1]
        inline TVector3<T> operator* (const TVector3<T>& rkVector) const
		{
			TVector3<T> kProd;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				kProd[iRow] =
					m[iRow][0]*rkPoint[0] +
					m[iRow][1]*rkPoint[1] +
					m[iRow][2]*rkPoint[2];
			}
			return kProd;
		}

        /// Vector * matrix [1x3 * 3x3 = 1x3]
        friend TVector3<T> operator* (const TVector3<T>& rkVector,
            const TMatrix3& rkMatrix)
		{
			TVector3<T> kProd;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				kProd[iRow] =
					rkPoint[0]*rkMatrix.m[0][iRow] +
					rkPoint[1]*rkMatrix.m[1][iRow] +
					rkPoint[2]*rkMatrix.m[2][iRow];
			}
			return kProd;
		}

        /// Matrix * scalar
        inline TMatrix3 operator* (T fScalar) const
		{
			TMatrix3 kProd;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
					kProd[iRow][iCol] = fScalar*m[iRow][iCol];
			}
			return kProd;
		}

        /// Scalar * matrix
        friend TMatrix3 operator* (T fScalar, const TMatrix3& rkMatrix)
		{
			TMatrix3 kProd;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
					kProd[iRow][iCol] = fScalar*rkMatrix.m[iRow][iCol];
			}
			return kProd;
		}

        // utilities
        inline TMatrix3 Transpose () const
		{
			TMatrix3 kTranspose;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
					kTranspose[iRow][iCol] = m[iCol][iRow];
			}
			return kTranspose;
		}

        inline bool Inverse (TMatrix3& rkInverse, float fTolerance = 1e-06) const
		{
			// Invert a 3x3 using cofactors.  This is about 8 times faster than
			// the Numerical Recipes code which uses Gaussian elimination.

			rkInverse[0][0] = m[1][1]*m[2][2] -
				m[1][2]*m[2][1];
			rkInverse[0][1] = m[0][2]*m[2][1] -
				m[0][1]*m[2][2];
			rkInverse[0][2] = m[0][1]*m[1][2] -
				m[0][2]*m[1][1];
			rkInverse[1][0] = m[1][2]*m[2][0] -
				m[1][0]*m[2][2];
			rkInverse[1][1] = m[0][0]*m[2][2] -
				m[0][2]*m[2][0];
			rkInverse[1][2] = m[0][2]*m[1][0] -
				m[0][0]*m[1][2];
			rkInverse[2][0] = m[1][0]*m[2][1] -
				m[1][1]*m[2][0];
			rkInverse[2][1] = m[0][1]*m[2][0] -
				m[0][0]*m[2][1];
			rkInverse[2][2] = m[0][0]*m[1][1] -
				m[0][1]*m[1][0];

			T fDet =
				m[0][0]*rkInverse[0][0] +
				m[0][1]*rkInverse[1][0]+
				m[0][2]*rkInverse[2][0];

			if ( Abs(fDet) <= fTolerance )
				return false;

			T fInvDet = 1.0f/fDet;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				for (size_t iCol = 0; iCol < 3; iCol++)
					rkInverse[iRow][iCol] *= fInvDet;
			}

			return true;
		}

        inline TMatrix3 Inverse (float fTolerance = 1e-06) const
		{
			TMatrix3 kInverse;
            Inverse(kInverse,fTolerance);
            return kInverse;
		}

        inline T Determinant () const
		{
			T fCofactor00 = m[1][1]*m[2][2] - m[1][2]*m[2][1];
			T fCofactor10 = m[1][2]*m[2][0] - m[1][0]*m[2][2];
			T fCofactor20 = m[1][0]*m[2][1] - m[1][1]*m[2][0];

			T fDet = m[0][0]*fCofactor00 + m[0][1]*fCofactor10 + m[0][2]*fCofactor20;
			return fDet;
		}

		inline void QDUDecomposition (TMatrix3& rkQ, TVector3<T>& rkD, TVector3<T>& rkU) const
		{
			// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
			// and U is upper triangular with ones on its diagonal.  Algorithm uses
			// Gram-Schmidt orthogonalization (the QR algorithm).
			//
			// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
			//
			//   q0 = m0/|m0|
			//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
			//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
			//
			// where |V| indicates length of vector V and A*B indicates dot
			// product of vectors A and B.  The matrix R has entries
			//
			//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
			//   r10 = 0      r11 = q1*m1  r12 = q1*m2
			//   r20 = 0      r21 = 0      r22 = q2*m2
			//
			// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
			// u02 = r02/r00, and u12 = r12/r11.

			// Q = rotation
			// D = scaling
			// U = shear

			// D stores the three diagonal entries r00, r11, r22
			// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

			// build orthogonal matrix Q
			float fInvLength = m[0][0]*m[0][0] + m[1][0]*m[1][0] + m[2][0]*m[2][0];
			if (!Math::FloatEqual(fInvLength,0)) fInvLength = 1.0f/Math::Sqrtf(fInvLength);

			kQ[0][0] = m[0][0]*fInvLength;
			kQ[1][0] = m[1][0]*fInvLength;
			kQ[2][0] = m[2][0]*fInvLength;

			float fDot = kQ[0][0]*m[0][1] + kQ[1][0]*m[1][1] +
				kQ[2][0]*m[2][1];
			kQ[0][1] = m[0][1]-fDot*kQ[0][0];
			kQ[1][1] = m[1][1]-fDot*kQ[1][0];
			kQ[2][1] = m[2][1]-fDot*kQ[2][0];
			fInvLength = kQ[0][1]*kQ[0][1] + kQ[1][1]*kQ[1][1] + kQ[2][1]*kQ[2][1];
			if (!Math::FloatEqual(fInvLength,0)) fInvLength = 1.0f/Math::Sqrtf(fInvLength);
        
			kQ[0][1] *= fInvLength;
			kQ[1][1] *= fInvLength;
			kQ[2][1] *= fInvLength;

			fDot = kQ[0][0]*m[0][2] + kQ[1][0]*m[1][2] +
				kQ[2][0]*m[2][2];
			kQ[0][2] = m[0][2]-fDot*kQ[0][0];
			kQ[1][2] = m[1][2]-fDot*kQ[1][0];
			kQ[2][2] = m[2][2]-fDot*kQ[2][0];
			fDot = kQ[0][1]*m[0][2] + kQ[1][1]*m[1][2] +
				kQ[2][1]*m[2][2];
			kQ[0][2] -= fDot*kQ[0][1];
			kQ[1][2] -= fDot*kQ[1][1];
			kQ[2][2] -= fDot*kQ[2][1];
			fInvLength = kQ[0][2]*kQ[0][2] + kQ[1][2]*kQ[1][2] + kQ[2][2]*kQ[2][2];
			if (!Math::FloatEqual(fInvLength,0)) fInvLength = 1.0f/Math::Sqrtf(fInvLength);

			kQ[0][2] *= fInvLength;
			kQ[1][2] *= fInvLength;
			kQ[2][2] *= fInvLength;

			// guarantee that orthogonal matrix has determinant 1 (no reflections)
			float fDet = kQ[0][0]*kQ[1][1]*kQ[2][2] + kQ[0][1]*kQ[1][2]*kQ[2][0] +
				kQ[0][2]*kQ[1][0]*kQ[2][1] - kQ[0][2]*kQ[1][1]*kQ[2][0] -
				kQ[0][1]*kQ[1][0]*kQ[2][2] - kQ[0][0]*kQ[1][2]*kQ[2][1];

			if ( fDet < 0.0 )
			{
				for (size_t iRow = 0; iRow < 3; iRow++)
					for (size_t iCol = 0; iCol < 3; iCol++)
						kQ[iRow][iCol] = -kQ[iRow][iCol];
			}

			// build "right" matrix R
			Matrix3 kR;
			kR[0][0] = kQ[0][0]*m[0][0] + kQ[1][0]*m[1][0] +
				kQ[2][0]*m[2][0];
			kR[0][1] = kQ[0][0]*m[0][1] + kQ[1][0]*m[1][1] +
				kQ[2][0]*m[2][1];
			kR[1][1] = kQ[0][1]*m[0][1] + kQ[1][1]*m[1][1] +
				kQ[2][1]*m[2][1];
			kR[0][2] = kQ[0][0]*m[0][2] + kQ[1][0]*m[1][2] +
				kQ[2][0]*m[2][2];
			kR[1][2] = kQ[0][1]*m[0][2] + kQ[1][1]*m[1][2] +
				kQ[2][1]*m[2][2];
			kR[2][2] = kQ[0][2]*m[0][2] + kQ[1][2]*m[1][2] +
				kQ[2][2]*m[2][2];

			// the scaling component
			kD[0] = kR[0][0];
			kD[1] = kR[1][1];
			kD[2] = kR[2][2];

			// the shear component
			float fInvD0 = 1.0f/kD[0];
			kU[0] = kR[0][1]*fInvD0;
			kU[1] = kR[0][2]*fInvD0;
			kU[2] = kR[1][2]/kD[1];
		}
 
        inline void Rotate (const TVector3<T>& rkAxis, const float& fRadians)
		{
			T fCos = cosf(fRadians);
			T fSin = sinf(fRadians);
			T fOneMinusCos = 1.0f-fCos;
			T fX2 = rkAxis.x*rkAxis.x;
			T fY2 = rkAxis.y*rkAxis.y;
			T fZ2 = rkAxis.z*rkAxis.z;
			T fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
			T fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
			T fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
			T fXSin = rkAxis.x*fSin;
			T fYSin = rkAxis.y*fSin;
			T fZSin = rkAxis.z*fSin;

			m[0][0] = fX2*fOneMinusCos+fCos;
			m[0][1] = fXYM-fZSin;
			m[0][2] = fXZM+fYSin;
			m[1][0] = fXYM+fZSin;
			m[1][1] = fY2*fOneMinusCos+fCos;
			m[1][2] = fYZM-fXSin;
			m[2][0] = fXZM-fYSin;
			m[2][1] = fYZM+fXSin;
			m[2][2] = fZ2*fOneMinusCos+fCos;
		}

        // The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
        // where yaw is rotation about the Up vector, pitch is rotation about the
        // Right axis, and roll is rotation about the Direction axis.
        inline bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle, float& rfRAngle) const
		{
			// rot =  cy*cz          -cy*sz           sy
			//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
			//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

			rfPAngle = float(asin(m[0][2]));
			if ( rfPAngle < float(Math::HALF_PI) )
			{
				if ( rfPAngle > float(-Math::HALF_PI) )
				{
					rfYAngle = Math::ATan2f(-m[1][2],m[2][2]);
					rfRAngle = Math::ATan2f(-m[0][1],m[0][0]);
					return true;
				}
				else
				{
					// WARNING.  Not a unique solution.
					float fRmY = Math::ATan2f(m[1][0],m[1][1]);
					rfRAngle = 0.f;  // any angle works
					rfYAngle = rfRAngle - fRmY;
					return false;
				}
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRpY = Math::ATan2f(m[1][0],m[1][1]);
				rfRAngle = 0.f;  // any angle works
				rfYAngle = fRpY - rfRAngle;
				return false;
			}
		}

        inline bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle,float& rfRAngle) const
		{
			// rot =  cy*cz          -sz              cz*sy
			//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
			//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

			rfPAngle = Math::ASinf(-m[0][1]);
			if ( rfPAngle < float(Math::HALF_PI) )
			{
				if ( rfPAngle > float(-Math::HALF_PI) )
				{
					rfYAngle = Math::ATan2f(m[2][1],m[1][1]);
					rfRAngle = Math::ATan2f(m[0][2],m[0][0]);
					return true;
				}
				else
				{
					// WARNING.  Not a unique solution.
					float fRmY = Math::ATan2f(-m[2][0],m[2][2]);
					rfRAngle = float(0.0);  // any angle works
					rfYAngle = rfRAngle - fRmY;
					return false;
				}
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRpY = Math::ATan2f(-m[2][0],m[2][2]);
				rfRAngle = float(0.0);  // any angle works
				rfYAngle = fRpY - rfRAngle;
				return false;
			}
		}

        inline bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle, float& rfRAngle) const
		{
			 // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
			//        cx*sz           cx*cz          -sx
			//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

			rfPAngle = Math::ASinf(-m[1][2]);
			if ( rfPAngle < float(Math::HALF_PI) )
			{
				if ( rfPAngle > float(-Math::HALF_PI) )
				{
					rfYAngle = Math::ATan2f(m[0][2],m[2][2]);
					rfRAngle = Math::ATan2f(m[1][0],m[1][1]);
					return true;
				}
				else
				{
					// WARNING.  Not a unique solution.
					float fRmY = Math::ATan2f(-m[0][1],m[0][0]);
					rfRAngle = float(0.0);  // any angle works
					rfYAngle = rfRAngle - fRmY;
					return false;
				}
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRpY = Math::ATan2f(-m[0][1],m[0][0]);
				rfRAngle = float(0.0);  // any angle works
				rfYAngle = fRpY - rfRAngle;
				return false;
			}
		}

        inline bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle,float& rfRAngle) const
		{
			 // rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
			//        sz              cx*cz          -cz*sx
			//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

			rfPAngle = Math::ASinf(m[1][0]);
			if ( rfPAngle < float(Math::HALF_PI) )
			{
				if ( rfPAngle > float(-Math::HALF_PI) )
				{
					rfYAngle = Math::ATan2f(-m[2][0],m[0][0]);
					rfRAngle = Math::ATan2f(-m[1][2],m[1][1]);
					return true;
				}
				else
				{
					// WARNING.  Not a unique solution.
					float fRmY = Math::ATan2f(m[2][1],m[2][2]);
					rfRAngle = float(0.0);  // any angle works
					rfYAngle = rfRAngle - fRmY;
					return false;
				}
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRpY = Math::ATan2f(m[2][1],m[2][2]);
				rfRAngle = float(0.0);  // any angle works
				rfYAngle = fRpY - rfRAngle;
				return false;
			}
		}

        inline bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle,float& rfRAngle) const
		{
			// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
			//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
			//       -cx*sy           sx              cx*cy

			rfPAngle = Math::ASinf(m[2][1]);
			if ( rfPAngle < float(Math::HALF_PI) )
			{
				if ( rfPAngle > float(-Math::HALF_PI) )
				{
					rfYAngle = Math::ATan2f(-m[0][1],m[1][1]);
					rfRAngle = Math::ATan2f(-m[2][0],m[2][2]);
					return true;
				}
				else
				{
					// WARNING.  Not a unique solution.
					float fRmY = Math::ATan2f(m[0][2],m[0][0]);
					rfRAngle = float(0.0);  // any angle works
					rfYAngle = rfRAngle - fRmY;
					return false;
				}
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRpY = Math::ATan2f(m[0][2],m[0][0]);
				rfRAngle = float(0.0);  // any angle works
				rfYAngle = fRpY - rfRAngle;
				return false;
			}

		}

        inline bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle, float& rfRAngle) const
		{
			 // rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
			//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
			//       -sy              cy*sx           cx*cy

			rfPAngle = Math::ASinf(-m[2][0]);
			if ( rfPAngle < float(Math::HALF_PI) )
			{
				if ( rfPAngle > float(-Math::HALF_PI) )
				{
					rfYAngle = Math::ATan2f(m[1][0],m[0][0]);
					rfRAngle = Math::ATan2f(m[2][1],m[2][2]);
					return true;
				}
				else
				{
					// WARNING.  Not a unique solution.
					float fRmY = Math::ATan2f(-m[0][1],m[0][2]);
					rfRAngle = float(0.0);  // any angle works
					rfYAngle = rfRAngle - fRmY;
					return false;
				}
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRpY = Math::ATan2f(-m[0][1],m[0][2]);
				rfRAngle = float(0.0);  // any angle works
				rfYAngle = fRpY - rfRAngle;
				return false;
			}
		}

        void EulerAnglesXYZ (const float fYAngle, const float fPAngle, const float fRAngle)
		{
			float fCos, fSin;

			fCos = cos(fYAngle);
			fSin = sin(fYAngle);
			Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

			fCos = Math::Cosf(fPAngle);
			fSin = Math::Sinf(fPAngle);
			Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

			fCos = Math::Cosf(fRAngle);
			fSin = Math::Sinf(fRAngle);
			Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

			*this = kXMat*(kYMat*kZMat);
		}

        void EulerAnglesXZY (const float fYAngle, const float fPAngle, const float fRAngle)
		{
			float fCos, fSin;

			fCos = Math::Cosf(fYAngle);
			fSin = Math::Sinf(fYAngle);
			Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

			fCos = Math::Cosf(fPAngle);
			fSin = Math::Sinf(fPAngle);
			Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

			fCos = Math::Cosf(fRAngle);
			fSin = Math::Sinf(fRAngle);
			Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

			*this = kXMat*(kZMat*kYMat);
		}

        void EulerAnglesYXZ (const float fYAngle, const float fPAngle, const float fRAngle)
		{
			float fCos, fSin;

			fCos = Math::Cosf(fYAngle);
			fSin = Math::Sinf(fYAngle);
			Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

			fCos = Math::Cosf(fPAngle);
			fSin = Math::Sinf(fPAngle);
			Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

			fCos = Math::Cosf(fRAngle);
			fSin = Math::Sinf(fRAngle);
			Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

			*this = kYMat*(kXMat*kZMat);
		}

        void EulerAnglesYZX (const float fYAngle, const float fPAngle, const float fRAngle)
		{
			float fCos, fSin;

			fCos = Math::Cosf(fYAngle);
			fSin = Math::Sinf(fYAngle);
			Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

			fCos = Math::Cosf(fPAngle);
			fSin = Math::Sinf(fPAngle);
			Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

			fCos = Math::Cosf(fRAngle);
			fSin = Math::Sinf(fRAngle);
			Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

			*this = kYMat*(kZMat*kXMat);
		}

        void EulerAnglesZXY (const float fYAngle, const float fPAngle, const float fRAngle)
		{
			float fCos, fSin;

			fCos = Math::Cosf(fYAngle);
			fSin = Math::Sinf(fYAngle);
			Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

			fCos = Math::Cosf(fPAngle);
			fSin = Math::Sinf(fPAngle);
			Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

			fCos = Math::Cosf(fRAngle);
			fSin = Math::Sinf(fRAngle);
			Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

			*this = kZMat*(kXMat*kYMat);
		}

        void EulerAnglesZYX (const float fYAngle, const float fPAngle, const float fRAngle)
		{
			float fCos, fSin;

			fCos = Math::Cosf(fYAngle);
			fSin = Math::Sinf(fYAngle);
			Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

			fCos = Math::Cosf(fPAngle);
			fSin = Math::Sinf(fPAngle);
			Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

			fCos = Math::Cosf(fRAngle);
			fSin = Math::Sinf(fRAngle);
			Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

			*this = kZMat*(kYMat*kXMat);
		}
       
		/** Determines if this matrix involves a scaling. */
		inline bool HasScale() const
		{
			// check magnitude of column vectors (==local axes)
			float t = (float)(m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0]);
			if (!Math::FloatEqual(t, 1.0, (float)1e-04))
				return true;
			t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
			if (!Math::FloatEqual(t, 1.0, (float)1e-04))
				return true;
			t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
			if (!Math::FloatEqual(t, 1.0, (float)1e-04))
				return true;

			return false;
		}

    protected:
 
        T m[3][3];

     
    };

	typedef TMatrix3<float> Matrix3f;

	template <typename T>
    class TMatrix4
    {
    protected:
        /// The matrix entries, indexed by [row][col].
        union {
            T m[4][4];
            T _m[16];
        };

		inline T MINOR(const size_t r0, const size_t r1, const size_t r2, 
								const size_t c0, const size_t c1, const size_t c2)
		{
			return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
				m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
				m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
		}

    public:
  
        inline TMatrix4()
        {
        }

        inline TMatrix4(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33 )
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }

        /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling 3x3 matrix.
         */

        inline TMatrix4(const TMatrix3<T>& m3x3)
        {
           memset(_m,0,sizeof(T)*16);
           operator=(m3x3);
		   m[3][3] = 1;
        }

        /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling Quaternion.
         */
        
        inline TMatrix4(const TQuaternion<T>& rot)
        {
            TMatrix3 m3x3;
            rot.ToRotationMatrix(m3x3);
            memset(_m,0,sizeof(T)*16);
            operator=(m3x3);
			m[3][3] = 1;
        }
 
		inline T* Data()
		{
			return _m;
		}

		inline T* operator [] ( size_t iRow )
        {
            return m[iRow];
        }

        inline const T *operator [] ( size_t iRow ) const
        {
            return m[iRow];
        }

        inline TMatrix4 Concatenate(const TMatrix4 &m2) const
        {
            TMatrix4 r;
            r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
            r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
            r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
            r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

            r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
            r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
            r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
            r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

            r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
            r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
            r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
            r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

            r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
            r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
            r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
            r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

            return r;
        }

        /** Matrix concatenation using '*'.
        */
        inline TMatrix4 operator * ( const TMatrix4 &m2 ) const
        {
            return Concatenate( m2 );
        }

        /** Vector transformation using '*'.
            @remarks
                Transforms the given 3-D vector by the matrix, projecting the 
                result back into <i>w</i> = 1.
            @note
                This means that the initial <i>w</i> is considered to be 1.0,
                and then all the tree elements of the resulting 3-D vector are
                divided by the resulting <i>w</i>.
        */
        inline TVector3<T> operator * ( const TVector3<T> &v ) const
        {
            TVector3<T> r;

            T fInvW = (T)(1.0 / ( m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] ));

            r.x = ( m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] ) * fInvW;
            r.y = ( m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] ) * fInvW;
            r.z = ( m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] ) * fInvW;

            return r;
        }

        inline TVector4<T> operator * (const TVector4<T>& v) const
        {
            return TVector4<T>(
                m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
                );
        }

     /*   inline Plane operator * (const Plane& p) const
        {
            Plane ret;
			Matrix4 invTrans = Inverse().Transpose();
			Vector4 v4( p.Normal.x, p.Normal.y, p.Normal.z, p.D );
			v4 = invTrans * v4;
			ret.Normal.x = v4.x; 
			ret.Normal.y = v4.y; 
			ret.Normal.z = v4.z;
			ret.D = v4.w / ret.Normal.Normalise();

            return ret;
        }*/


        /** Matrix addition.
        */
        inline TMatrix4 operator + ( const TMatrix4 &m2 ) const
        {
            TMatrix4 r;

            r.m[0][0] = m[0][0] + m2.m[0][0];
            r.m[0][1] = m[0][1] + m2.m[0][1];
            r.m[0][2] = m[0][2] + m2.m[0][2];
            r.m[0][3] = m[0][3] + m2.m[0][3];

            r.m[1][0] = m[1][0] + m2.m[1][0];
            r.m[1][1] = m[1][1] + m2.m[1][1];
            r.m[1][2] = m[1][2] + m2.m[1][2];
            r.m[1][3] = m[1][3] + m2.m[1][3];

            r.m[2][0] = m[2][0] + m2.m[2][0];
            r.m[2][1] = m[2][1] + m2.m[2][1];
            r.m[2][2] = m[2][2] + m2.m[2][2];
            r.m[2][3] = m[2][3] + m2.m[2][3];

            r.m[3][0] = m[3][0] + m2.m[3][0];
            r.m[3][1] = m[3][1] + m2.m[3][1];
            r.m[3][2] = m[3][2] + m2.m[3][2];
            r.m[3][3] = m[3][3] + m2.m[3][3];

            return r;
        }

        /** Matrix subtraction.
        */
        inline TMatrix4 operator - ( const TMatrix4 &m2 ) const
        {
            TMatrix4 r;
            r.m[0][0] = m[0][0] - m2.m[0][0];
            r.m[0][1] = m[0][1] - m2.m[0][1];
            r.m[0][2] = m[0][2] - m2.m[0][2];
            r.m[0][3] = m[0][3] - m2.m[0][3];

            r.m[1][0] = m[1][0] - m2.m[1][0];
            r.m[1][1] = m[1][1] - m2.m[1][1];
            r.m[1][2] = m[1][2] - m2.m[1][2];
            r.m[1][3] = m[1][3] - m2.m[1][3];

            r.m[2][0] = m[2][0] - m2.m[2][0];
            r.m[2][1] = m[2][1] - m2.m[2][1];
            r.m[2][2] = m[2][2] - m2.m[2][2];
            r.m[2][3] = m[2][3] - m2.m[2][3];

            r.m[3][0] = m[3][0] - m2.m[3][0];
            r.m[3][1] = m[3][1] - m2.m[3][1];
            r.m[3][2] = m[3][2] - m2.m[3][2];
            r.m[3][3] = m[3][3] - m2.m[3][3];

            return r;
        }

        /** Tests 2 matrices for equality.
        */
        inline bool operator == ( const TMatrix4& m2 ) const
        {
            if( m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
                m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
                m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
                m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
                return false;

            return true;
        }

        /** Tests 2 matrices for inequality.
        */
        inline bool operator != ( const TMatrix4& m2 ) const
        {
            if( m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
                m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
                m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
                m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
                return true;

            return false;
        }

        /** Assignment from 3x3 matrix.
        */
        inline void operator = ( const TMatrix3<T>& mat3 )
        {
            m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
            m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
            m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
        }

        inline TMatrix4 Transpose(void) const
        {
            return TMatrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                            m[0][1], m[1][1], m[2][1], m[3][1],
                            m[0][2], m[1][2], m[2][2], m[3][2],
                            m[0][3], m[1][3], m[2][3], m[3][3]);
        }

        /*
        -----------------------------------------------------------------------
        Translation Transformation
        -----------------------------------------------------------------------
        */
        /** Sets the translation transformation part of the matrix.
        */
        inline void Translate( const TVector3<T>& v )
        {
            m[0][3] = v.x;
            m[1][3] = v.y;
            m[2][3] = v.z;
        }

        /** Extracts the translation transformation part of the matrix.
         */
        inline TVector3<T> GetTranslate() const
        {
          return TVector3<T>(m[0][3], m[1][3], m[2][3]);
        }
        

        /** Builds a translation matrix
        */
        inline void MakeTranslate( const TVector3<T>& v )
        {
            m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
            m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }

        inline void MakeTranslate( T tx, T ty, T tz )
        {
            m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
            m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }
 
        /*
        -----------------------------------------------------------------------
        Scale Transformation
        -----------------------------------------------------------------------
        */
        /** Sets the scale part of the matrix.
        */
        inline void Scale( const TVector3<T>& v )
        {
            m[0][0] = v.x;
            m[1][1] = v.y;
            m[2][2] = v.z;
        }

		inline bool IdentityScale()
		{
			if(Math::FloatEqual(m[0][0], m[1][1], (float)1e-04) && Math::FloatEqual(m[0][0], m[2][2], (float)1e-04) )
				return true;

			return false;
		}

		inline TMatrix4 GetNormalMatrix()
		{
			if(Math::FloatEqual(m[0][0], m[1][1], (float)1e-04) && Math::FloatEqual(m[0][0], m[2][2], (float)1e-04) )
			{
				return Inverse().Transpose();
			}

			return *this;
		}


        /** Gets a scale matrix.
        */
        inline static TMatrix4 GetScaleMatrix( const TVector3<T>& v )
        {
            Matrix4 r;
            r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
            r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
            r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
            r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

            return r;
        }

        /** Gets a scale matrix - variation for not using a vector.
        */
        inline static TMatrix4 GetScaleMatrix( T s_x, T s_y, T s_z )
        {
            Matrix4 r;
            r.m[0][0] = s_x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
            r.m[1][0] = 0.0; r.m[1][1] = s_y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
            r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = s_z; r.m[2][3] = 0.0;
            r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

            return r;
        }

        /** Extracts the rotation / scaling part of the Matrix as a 3x3 matrix. 
        @param m3x3 Destination Matrix3
        */
        inline void Extract3x3Matrix(TMatrix3<T>& m3x3) const
        {
            m3x3.m[0][0] = m[0][0];
            m3x3.m[0][1] = m[0][1];
            m3x3.m[0][2] = m[0][2];
            m3x3.m[1][0] = m[1][0];
            m3x3.m[1][1] = m[1][1];
            m3x3.m[1][2] = m[1][2];
            m3x3.m[2][0] = m[2][0];
            m3x3.m[2][1] = m[2][1];
            m3x3.m[2][2] = m[2][2];

        }

		/** Determines if this matrix involves a scaling. */
		inline bool HasScale() const
		{
			// check magnitude of column vectors (==local axes)
			float t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
			if (!Math::FloatEqual(t, 1.0, (float)1e-04))
				return true;
			t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
			if (!Math::FloatEqual(t, 1.0, (float)1e-04))
				return true;
			t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
			if (!Math::FloatEqual(t, 1.0, (float)1e-04))
				return true;

			return false;
		}

		/** Determines if this matrix involves a negative scaling. */
		inline bool HasNegativeScale() const
		{
			return Determinant() < 0;
		}

		/** Extracts the rotation / scaling part as a quaternion from the Matrix.
         */
        inline TQuaternion<T> ExtractQuaternion() const
        {
           TMatrix3<T> m3x3;
           Extract3x3Matrix(m3x3);
           return TQuaternion<T>(m3x3);
        }
 
        inline TMatrix4 operator*(T scalar) const
        {
            return TMatrix4( scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
							 scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
							 scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
							 scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
        }
 
		//-----------------------------------------------------------------------
		inline TMatrix4 Adjoint() const
		{
			return TMatrix4(
				         MINOR(1, 2, 3, 1, 2, 3),
						-MINOR( 0, 2, 3, 1, 2, 3),
						 MINOR( 0, 1, 3, 1, 2, 3),
						-MINOR( 0, 1, 2, 1, 2, 3),

						-MINOR( 1, 2, 3, 0, 2, 3),
						 MINOR( 0, 2, 3, 0, 2, 3),
						-MINOR( 0, 1, 3, 0, 2, 3),
						 MINOR( 0, 1, 2, 0, 2, 3),

						 MINOR( 1, 2, 3, 0, 1, 3),
						-MINOR( 0, 2, 3, 0, 1, 3),
						 MINOR( 0, 1, 3, 0, 1, 3),
						-MINOR( 0, 1, 2, 0, 1, 3),

						-MINOR( 1, 2, 3, 0, 1, 2),
						 MINOR(0, 2, 3, 0, 1, 2),
						-MINOR( 0, 1, 3, 0, 1, 2),
						 MINOR( 0, 1, 2, 0, 1, 2));
		}

		//-----------------------------------------------------------------------
		inline float Determinant() const
		{
			return m[0][0] * MINOR(1, 2, 3, 1, 2, 3) -
				   m[0][1] * MINOR(1, 2, 3, 0, 2, 3) +
				   m[0][2] * MINOR(1, 2, 3, 0, 1, 3) -
				   m[0][3] * MINOR(1, 2, 3, 0, 1, 2);
		}
		
 
		inline TMatrix4 Inverse() const
		{
			T m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
			T m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
			T m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
			T m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

			T v0 = m20 * m31 - m21 * m30;
			T v1 = m20 * m32 - m22 * m30;
			T v2 = m20 * m33 - m23 * m30;
			T v3 = m21 * m32 - m22 * m31;
			T v4 = m21 * m33 - m23 * m31;
			T v5 = m22 * m33 - m23 * m32;

			T t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
			T t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
			T t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
			T t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

			T invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

			T d00 = t00 * invDet;
			T d10 = t10 * invDet;
			T d20 = t20 * invDet;
			T d30 = t30 * invDet;

			T d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
			T d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
			T d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
			T d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

			v0 = m10 * m31 - m11 * m30;
			v1 = m10 * m32 - m12 * m30;
			v2 = m10 * m33 - m13 * m30;
			v3 = m11 * m32 - m12 * m31;
			v4 = m11 * m33 - m13 * m31;
			v5 = m12 * m33 - m13 * m32;

			T d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
			T d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
			T d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
			T d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

			v0 = m21 * m10 - m20 * m11;
			v1 = m22 * m10 - m20 * m12;
			v2 = m23 * m10 - m20 * m13;
			v3 = m22 * m11 - m21 * m12;
			v4 = m23 * m11 - m21 * m13;
			v5 = m23 * m12 - m22 * m13;

			T d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
			T d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
			T d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
			T d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

			return TMatrix4(
				d00, d01, d02, d03,
				d10, d11, d12, d13,
				d20, d21, d22, d23,
				d30, d31, d32, d33);
		}

        /** Building a Matrix4 from orientation / scale / position.
        @remarks
            Transform is performed in the order scale, rotate, translation, i.e. translation is independent
            of orientation axes, scale does not affect size of translation, rotation and scaling are always
            centered on the origin.
        */
        void MakeTransform(const TVector3<T>& position, const TVector3<T>& scale, const TQuaternion<T>& orientation)
		{
			// Ordering:
			//    1. Scale
			//    2. Rotate
			//    3. Translate

			TMatrix3<T> rot3x3;
			orientation.ToRotationMatrix(rot3x3);

			// Set up final matrix with scale, rotation and translation
			m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
			m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
			m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;

			// No projection term
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
		}

		inline void MakeTransform(const TVector3<T>& position, const TVector3<T>& scale, const TVector3<T>& orientation)
		{
			TMatrix3 rot3x3;
			rot3x3.EulerAnglesXYZ(orientation.x,orientation.y,orientation.z);
			 // Set up final matrix with scale, rotation and translation
			m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
			m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
			m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;

			// No projection term
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
		}
        /** Building an inverse Matrix4 from orientation / scale / position.
        @remarks
            As makeTransform except it build the inverse given the same data as makeTransform, so
            performing -translation, -rotate, 1/scale in that order.
        */
        inline void MakeInverseTransform(const TVector3<T>& position, const TVector3<T>& scale, const TQuaternion<T>& orientation)
		{
			 // Invert the parameters
			TVector3<T> invTranslate = -position;
			TVector3<T> invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
			TQuaternion<T> invRot = orientation.Inverse();

			// Because we're inverting, order is translation, rotation, scale
			// So make translation relative to scale & rotation
			invTranslate = invRot * invTranslate; // rotate
			invTranslate *= invScale; // scale

			// Next, make a 3x3 rotation matrix
			TMatrix3<T> rot3x3;
			invRot.ToRotationMatrix(rot3x3);

			// Set up final matrix with scale, rotation and translation
			m[0][0] = invScale.x * rot3x3[0][0]; m[0][1] = invScale.x * rot3x3[0][1]; m[0][2] = invScale.x * rot3x3[0][2]; m[0][3] = invTranslate.x;
			m[1][0] = invScale.y * rot3x3[1][0]; m[1][1] = invScale.y * rot3x3[1][1]; m[1][2] = invScale.y * rot3x3[1][2]; m[1][3] = invTranslate.y;
			m[2][0] = invScale.z * rot3x3[2][0]; m[2][1] = invScale.z * rot3x3[2][1]; m[2][2] = invScale.z * rot3x3[2][2]; m[2][3] = invTranslate.z;		

			// No projection term
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
		}

        /** Decompose a Matrix4 to orientation / scale / position.
        */
        inline void Decomposition(TVector3<T>& position, TVector3<T>& scale, TQuaternion<T>& orientation) const
		{
			BOOST_ASSERT(IsAffine());

			TMatrix3<T> m3x3;
			Extract3x3Matrix(m3x3);

			Matrix3 matQ;
			Vector3 vecU;
			m3x3.QDUDecomposition( matQ, scale, vecU ); 

			orientation = Quaternion( matQ );
			position = TVector3<T>( m[0][3], m[1][3], m[2][3] );
		}

        /** Check whether or not the matrix is affine matrix.
            @remarks
                An affine matrix is a 4x4 matrix with row 3 equal to (0, 0, 0, 1),
                e.g. no projective coefficients.
        */
        inline bool IsAffine(void) const
        {
            return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
        }

        /** Returns the inverse of the affine matrix.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        TMatrix4 InverseAffine(void) const
		{
			 BOOST_ASSERT(IsAffine());

			T m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
			T m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

			T t00 = m22 * m11 - m21 * m12;
			T t10 = m20 * m12 - m22 * m10;
			T t20 = m21 * m10 - m20 * m11;

			T m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];

			T invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

			t00 *= invDet; t10 *= invDet; t20 *= invDet;

			m00 *= invDet; m01 *= invDet; m02 *= invDet;

			T r00 = t00;
			T r01 = m02 * m21 - m01 * m22;
			T r02 = m01 * m12 - m02 * m11;

			T r10 = t10;
			T r11 = m00 * m22 - m02 * m20;
			T r12 = m02 * m10 - m00 * m12;

			T r20 = t20;
			T r21 = m01 * m20 - m00 * m21;
			T r22 = m00 * m11 - m01 * m10;

			T m03 = m[0][3], m13 = m[1][3], m23 = m[2][3];

			T r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
			T r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
			T r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

			return TMatrix4(
				r00, r01, r02, r03,
				r10, r11, r12, r13,
				r20, r21, r22, r23,
				  0,   0,   0,   1);
		}

        /** Concatenate two affine matrices.
            @note
                The matrices must be affine matrix. @see Matrix4::isAffine.
        */
        inline TMatrix4 ConcatenateAffine(const TMatrix4 &m2) const
        {
            BOOST_ASSERT(IsAffine() && m2.IsAffine());

            return TMatrix4(
                m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0],
                m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1],
                m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2],
                m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3],

                m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0],
                m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1],
                m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2],
                m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3],

                m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0],
                m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1],
                m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2],
                m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3],

                0, 0, 0, 1);
        }

        /** 3-D Vector transformation specially for an affine matrix.
            @remarks
                Transforms the given 3-D vector by the matrix, projecting the 
                result back into <i>w</i> = 1.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        inline TVector3<T> TransformAffine(const TVector3<T>& v) const
        {
            BOOST_ASSERT(IsAffine());

            return Vector3(
                    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3], 
                    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
        }

        /** 4-D Vector transformation specially for an affine matrix.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
        inline TVector4<T> TransformAffine(const TVector4<T>& v) const
        {
            BOOST_ASSERT(IsAffine());

            return TVector4<T>(
                m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                v.w);
        }
    };

	typedef TMatrix4<float> Matrix4f;
 
}


#endif
