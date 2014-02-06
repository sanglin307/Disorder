#ifndef _DISORDER_VECTOR_H_
#define _DISORDER_VECTOR_H_

namespace Disorder
{
	////////////////////////////////////////////////////////////////////////////////////
	//// these vector used to mesh inside vertexs, they are lite ///////////////////////
	//// we don't use huge Eigen related vector inside mesh     /////////////////////// 
	///////////////////////////////////////////////////////////////////////////////////

    class  Vector2f
    {
    public:
        float x, y;

    public:
        inline Vector2f()
        {
        }

		inline Vector2f(const Eigen::Vector2f& vec)
		{
			x = vec.x();
			y = vec.y();
		}

		inline Vector2f(const double fX, const double fY )
		{
			x = (float)fX;
			y = (float)fY;
		}

        inline Vector2f(const float fX, const float fY )
            : x( fX ), y( fY )
        {
        }

        inline explicit Vector2f( const float scaler )
            : x( scaler), y( scaler )
        {
        }

        inline explicit Vector2f( const float afCoordinate[2] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] )
        {
        }

        inline explicit Vector2f( const int afCoordinate[2] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
        }

        inline explicit Vector2f( float* const r )
            : x( r[0] ), y( r[1] )
        {
        }
 
		inline float operator [] ( const size_t i ) const
        {
            BOOST_ASSERT( i < 2 );
            return *(&x+i);
        }

		inline float& operator [] ( const size_t i )
        {
            BOOST_ASSERT( i < 2 );
            return *(&x+i);
        }

		/// Pointer accessor for direct copying
		inline float* Ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const float* Ptr() const
		{
			return &x;
		}

        /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline Vector2f& operator = ( const Vector2f& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }

		inline Vector2f& operator = ( const Eigen::Vector2f& rkVector )
        {
            x = rkVector.x();
            y = rkVector.y();

            return *this;
        }

		inline Vector2f& operator = ( const float fScalar)
		{
			x = fScalar;
			y = fScalar;

			return *this;
		}

