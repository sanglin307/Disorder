#ifndef _DISORDER_VECTOR_H_
#define _DISORDER_VECTOR_H_

namespace Disorder
{
	////////////////////////////////////////////////////////////////////////////////////
	//// these vector used to mesh inside vertexs, they are lite ///////////////////////
	//// we don't use huge Eigen related vector inside mesh     /////////////////////// 
	///////////////////////////////////////////////////////////////////////////////////

	template <typename T>
    class  TVector2
    {
    public:
        T x, y;

    public:
        inline TVector2()
        {
        }

		inline TVector2(const Eigen::Vector2f& vec)
		{
			x = (T)(vec.x());
			y = (T)(vec.y());
		}

		inline TVector2(const T fX, const T fY )
		{
			x = fX;
			y = fY;
		}
 
        inline explicit TVector2( const T scaler )
            : x( scaler), y( scaler )
        {
        }
 
      
		inline T operator [] ( const size_t i ) const
        {
            return *(&x+i);
        }

		inline T& operator [] ( const size_t i )
        {
            return *(&x+i);
        }

		/// Pointer accessor for direct copying
		inline T* Ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const T* Ptr() const
		{
			return &x;
		}

        /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline TVector2& operator = ( const TVector2& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }

		inline TVector2& operator = ( const Eigen::Vector2f& rkVector )
        {
            x = (T)(rkVector.x());
            y = (T)(rkVector.y());

            return *this;
        }

		inline TVector2& operator = ( const T fScalar)
		{
			x = fScalar;
			y = fScalar;

			return *this;
		}

        inline bool operator == ( const TVector2& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y );
        }

