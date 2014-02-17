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

        TMatrix3 Inverse (float fTolerance = 1e-06) const
		{
			TMatrix3 kInverse;
            Inverse(kInverse,fTolerance);
            return kInverse;
		}

        T Determinant () const
		{
			T fCofactor00 = m[1][1]*m[2][2] - m[1][2]*m[2][1];
			T fCofactor10 = m[1][2]*m[2][0] - m[1][0]*m[2][2];
			T fCofactor20 = m[1][0]*m[2][1] - m[1][1]*m[2][0];

			T fDet = m[0][0]*fCofactor00 + m[0][1]*fCofactor10 + m[0][2]*fCofactor20;
			return fDet;
		}
 
        void Rotate (const TVector3<T>& rkAxis, const float& fRadians)
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
        bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
        bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
        bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
        bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
        bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
        bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
        void FromEulerAnglesXYZ (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesXZY (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesYXZ (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesYZX (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesZXY (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesZYX (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        /// Eigensolver, matrix must be symmetric
        void EigenSolveSymmetric (float afEigenvalue[3],
            Vector3 akEigenvector[3]) const;

        static void TensorProduct (const Vector3& rkU, const Vector3& rkV,
            Matrix3& rkProduct);

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
 
        static const float EPSILON;
        static const Matrix3 ZERO;
        static const Matrix3 IDENTITY;

    protected:
        // support for eigensolver
        void Tridiagonal (float afDiag[3], float afSubDiag[3]);
        bool QLAlgorithm (float afDiag[3], float afSubDiag[3]);

        // support for singular value decomposition
        static const float msSvdEpsilon;
        static const unsigned int msSvdMaxIterations;
        static void Bidiagonalize (Matrix3& kA, Matrix3& kL,
            Matrix3& kR);
        static void GolubKahanStep (Matrix3& kA, Matrix3& kL,
            Matrix3& kR);

        // support for spectral norm
        static float MaxCubicRoot (float afCoeff[3]);

        float m[3][3];

        // for faster access
        friend class Matrix4;
    };


	//------------------------------------------------------------------------------------------------------------------------
		/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/
	/** Class encapsulating a standard 4x4 homogeneous matrix.
        @remarks
            OGRE uses column vectors when applying matrix multiplications,
            This means a vector is represented as a single column, 4-row
            matrix. This has the effect that the transformations implemented
            by the matrices happens right-to-left e.g. if vector V is to be
            transformed by M1 then M2 then M3, the calculation would be
            M3 * M2 * M1 * V. The order that matrices are concatenated is
            vital since matrix multiplication is not commutative, i.e. you
            can get a different result if you concatenate in the wrong order.
        @par
            The use of column vectors and right-to-left ordering is the
            standard in most mathematical texts, and is the same as used in
            OpenGL. It is, however, the opposite of Direct3D, which has
            inexplicably chosen to differ from the accepted standard and uses
            row vectors and left-to-right matrix multiplication.
        @par
            OGRE deals with the differences between D3D and OpenGL etc.
            internally when operating through different render systems. OGRE
            users only need to conform to standard maths conventions, i.e.
            right-to-left matrix multiplication, (OGRE transposes matrices it
            passes to D3D to compensate).
        @par
            The generic form M * V which shows the layout of the matrix 
            entries is shown below:
            <pre>
                [ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
                | m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
                | m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
                [ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
            </pre>
    */
    class Matrix4
    {
    protected:
        /// The matrix entries, indexed by [row][col].
        union {
            float m[4][4];
            float _m[16];
        };
    public:
        /** Default constructor.
            @note
                It does <b>NOT</b> initialize the matrix for efficiency.
        */
        inline Matrix4()
        {
        }

        inline Matrix4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33 )
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

        inline Matrix4(const Matrix3& m3x3)
        {
          operator=(IDENTITY);
          operator=(m3x3);
        }

        /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling Quaternion.
         */
        
        inline Matrix4(const Quaternion& rot)
        {
          Matrix3 m3x3;
          rot.ToRotationMatrix(m3x3);
          operator=(IDENTITY);
          operator=(m3x3);
        }
        

		/** Exchange the contents of this matrix with another. 
		*/
		inline void Swap(Matrix4& other)
		{
			std::swap(m[0][0], other.m[0][0]);
			std::swap(m[0][1], other.m[0][1]);
			std::swap(m[0][2], other.m[0][2]);
			std::swap(m[0][3], other.m[0][3]);
			std::swap(m[1][0], other.m[1][0]);
			std::swap(m[1][1], other.m[1][1]);
			std::swap(m[1][2], other.m[1][2]);
			std::swap(m[1][3], other.m[1][3]);
			std::swap(m[2][0], other.m[2][0]);
			std::swap(m[2][1], other.m[2][1]);
			std::swap(m[2][2], other.m[2][2]);
			std::swap(m[2][3], other.m[2][3]);
			std::swap(m[3][0], other.m[3][0]);
			std::swap(m[3][1], other.m[3][1]);
			std::swap(m[3][2], other.m[3][2]);
			std::swap(m[3][3], other.m[3][3]);
		}

		inline float* operator [] ( size_t iRow )
        {
            BOOST_ASSERT( iRow < 4 );
            return m[iRow];
        }

        inline const float *operator [] ( size_t iRow ) const
        {
            BOOST_ASSERT( iRow < 4 );
            return m[iRow];
        }

        inline Matrix4 Concatenate(const Matrix4 &m2) const
        {
            Matrix4 r;
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
        inline Matrix4 operator * ( const Matrix4 &m2 ) const
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
        inline Vector3 operator * ( const Vector3 &v ) const
        {
            Vector3 r;

            float fInvW = 1.0f / ( m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] );

            r.x = ( m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] ) * fInvW;
            r.y = ( m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] ) * fInvW;
            r.z = ( m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] ) * fInvW;

            return r;
        }
        inline Vector4 operator * (const Vector4& v) const
        {
            return Vector4(
                m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
                );
        }
        inline Plane operator * (const Plane& p) const
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
        }


        /** Matrix addition.
        */
        inline Matrix4 operator + ( const Matrix4 &m2 ) const
        {
            Matrix4 r;

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
        inline Matrix4 operator - ( const Matrix4 &m2 ) const
        {
            Matrix4 r;
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
        inline bool operator == ( const Matrix4& m2 ) const
        {
            if( 
                m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
                m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
                m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
                m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
                return false;
            return true;
        }

        /** Tests 2 matrices for inequality.
        */
        inline bool operator != ( const Matrix4& m2 ) const
        {
            if( 
                m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
                m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
                m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
                m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
                return true;
            return false;
        }

        /** Assignment from 3x3 matrix.
        */
        inline void operator = ( const Matrix3& mat3 )
        {
            m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
            m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
            m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
        }

        inline Matrix4 Transpose(void) const
        {
            return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
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
        inline void SetTrans( const Vector3& v )
        {
            m[0][3] = v.x;
            m[1][3] = v.y;
            m[2][3] = v.z;
        }

        /** Extracts the translation transformation part of the matrix.
         */
        inline Vector3 GetTrans() const
        {
          return Vector3(m[0][3], m[1][3], m[2][3]);
        }
        

        /** Builds a translation matrix
        */
        inline void MakeTrans( const Vector3& v )
        {
            m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
            m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }

        inline void MakeTrans( float tx, float ty, float tz )
        {
            m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
            m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }

        /** Gets a translation matrix.
        */
        inline static Matrix4 GetTrans( const Vector3& v )
        {
            Matrix4 r;

            r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = v.x;
            r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = v.y;
            r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = v.z;
            r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

            return r;
        }

        /** Gets a translation matrix - variation for not using a vector.
        */
        inline static Matrix4 GetTrans( float t_x, float t_y, float t_z )
        {
            Matrix4 r;

            r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = t_x;
            r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = t_y;
            r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = t_z;
            r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

            return r;
        }

        /*
        -----------------------------------------------------------------------
        Scale Transformation
        -----------------------------------------------------------------------
        */
        /** Sets the scale part of the matrix.
        */
        inline void SetScale( const Vector3& v )
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

		inline Matrix4 GetNormalMatrix()
		{
			if(Math::FloatEqual(m[0][0], m[1][1], (float)1e-04) && Math::FloatEqual(m[0][0], m[2][2], (float)1e-04) )
			{
				return Inverse().Transpose();
			}

			return *this;
		}


        /** Gets a scale matrix.
        */
        inline static Matrix4 GetScale( const Vector3& v )
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
        inline static Matrix4 GetScale( float s_x, float s_y, float s_z )
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
        inline void Extract3x3Matrix(Matrix3& m3x3) const
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
        inline Quaternion ExtractQuaternion() const
        {
          Matrix3 m3x3;
          Extract3x3Matrix(m3x3);
          return Quaternion(m3x3);
        }

		static const Matrix4 ZERO;
		static const Matrix4 ZEROAFFINE;
		static const Matrix4 IDENTITY;
        /** Useful little matrix which takes 2D clipspace {-1, 1} to {0,1}
            and inverts the Y. */
        static const Matrix4 CLIPSPACE2DTOIMAGESPACE;

        inline Matrix4 operator*(float scalar) const
        {
            return Matrix4(
                scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
                scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
                scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
                scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
        }

        
		
		Matrix4 Adjoint() const;
		float Determinant() const;
		Matrix4 Inverse() const;

        /** Building a Matrix4 from orientation / scale / position.
        @remarks
            Transform is performed in the order scale, rotate, translation, i.e. translation is independent
            of orientation axes, scale does not affect size of translation, rotation and scaling are always
            centered on the origin.
        */
        void MakeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);
		void MakeTransform(const Vector3& position, const Vector3& scale, const Vector3& orientation);
        /** Building an inverse Matrix4 from orientation / scale / position.
        @remarks
            As makeTransform except it build the inverse given the same data as makeTransform, so
            performing -translation, -rotate, 1/scale in that order.
        */
        void MakeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);

        /** Decompose a Matrix4 to orientation / scale / position.
        */
        void Decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const;

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
        Matrix4 InverseAffine(void) const;

        /** Concatenate two affine matrices.
            @note
                The matrices must be affine matrix. @see Matrix4::isAffine.
        */
        inline Matrix4 ConcatenateAffine(const Matrix4 &m2) const
        {
            BOOST_ASSERT(IsAffine() && m2.IsAffine());

            return Matrix4(
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
        inline Vector3 TransformAffine(const Vector3& v) const
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
        inline Vector4 TransformAffine(const Vector4& v) const
        {
            BOOST_ASSERT(IsAffine());

            return Vector4(
                m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                v.w);
        }
    };

    /* Removed from Vector4 and made a non-member here because otherwise
       OgreMatrix4.h and OgreVector4.h have to try to include and inline each 
       other, which frankly doesn't work ;)
   */
    inline Vector4 operator * (const Vector4& v, const Matrix4& mat)
    {
        return Vector4(
            v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
            v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
            v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
            v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
            );
    }
 
}


#endif