        inline bool operator == ( const Vector2f& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y );
        }

        inline bool operator != ( const Vector2f& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y  );
        }

        // arithmetic operations
        inline Vector2f operator + ( const Vector2f& rkVector ) const
        {
            return Vector2f(
                x + rkVector.x,
                y + rkVector.y);
        }

        inline Vector2f operator - ( const Vector2f& rkVector ) const
        {
            return Vector2f(
                x - rkVector.x,
                y - rkVector.y);
        }

        inline Vector2f operator * ( const float fScalar ) const
        {
            return Vector2f(
                x * fScalar,
                y * fScalar);
        }

        inline Vector2f operator * ( const Vector2f& rhs) const
        {
            return Vector2f(
                x * rhs.x,
                y * rhs.y);
        }

        inline Vector2f operator / ( const float fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector2f(
                x * fInv,
                y * fInv);
        }

        inline Vector2f operator / ( const Vector2f& rhs) const
        {
            return Vector2f(
                x / rhs.x,
                y / rhs.y);
        }

        inline const Vector2f& operator + () const
        {
            return *this;
        }

        inline Vector2f operator - () const
        {
            return Vector2f(-x, -y);
        }

        // overloaded operators to help Vector2f
        inline friend Vector2f operator * ( const float fScalar, const Vector2f& rkVector )
        {
            return Vector2f(
                fScalar * rkVector.x,
                fScalar * rkVector.y);
        }

        inline friend Vector2f operator / ( const float fScalar, const Vector2f& rkVector )
        {
            return Vector2f(
                fScalar / rkVector.x,
                fScalar / rkVector.y);
        }

        inline friend Vector2f operator + (const Vector2f& lhs, const float rhs)
        {
            return Vector2f(
                lhs.x + rhs,
                lhs.y + rhs);
        }

        inline friend Vector2f operator + (const float lhs, const Vector2f& rhs)
        {
            return Vector2f(
                lhs + rhs.x,
                lhs + rhs.y);
        }

        inline friend Vector2f operator - (const Vector2f& lhs, const float rhs)
        {
            return Vector2f(
                lhs.x - rhs,
                lhs.y - rhs);
        }

        inline friend Vector2f operator - (const float lhs, const Vector2f& rhs)
        {
            return Vector2f(
                lhs - rhs.x,
                lhs - rhs.y);
        }

        // arithmetic updates
        inline Vector2f& operator += ( const Vector2f& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }

        inline Vector2f& operator += ( const float fScaler )
        {
            x += fScaler;
            y += fScaler;

            return *this;
        }

        inline Vector2f& operator -= ( const Vector2f& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline Vector2f& operator -= ( const float fScaler )
        {
            x -= fScaler;
            y -= fScaler;

            return *this;
        }

        inline Vector2f& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;

            return *this;
        }

        inline Vector2f& operator *= ( const Vector2f& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline Vector2f& operator /= ( const float fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline Vector2f& operator /= ( const Vector2f& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;

            return *this;
        }
 
        inline float Length () const
        {
            return Math::Sqrtf( x * x + y * y );
        }
 
        inline float SquaredLength () const
        {
            return x * x + y * y;
        }

      
        inline float Distance(const Vector2f& rhs) const
        {
            return (*this - rhs).Length();
        }

       
        inline float SquaredDistance(const Vector2f& rhs) const
        {
			return (*this - rhs).SquaredLength();
        }

       
        inline float Dot(const Vector2f& vec) const
        {
            return x * vec.x + y * vec.y;
        }
 
        inline float Normalise()
        {
            float fLength = Math::Sqrtf( x * x + y * y);
 
			if ( fLength > float(0.0f) && fLength != 1.0f )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
            }

            return fLength;
        }

 
        inline Vector2f MidPoint( const Vector2f& vec ) const
        {
            return Vector2f(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f );
        }

        inline bool operator < ( const Vector2f& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
                return true;
            return false;
        }

        inline bool operator > ( const Vector2f& rhs ) const
        {
            if( x > rhs.x && y > rhs.y )
                return true;
            return false;
        }
 
        inline Vector2f Perpendicular(void) const
        {
            return Vector2f (-y, x);
        }
 
		// equal to the area of parallelogram
        inline float Cross( const Vector2f& rkVector ) const
        {
            return x * rkVector.y - y * rkVector.x;
        }

        /** Generates a new random vector which deviates from this vector by a
            given angle in a random direction.
            @remarks
                This method assumes that the random number generator has already
                been seeded appropriately.
            @param
                angle The angle at which to deviate in radians, the angle is the max one
            @param
                up Any vector perpendicular to this one (which could generated
                by cross-product of this vector and any other non-colinear
                vector). If you choose not to provide this the function will
                derive one on it's own, however if you provide one yourself the
                function will be faster (this allows you to reuse up vectors if
                you call this method more than once)
            @return
                A random vector which deviates from this vector by angle. This
                vector will not be normalised, normalise it if you wish
                afterwards.
        */
        inline Vector2f RandomDeviant(float angle) const
        {

            angle *=  Math::UnitRandom() * Math::TWO_PI;
            float cosa = cos(angle);
            float sina = sin(angle);
            return  Vector2f(cosa * x - sina * y,
                            sina * x + cosa * y);
        }

        /** Returns true if this vector is zero length. */
        inline bool IsZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (1e-06 * 1e-06));

        }
 
        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline Vector2f Reflect(const Vector2f& normal) const
        {
			return Vector2f( *this - ( 2 * this->Dot(normal) * normal ) );
        }

	    /// Check whether this vector contains valid values
		inline bool IsNaN() const
		{
			return Math::IsNaN(x) || Math::IsNaN(y);
		}

		/**	 Gets the angle between 2 vectors.
		@remarks
			Vectors do not have to be unit-length but must represent directions.
		*/
		inline float AngleBetween(const Vector2f& other) const
		{		
			float lenProduct = Length() * other.Length();
			// Divide by zero check
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;
		
			float f = Dot(other) / lenProduct;
	
			f = Clamp<float>(f, -1.0f, 1.0f);
			return Math::ACosf(f);
		}

		/**	 Gets the oriented angle between 2 vectors.
		@remarks
			Vectors do not have to be unit-length but must represent directions.
			The angle is comprised between 0 and 2 PI.
		*/
		inline float AngleTo(const Vector2f& other) const
		{
			float angle = AngleBetween(other);
		
			if (Cross(other)<0)			
				angle = (float)Math::TWO_PI - angle;		

			return angle;
		}

		static const Vector2f ZERO;
        static const Vector2f UNIT_X;
        static const Vector2f UNIT_Y;
        static const Vector2f NEGATIVE_UNIT_X;
        static const Vector2f NEGATIVE_UNIT_Y;
        static const Vector2f UNIT_SCALE;

    };

    class Vector3f
    {
    public:
		float x, y, z;

    public:
        inline Vector3f()
        {
        }

		 
		inline Vector3f( const double dX, const double dY, const double dZ )
		{
			x = (float)dX;
			y = (float)dY;
			z = (float)dZ;
		}
 
		 inline Vector3f( const Eigen::Vector3f& vec )
			 : x( vec.x() ), y( vec.y() ), z( vec.z() )
        {
        }

        inline Vector3f( const float fX, const float fY, const float fZ )
            : x( fX ), y( fY ), z( fZ )
        {
        }

        inline explicit Vector3f( const float afCoordinate[3] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] )
        {
        }

        inline explicit Vector3f( const int afCoordinate[3] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
        }

        inline explicit Vector3f( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

        inline explicit Vector3f( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }


		/** Exchange the contents of this vector with another. 
		*/
		inline void Swap(Vector3f& other)
		{
			Disorder::Swap<float>(x, other.x);
			Disorder::Swap<float>(y, other.y);
			Disorder::Swap<float>(z, other.z);
		}

		inline float operator [] ( const size_t i ) const
        {
            BOOST_ASSERT( i < 3 );
            return *(&x+i);
        }

		inline float& operator [] ( const size_t i )
        {
            BOOST_ASSERT( i < 3 );
            return *(&x+i);
        }
		/// Pointer accessor for direct copying
		inline float* Ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const float* Ptr() const
		{
			return &x;
		}

        /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline Vector3f& operator = ( const Vector3f& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

		 inline Vector3f& operator = ( const Eigen::Vector3f& rkVector )
        {
            x = rkVector.x();
            y = rkVector.y();
            z = rkVector.z();

            return *this;
        }

        inline Vector3f& operator = ( const float fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const Vector3f& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const Vector3f& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        // arithmetic operations
        inline Vector3f operator + ( const Vector3f& rkVector ) const
        {
            return Vector3f(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline Vector3f operator - ( const Vector3f& rkVector ) const
        {
            return Vector3f(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline Vector3f operator * ( const float fScalar ) const
        {
            return Vector3f(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline Vector3f operator * ( const Vector3f& rhs) const
        {
            return Vector3f(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline Vector3f operator / ( const float fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector3f(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline Vector3f operator / ( const Vector3f& rhs) const
        {
            return Vector3f(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const Vector3f& operator + () const
        {
            return *this;
        }

        inline Vector3f operator - () const
        {
            return Vector3f(-x, -y, -z);
        }

        // overloaded operators to help Vector3f
        inline friend Vector3f operator * ( const float fScalar, const Vector3f& rkVector )
        {
            return Vector3f(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend Vector3f operator / ( const float fScalar, const Vector3f& rkVector )
        {
            return Vector3f(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend Vector3f operator + (const Vector3f& lhs, const float rhs)
        {
            return Vector3f(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend Vector3f operator + (const float lhs, const Vector3f& rhs)
        {
            return Vector3f(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend Vector3f operator - (const Vector3f& lhs, const float rhs)
        {
            return Vector3f(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend Vector3f operator - (const float lhs, const Vector3f& rhs)
        {
            return Vector3f(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline Vector3f& operator += ( const Vector3f& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline Vector3f& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline Vector3f& operator -= ( const Vector3f& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline Vector3f& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline Vector3f& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline Vector3f& operator *= ( const Vector3f& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline Vector3f& operator /= ( const float fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline Vector3f& operator /= ( const Vector3f& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

            return *this;
        }
 
        inline float Length () const
        {
			return Math::Sqrtf( x * x + y * y + z * z );
        }

        
        inline float SquaredLength () const
        {
            return x * x + y * y + z * z;
        }

        
        inline float Distance(const Vector3f& rhs) const
        {
            return (*this - rhs).Length();
        }

        
        inline float SquaredDistance(const Vector3f& rhs) const
        {
            return (*this - rhs).SquaredLength();
        }

        
        inline float Dot(const Vector3f& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
 
        inline float AbsDot(const Vector3f& vec) const
        {
            return Abs<float>(x * vec.x) + Abs<float>(y * vec.y) + Abs<float>(z * vec.z);
        }

       
        inline float Normalise()
        {
			float fLength = Math::Sqrtf( x * x + y * y + z * z );

			if ( fLength > float(0.0f) && fLength != 1.0f )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }
 
        inline Vector3f Cross( const Vector3f& rkVector ) const
        {
            return Vector3f(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        
        inline Vector3f MidPoint( const Vector3f& vec ) const
        {
            return Vector3f(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

      
        inline bool operator < ( const Vector3f& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        
        inline bool operator > ( const Vector3f& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }
 
        
        inline Vector3f Perpendicular(void) const
        {
            static const float fSquareZero = (float)(1e-06 * 1e-06);

			Vector3f perp = this->Cross( Vector3f::UNIT_X );

            // Check length
            if( perp.SquaredLength() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a scalar, so we have
                   to use another axis.
                */
				perp = this->Cross( Vector3f::UNIT_Y );
            }
			perp.Normalise();

            return perp;
        }
       
		/** Gets the angle between 2 vectors.
		*/
		inline float AngleBetween(const Vector3f& dest) const
		{
			float lenProduct = Length() * dest.Length();

			// Divide by zero check
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;

		    float f = Dot(dest) / lenProduct;

			f = Clamp<float>(f, (float)-1.0, (float)1.0);
			return Math::ACosf(f);

		}

        /** Returns true if this vector is zero length. */
        inline bool IsZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));

        }
 
        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline Vector3f Reflect(const Vector3f& normal) const
        {
            return Vector3f( *this - ( 2 * this->Dot(normal) * normal ) );
        }

		/** Returns whether this vector is within a positional tolerance
			of another vector.
		@param rhs The vector to compare with
		@param tolerance The amount that each element of the vector may vary by
			and still be considered equal
		*/
		inline bool PositionEquals(const Vector3f& rhs, float tolerance = 1e-03) const
		{
			return Math::FloatEqual(x, rhs.x, tolerance) &&
				Math::FloatEqual(y, rhs.y, tolerance) &&
				Math::FloatEqual(z, rhs.z, tolerance);

		}

		/** Returns whether this vector is within a directional tolerance
			of another vector.
		@param rhs The vector to compare with
		@param tolerance The maximum angle by which the vectors may vary and
			still be considered equal
		@note Both vectors should be normalised.
		*/
		inline bool DirectionEquals(const Vector3f& rhs,
			const float& tolerance) const
		{
			float dot = Dot(rhs);
			float angle = Math::ACosf(dot);

			return Abs<float>(angle) <= tolerance;

		}

		/// Check whether this vector contains valid values
	 	inline bool IsNaN() const
		{
			return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z);
		}

		/// Extract the primary (dominant) axis from this direction vector
		inline Vector3f PrimaryAxis() const
		{
			float absx = Abs<float>(x);
			float absy = Abs<float>(y);
			float absz = Abs<float>(z);
			if (absx > absy)
				if (absx > absz)
					return x > 0 ? Vector3f::UNIT_X : Vector3f::NEGATIVE_UNIT_X;
				else
					return z > 0 ? Vector3f::UNIT_Z : Vector3f::NEGATIVE_UNIT_Z;
			else // absx <= absy
				if (absy > absz)
					return y > 0 ? Vector3f::UNIT_Y : Vector3f::NEGATIVE_UNIT_Y;
				else
					return z > 0 ? Vector3f::UNIT_Z : Vector3f::NEGATIVE_UNIT_Z;
		}

		// special points
        static const Vector3f ZERO;
        static const Vector3f UNIT_X;
        static const Vector3f UNIT_Y;
        static const Vector3f UNIT_Z;
        static const Vector3f NEGATIVE_UNIT_X;
        static const Vector3f NEGATIVE_UNIT_Y;
        static const Vector3f NEGATIVE_UNIT_Z;
        static const Vector3f UNIT_SCALE;
    };
 
    class  Vector4f
    {
    public:
        float x, y, z, w;

    public:
        inline Vector4f()
        {
        }

        inline Vector4f( const float fX, const float fY, const float fZ, const float fW )
            : x( fX ), y( fY ), z( fZ ), w( fW)
        {
        }

		inline Vector4f( const Eigen::Vector4f& vec)
			: x( vec.x() ), y( vec.y() ), z( vec.z() ), w( vec.w() )
        {
        }

		inline Vector4f( const double dX, const double dY, const double dZ, const double dW )
		{
			x = (float)dX;
			y = (float)dY;
			z = (float)dZ;
			w = (float)dW;
		}

        inline explicit Vector4f( const float afCoordinate[4] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] ),
              w( afCoordinate[3] )
        {
        }

        inline explicit Vector4f( const int afCoordinate[4] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
            w = (float)afCoordinate[3];
        }

        inline explicit Vector4f( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
        {
        }

        inline explicit Vector4f( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
            , w( scaler )
        {
        }

        inline explicit Vector4f(const Vector3f& rhs)
            : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
        {
        }

		/** Exchange the contents of this vector with another. 
		*/
		inline void Swap(Vector4f& other)
		{
			Disorder::Swap<float>(x, other.x);
			Disorder::Swap<float>(y, other.y);
			Disorder::Swap<float>(z, other.z);
			Disorder::Swap<float>(w, other.w);
		}
	
		inline float operator [] ( const size_t i ) const
        {
            BOOST_ASSERT( i < 4 );

            return *(&x+i);
        }

		inline float& operator [] ( const size_t i )
        {
            BOOST_ASSERT( i < 4 );

            return *(&x+i);
        }

		/// Pointer accessor for direct copying
		inline float* Ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const float* Ptr() const
		{
			return &x;
		}

 
        inline Vector4f& operator = ( const Vector4f& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;
            w = rkVector.w;

            return *this;
        }

		 inline Vector4f& operator = ( const Eigen::Vector4f& rkVector )
        {
            x = rkVector.x();
            y = rkVector.y();
            z = rkVector.z();
            w = rkVector.w();

            return *this;
        }

		inline Vector4f& operator = ( const float fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return *this;
		}

        inline bool operator == ( const Vector4f& rkVector ) const
        {
            return ( x == rkVector.x &&
                y == rkVector.y &&
                z == rkVector.z &&
                w == rkVector.w );
        }

        inline bool operator != ( const Vector4f& rkVector ) const
        {
            return ( x != rkVector.x ||
                y != rkVector.y ||
                z != rkVector.z ||
                w != rkVector.w );
        }

        inline Vector4f& operator = (const Vector3f& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = 1.0f;
            return *this;
        }

        // arithmetic operations
        inline Vector4f operator + ( const Vector4f& rkVector ) const
        {
            return Vector4f(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z,
                w + rkVector.w);
        }

        inline Vector4f operator - ( const Vector4f& rkVector ) const
        {
            return Vector4f(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z,
                w - rkVector.w);
        }

        inline Vector4f operator * ( const float fScalar ) const
        {
            return Vector4f(
                x * fScalar,
                y * fScalar,
                z * fScalar,
                w * fScalar);
        }

        inline Vector4f operator * ( const Vector4f& rhs) const
        {
            return Vector4f(
                rhs.x * x,
                rhs.y * y,
                rhs.z * z,
                rhs.w * w);
        }

        inline Vector4f operator / ( const float fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector4f(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline Vector4f operator / ( const Vector4f& rhs) const
        {
            return Vector4f(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w);
        }

        inline const Vector4f& operator + () const
        {
            return *this;
        }

        inline Vector4f operator - () const
        {
            return Vector4f(-x, -y, -z, -w);
        }

        inline friend Vector4f operator * ( const float fScalar, const Vector4f& rkVector )
        {
            return Vector4f(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z,
                fScalar * rkVector.w);
        }

        inline friend Vector4f operator / ( const float fScalar, const Vector4f& rkVector )
        {
            return Vector4f(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z,
                fScalar / rkVector.w);
        }

        inline friend Vector4f operator + (const Vector4f& lhs, const float rhs)
        {
            return Vector4f(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs,
                lhs.w + rhs);
        }

        inline friend Vector4f operator + (const float lhs, const Vector4f& rhs)
        {
            return Vector4f(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z,
                lhs + rhs.w);
        }

        inline friend Vector4f operator - (const Vector4f& lhs, float rhs)
        {
            return Vector4f(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs,
                lhs.w - rhs);
        }

        inline friend Vector4f operator - (const float lhs, const Vector4f& rhs)
        {
            return Vector4f(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z,
                lhs - rhs.w);
        }

        // arithmetic updates
        inline Vector4f& operator += ( const Vector4f& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;
            w += rkVector.w;

            return *this;
        }

        inline Vector4f& operator -= ( const Vector4f& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;
            w -= rkVector.w;

            return *this;
        }

        inline Vector4f& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            w *= fScalar;
            return *this;
        }

        inline Vector4f& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            w += fScalar;
            return *this;
        }

        inline Vector4f& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            w -= fScalar;
            return *this;
        }

        inline Vector4f& operator *= ( const Vector4f& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;
            w *= rkVector.w;

            return *this;
        }

        inline Vector4f& operator /= ( const float fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

        inline Vector4f& operator /= ( const Vector4f& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;
            w /= rkVector.w;

            return *this;
        }

         
        inline float Dot(const Vector4f& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }
		/// Check whether this vector contains valid values
		inline bool IsNaN() const
		{
			return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z) || Math::IsNaN(w);
		}
        
        // special
        static const Vector4f ZERO;
		static const Vector4f ONE;
    };
 }
#endif