        inline bool operator != ( const TVector2& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y  );
        }

        // arithmetic operations
        inline TVector2 operator + ( const TVector2& rkVector ) const
        {
            return TVector2(
                x + rkVector.x,
                y + rkVector.y);
        }

        inline TVector2 operator - ( const TVector2& rkVector ) const
        {
            return TVector2(
                x - rkVector.x,
                y - rkVector.y);
        }

        inline TVector2 operator * ( const T fScalar ) const
        {
            return TVector2(
                x * fScalar,
                y * fScalar);
        }

        inline TVector2 operator * ( const TVector2& rhs) const
        {
            return TVector2(
                x * rhs.x,
                y * rhs.y);
        }

        inline TVector2 operator / ( const T fScalar ) const
		{
            return TVector2(
                x / fScalar,
                y / fScalar);
        }

        inline TVector2 operator / ( const TVector2& rhs) const
        {
            return TVector2(
                x / rhs.x,
                y / rhs.y);
        }

        inline const TVector2& operator + () const
        {
            return *this;
        }

        inline TVector2 operator - () const
        {
            return TVector2(-x, -y);
        }

        // overloaded operators to help Vector2f
        inline friend TVector2 operator * ( const T fScalar, const TVector2& rkVector )
        {
            return TVector2(
                fScalar * rkVector.x,
                fScalar * rkVector.y);
        }

        inline friend TVector2 operator / ( const T fScalar, const TVector2& rkVector )
        {
            return TVector2(
                fScalar / rkVector.x,
                fScalar / rkVector.y);
        }

        inline friend TVector2 operator + (const TVector2& lhs, const T rhs)
        {
            return TVector2(
                lhs.x + rhs,
                lhs.y + rhs);
        }

        inline friend TVector2 operator + (const T lhs, const TVector2& rhs)
        {
            return TVector2(
                lhs + rhs.x,
                lhs + rhs.y);
        }

        inline friend TVector2 operator - (const TVector2& lhs, const T rhs)
        {
            return Vector2f(
                lhs.x - rhs,
                lhs.y - rhs);
        }

        inline friend TVector2 operator - (const T lhs, const TVector2& rhs)
        {
            return TVector2(
                lhs - rhs.x,
                lhs - rhs.y);
        }

        // arithmetic updates
        inline TVector2& operator += ( const TVector2& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }

        inline TVector2& operator += ( const T fScaler )
        {
            x += fScaler;
            y += fScaler;

            return *this;
        }

        inline TVector2& operator -= ( const TVector2& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline TVector2& operator -= ( const T fScaler )
        {
            x -= fScaler;
            y -= fScaler;

            return *this;
        }

        inline TVector2& operator *= ( const T fScalar )
        {
            x *= fScalar;
            y *= fScalar;

            return *this;
        }

        inline TVector2& operator *= ( const TVector2& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline TVector2& operator /= ( const T fScalar )
        {
            x /= fScalar;
            y /= fScalar;

            return *this;
        }

        inline TVector2& operator /= ( const TVector2& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;

            return *this;
        }
 
        inline T Length () const
        {
			return (T)sqrt( x * x + y * y );
        }
 
        inline T SquaredLength () const
        {
            return x * x + y * y;
        }

      
        inline T Distance(const TVector2& rhs) const
        {
            return (*this - rhs).Length();
        }

       
        inline T SquaredDistance(const TVector2& rhs) const
        {
			return (*this - rhs).SquaredLength();
        }

       
        inline T Dot(const TVector2& vec) const
        {
            return x * vec.x + y * vec.y;
        }
 
        inline T Normalise()
        {
            T fLength = Length();
 
			if ( fLength > 0 && fLength != 1.0f )
            {
                T fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
            }

            return fLength;
        }
 
        inline bool operator < ( const TVector2& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
                return true;
            return false;
        }

        inline bool operator > ( const TVector2& rhs ) const
        {
            if( x > rhs.x && y > rhs.y )
                return true;
            return false;
        }
 
        inline TVector2 Perpendicular(void) const
        {
            return TVector2 (-y, x);
        }
 
		// equal to the area of parallelogram
        inline T Cross( const TVector2& rkVector ) const
        {
            return x * rkVector.y - y * rkVector.x;
        }
 
        /** Returns true if this vector is zero length. */
        inline bool IsZeroLength(void) const
        {
            T sqlen = (x * x) + (y * y);
            return (sqlen < 1e-08);

        }
 
        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline TVector2 Reflect(const TVector2& normal) const
        {
			return TVector2( *this - ( 2 * this->Dot(normal) * normal ) );
        }

	    /// Check whether this vector contains valid values
		inline bool IsNaN() const
		{
			return _isnan(x) || _isnan(y);
		}

		/**	 Gets the angle between 2 vectors.
		@remarks
			Vectors do not have to be unit-length but must represent directions.
		*/
		inline T AngleBetween(const TVector2& other) const
		{		
			T lenProduct = Length() * other.Length();
			// Divide by zero check
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;
		
			T f = Dot(other) / lenProduct;
	
			f = Clamp<T>(f, -1.0, 1.0);
			return acos(f);
		}

    };

	typedef TVector2<float> Vector2f;
	typedef TVector2<double> Vector2d;
	typedef TVector2<int> Vector2i;

	template <typename T> 
    class TVector3
    {
    public:
		T x, y, z;

    public:
        inline TVector3()
        {
        }
 
		 inline TVector3( const Eigen::Vector3f& vec )
			 : x( vec.x() ), y( vec.y() ), z( vec.z() )
        {
        }

        inline TVector3( const T fX, const T fY, const T fZ )
            : x( fX ), y( fY ), z( fZ )
        {
        }
 
        inline explicit TVector3( const T scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }
 
		inline T operator [] ( const size_t i ) const
        {
            return *(&x+i);
        }

		inline T& operator [] ( const size_t i )
        {
            return *(&x+i);
        }
		/// Pointer accessor for direct copying
		inline T* Ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const T* Ptr() const
		{
			return &x;
		}

        /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline TVector3& operator = ( const TVector3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

		 inline TVector3& operator = ( const Eigen::Vector3f& rkVector )
        {
            x = (T)(rkVector.x());
            y = (T)(rkVector.y());
            z = (T)(rkVector.z());

            return *this;
        }

        inline TVector3& operator = ( const T fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const TVector3& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const TVector3& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        // arithmetic operations
        inline TVector3 operator + ( const TVector3& rkVector ) const
        {
            return TVector3(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline TVector3 operator - ( const TVector3& rkVector ) const
        {
            return TVector3(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline TVector3 operator * ( const T fScalar ) const
        {
            return TVector3(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline TVector3 operator * ( const TVector3& rhs) const
        {
            return TVector3(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline TVector3 operator / ( const T fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            T fInv = 1.0f / fScalar;

            return TVector3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline TVector3 operator / ( const TVector3& rhs) const
        {
            return TVector3(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const TVector3& operator + () const
        {
            return *this;
        }

        inline TVector3 operator - () const
        {
            return TVector3(-x, -y, -z);
        }

        // overloaded operators to help Vector3f
        inline friend TVector3 operator * ( const T fScalar, const TVector3& rkVector )
        {
            return TVector3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend TVector3 operator / ( const T fScalar, const TVector3& rkVector )
        {
            return TVector3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend TVector3 operator + (const TVector3& lhs, const T rhs)
        {
            return TVector3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend TVector3 operator + (const T lhs, const TVector3& rhs)
        {
            return TVector3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend TVector3 operator - (const TVector3& lhs, const T rhs)
        {
            return TVector3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend TVector3 operator - (const T lhs, const TVector3& rhs)
        {
            return TVector3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline TVector3& operator += ( const TVector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline TVector3& operator += ( const T fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline TVector3& operator -= ( const TVector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline TVector3& operator -= ( const T fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline TVector3& operator *= ( const T fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline TVector3& operator *= ( const TVector3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline TVector3& operator /= ( const T fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            T fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline TVector3& operator /= ( const TVector3& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

            return *this;
        }
 
        inline T Length () const
        {
			return sqrt( x * x + y * y + z * z );
        }

        
        inline T SquaredLength () const
        {
            return x * x + y * y + z * z;
        }

        
        inline T Distance(const TVector3& rhs) const
        {
            return (*this - rhs).Length();
        }

        
        inline T SquaredDistance(const TVector3& rhs) const
        {
            return (*this - rhs).SquaredLength();
        }

        
        inline T Dot(const TVector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
 
        inline T AbsDot(const TVector3& vec) const
        {
            return Abs(x * vec.x) + Abs(y * vec.y) + Abs(z * vec.z);
        }

       
        inline T Normalise()
        {
			T fLength = sqrt( x * x + y * y + z * z );

			if ( fLength > 0.0f && fLength != 1.0f )
            {
                T fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }
 
        inline TVector3 Cross( const TVector3& rkVector ) const
        {
            return TVector3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }
 
        inline bool operator < ( const TVector3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        
        inline bool operator > ( const TVector3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }
 
        
        inline TVector3 Perpendicular(void) const
        {
            static const T fSquareZero = (T)(1e-06 * 1e-06);

			TVector3 perp = this->Cross( TVector3<T>(1,0,0) );

            // Check length
            if( perp.SquaredLength() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a scalar, so we have
                   to use another axis.
                */
				perp = this->Cross( TVector3<T>(0,1,0) );
            }

			perp.Normalise();

            return perp;
        }
       
		/** Gets the angle between 2 vectors.
		*/
		inline T AngleBetween(const TVector3& dest) const
		{
			T lenProduct = Length() * dest.Length();

			// Divide by zero check
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;

		    T f = Dot(dest) / lenProduct;

			f = Clamp(f, -1.0, 1.0);
			return acos(f);

		}

        /** Returns true if this vector is zero length. */
        inline bool IsZeroLength(void) const
        {
            T sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-08));

        }
 
        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline TVector3 Reflect(const TVector3& normal) const
        {
            return TVector3( *this - ( 2 * this->Dot(normal) * normal ) );
        }

		/** Returns whether this vector is within a positional tolerance
			of another vector.
		@param rhs The vector to compare with
		@param tolerance The amount that each element of the vector may vary by
			and still be considered equal
		*/
		inline bool PositionEquals(const TVector3& rhs, float tolerance = 1e-03) const
		{
			return Math::FloatEqual((float)x, (float)rhs.x, tolerance) &&
				Math::FloatEqual((float)y, (float)rhs.y, tolerance) &&
				Math::FloatEqual((float)z, (float)rhs.z, tolerance);

		}

		/** Returns whether this vector is within a directional tolerance
			of another vector.
		@param rhs The vector to compare with
		@param tolerance The maximum angle by which the vectors may vary and
			still be considered equal
		@note Both vectors should be normalised.
		*/
		inline bool DirectionEquals(const TVector3& rhs,
			const float& tolerance) const
		{
			T dot = Dot(rhs);
			T angle = acos(dot);

			return Abs(angle) <= tolerance;

		}

		/// Check whether this vector contains valid values
	 	inline bool IsNaN() const
		{
			return _isnan(x) || _isnan(y) || _isnan(z);
		}
 
    };

	typedef TVector3<float> Vector3f;
	typedef TVector3<double> Vector3d;
	typedef TVector3<int>  Vector3i;
 
	template<typename T>
    class TVector4
    {
    public:
        T x, y, z, w;

    public:
        inline TVector4()
        {
        }

        inline TVector4( const T fX, const T fY, const T fZ, const T fW )
            : x( fX ), y( fY ), z( fZ ), w( fW)
        {
        }

		inline TVector4( const Eigen::Vector4f& vec)
			: x( vec.x() ), y( vec.y() ), z( vec.z() ), w( vec.w() )
        {
        }
 

        inline explicit TVector4( const T scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
            , w( scaler )
        {
        }

        inline explicit TVector4(const TVector3<T>& rhs)
            : x(rhs.x), y(rhs.y), z(rhs.z), w((T)1)
        {
        }
 
		inline T operator [] ( const size_t i ) const
        {
            return *(&x+i);
        }

		inline T& operator [] ( const size_t i )
		{
            return *(&x+i);
        }

		/// Pointer accessor for direct copying
		inline T* Ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const T* Ptr() const
		{
			return &x;
		}

 
        inline TVector4& operator = ( const TVector4& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;
            w = rkVector.w;

            return *this;
        }

		 inline TVector4& operator = ( const Eigen::Vector4f& rkVector )
        {
            x = rkVector.x();
            y = rkVector.y();
            z = rkVector.z();
            w = rkVector.w();

            return *this;
        }

		inline TVector4& operator = ( const T fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return *this;
		}

        inline bool operator == ( const TVector4& rkVector ) const
        {
            return ( x == rkVector.x &&
                y == rkVector.y &&
                z == rkVector.z &&
                w == rkVector.w );
        }

        inline bool operator != ( const TVector4& rkVector ) const
        {
            return ( x != rkVector.x ||
                y != rkVector.y ||
                z != rkVector.z ||
                w != rkVector.w );
        }

        inline TVector4& operator = (const TVector3<T>& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = (T)1;
            return *this;
        }

        // arithmetic operations
        inline TVector4 operator + ( const TVector4& rkVector ) const
        {
            return TVector4(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z,
                w + rkVector.w);
        }

        inline TVector4 operator - ( const TVector4& rkVector ) const
        {
            return TVector4(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z,
                w - rkVector.w);
        }

        inline TVector4 operator * ( const T fScalar ) const
        {
            return TVector4(
                x * fScalar,
                y * fScalar,
                z * fScalar,
                w * fScalar);
        }

        inline TVector4 operator * ( const TVector4& rhs) const
        {
            return TVector4(
                rhs.x * x,
                rhs.y * y,
                rhs.z * z,
                rhs.w * w);
        }

        inline TVector4 operator / ( const T fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            T fInv = 1.0f / fScalar;

            return TVector4(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline TVector4 operator / ( const TVector4& rhs) const
        {
            return TVector4(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w);
        }

        inline const TVector4& operator + () const
        {
            return *this;
        }

        inline TVector4 operator - () const
        {
            return TVector4(-x, -y, -z, -w);
        }

        inline friend TVector4 operator * ( const T fScalar, const TVector4& rkVector )
        {
            return TVector4(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z,
                fScalar * rkVector.w);
        }

        inline friend TVector4 operator / ( const T fScalar, const TVector4& rkVector )
        {
            return TVector4(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z,
                fScalar / rkVector.w);
        }

        inline friend TVector4 operator + (const TVector4& lhs, const T rhs)
        {
            return TVector4(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs,
                lhs.w + rhs);
        }

        inline friend TVector4 operator + (const T lhs, const TVector4& rhs)
        {
            return TVector4(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z,
                lhs + rhs.w);
        }

        inline friend TVector4 operator - (const TVector4& lhs, T rhs)
        {
            return TVector4(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs,
                lhs.w - rhs);
        }

        inline friend TVector4 operator - (const T lhs, const TVector4& rhs)
        {
            return TVector4(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z,
                lhs - rhs.w);
        }

        // arithmetic updates
        inline TVector4& operator += ( const TVector4& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;
            w += rkVector.w;

            return *this;
        }

        inline TVector4& operator -= ( const TVector4& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;
            w -= rkVector.w;

            return *this;
        }

        inline TVector4& operator *= ( const T fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            w *= fScalar;
            return *this;
        }

        inline TVector4& operator += ( const T fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            w += fScalar;
            return *this;
        }

        inline TVector4& operator -= ( const T fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            w -= fScalar;
            return *this;
        }

        inline TVector4& operator *= ( const TVector4& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;
            w *= rkVector.w;

            return *this;
        }

        inline TVector4& operator /= ( const T fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            T fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

        inline TVector4& operator /= ( const TVector4& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;
            w /= rkVector.w;

            return *this;
        }

         
        inline T Dot(const TVector4& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }
		/// Check whether this vector contains valid values
		inline bool IsNaN() const
		{
			return _isnan(x) || _isnan(y) || _isnan(z) || _isnan(w);
		}
        
    };

	typedef TVector4<float> Vector4f;
	typedef TVector4<double> Vector4d;
	typedef TVector4<int> Vector4i;
 }
#endif