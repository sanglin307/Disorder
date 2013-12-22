#ifndef _DISORDER_VECTOR_H_
#define _DISORDER_VECTOR_H_
 
 

namespace Disorder
{
    class  Vector2
    {
    public:
        float x, y;

    public:
        inline Vector2()
        {
        }

		inline Vector2(const double fX, const double fY )
		{
			x = (float)fX;
			y = (float)fY;
		}

        inline Vector2(const float fX, const float fY )
            : x( fX ), y( fY )
        {
        }

        inline explicit Vector2( const float scaler )
            : x( scaler), y( scaler )
        {
        }

        inline explicit Vector2( const float afCoordinate[2] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] )
        {
        }

        inline explicit Vector2( const int afCoordinate[2] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
        }

        inline explicit Vector2( float* const r )
            : x( r[0] ), y( r[1] )
        {
        }

		/** Exchange the contents of this vector with another. 
		*/
		inline void Swap(Vector2& other)
		{
			Disorder::Swap<float>(x, other.x);
			Disorder::Swap<float>(y, other.y);
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
        inline Vector2& operator = ( const Vector2& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }

		inline Vector2& operator = ( const float fScalar)
		{
			x = fScalar;
			y = fScalar;

			return *this;
		}

        inline bool operator == ( const Vector2& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y );
        }

        inline bool operator != ( const Vector2& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y  );
        }

        // arithmetic operations
        inline Vector2 operator + ( const Vector2& rkVector ) const
        {
            return Vector2(
                x + rkVector.x,
                y + rkVector.y);
        }

        inline Vector2 operator - ( const Vector2& rkVector ) const
        {
            return Vector2(
                x - rkVector.x,
                y - rkVector.y);
        }

        inline Vector2 operator * ( const float fScalar ) const
        {
            return Vector2(
                x * fScalar,
                y * fScalar);
        }

        inline Vector2 operator * ( const Vector2& rhs) const
        {
            return Vector2(
                x * rhs.x,
                y * rhs.y);
        }

        inline Vector2 operator / ( const float fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector2(
                x * fInv,
                y * fInv);
        }

        inline Vector2 operator / ( const Vector2& rhs) const
        {
            return Vector2(
                x / rhs.x,
                y / rhs.y);
        }

        inline const Vector2& operator + () const
        {
            return *this;
        }

        inline Vector2 operator - () const
        {
            return Vector2(-x, -y);
        }

        // overloaded operators to help Vector2
        inline friend Vector2 operator * ( const float fScalar, const Vector2& rkVector )
        {
            return Vector2(
                fScalar * rkVector.x,
                fScalar * rkVector.y);
        }

        inline friend Vector2 operator / ( const float fScalar, const Vector2& rkVector )
        {
            return Vector2(
                fScalar / rkVector.x,
                fScalar / rkVector.y);
        }

        inline friend Vector2 operator + (const Vector2& lhs, const float rhs)
        {
            return Vector2(
                lhs.x + rhs,
                lhs.y + rhs);
        }

        inline friend Vector2 operator + (const float lhs, const Vector2& rhs)
        {
            return Vector2(
                lhs + rhs.x,
                lhs + rhs.y);
        }

        inline friend Vector2 operator - (const Vector2& lhs, const float rhs)
        {
            return Vector2(
                lhs.x - rhs,
                lhs.y - rhs);
        }

        inline friend Vector2 operator - (const float lhs, const Vector2& rhs)
        {
            return Vector2(
                lhs - rhs.x,
                lhs - rhs.y);
        }

        // arithmetic updates
        inline Vector2& operator += ( const Vector2& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }

        inline Vector2& operator += ( const float fScaler )
        {
            x += fScaler;
            y += fScaler;

            return *this;
        }

        inline Vector2& operator -= ( const Vector2& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline Vector2& operator -= ( const float fScaler )
        {
            x -= fScaler;
            y -= fScaler;

            return *this;
        }

        inline Vector2& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;

            return *this;
        }

        inline Vector2& operator *= ( const Vector2& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline Vector2& operator /= ( const float fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline Vector2& operator /= ( const Vector2& rkVector )
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

      
        inline float Distance(const Vector2& rhs) const
        {
            return (*this - rhs).Length();
        }

       
        inline float SquaredDistance(const Vector2& rhs) const
        {
			return (*this - rhs).SquaredLength();
        }

       
        inline float Dot(const Vector2& vec) const
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

 
        inline Vector2 MidPoint( const Vector2& vec ) const
        {
            return Vector2(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f );
        }

        inline bool operator < ( const Vector2& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
                return true;
            return false;
        }

        inline bool operator > ( const Vector2& rhs ) const
        {
            if( x > rhs.x && y > rhs.y )
                return true;
            return false;
        }
 
        inline Vector2 Perpendicular(void) const
        {
            return Vector2 (-y, x);
        }
 
		// equal to the area of parallelogram
        inline float Cross( const Vector2& rkVector ) const
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
        inline Vector2 RandomDeviant(float angle) const
        {

            angle *=  Math::UnitRandom() * Math::TWO_PI;
            float cosa = cos(angle);
            float sina = sin(angle);
            return  Vector2(cosa * x - sina * y,
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
        inline Vector2 Reflect(const Vector2& normal) const
        {
			return Vector2( *this - ( 2 * this->Dot(normal) * normal ) );
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
		inline float AngleBetween(const Vector2& other) const
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
		inline float AngleTo(const Vector2& other) const
		{
			float angle = AngleBetween(other);
		
			if (Cross(other)<0)			
				angle = (float)Math::TWO_PI - angle;		

			return angle;
		}

        // special points
        static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 NEGATIVE_UNIT_X;
        static const Vector2 NEGATIVE_UNIT_Y;
        static const Vector2 UNIT_SCALE;
 
    };

    class Vector3
    {
    public:
		float x, y, z;

    public:
        inline Vector3()
        {
        }

		 
		inline Vector3( const double dX, const double dY, const double dZ )
		{
			x = (float)dX;
			y = (float)dY;
			z = (float)dZ;
		}
 
        inline Vector3( const float fX, const float fY, const float fZ )
            : x( fX ), y( fY ), z( fZ )
        {
        }

        inline explicit Vector3( const float afCoordinate[3] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] )
        {
        }

        inline explicit Vector3( const int afCoordinate[3] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
        }

        inline explicit Vector3( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

        inline explicit Vector3( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }


		/** Exchange the contents of this vector with another. 
		*/
		inline void Swap(Vector3& other)
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
        inline Vector3& operator = ( const Vector3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

        inline Vector3& operator = ( const float fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const Vector3& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const Vector3& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        // arithmetic operations
        inline Vector3 operator + ( const Vector3& rkVector ) const
        {
            return Vector3(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline Vector3 operator - ( const Vector3& rkVector ) const
        {
            return Vector3(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline Vector3 operator * ( const float fScalar ) const
        {
            return Vector3(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline Vector3 operator * ( const Vector3& rhs) const
        {
            return Vector3(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline Vector3 operator / ( const float fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline Vector3 operator / ( const Vector3& rhs) const
        {
            return Vector3(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const Vector3& operator + () const
        {
            return *this;
        }

        inline Vector3 operator - () const
        {
            return Vector3(-x, -y, -z);
        }

        // overloaded operators to help Vector3
        inline friend Vector3 operator * ( const float fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend Vector3 operator / ( const float fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend Vector3 operator + (const Vector3& lhs, const float rhs)
        {
            return Vector3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend Vector3 operator + (const float lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend Vector3 operator - (const Vector3& lhs, const float rhs)
        {
            return Vector3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend Vector3 operator - (const float lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline Vector3& operator += ( const Vector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline Vector3& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline Vector3& operator -= ( const Vector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline Vector3& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const Vector3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline Vector3& operator /= ( const float fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline Vector3& operator /= ( const Vector3& rkVector )
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

        
        inline float Distance(const Vector3& rhs) const
        {
            return (*this - rhs).Length();
        }

        
        inline float SquaredDistance(const Vector3& rhs) const
        {
            return (*this - rhs).SquaredLength();
        }

        
        inline float Dot(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
 
        inline float AbsDot(const Vector3& vec) const
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
 
        inline Vector3 Cross( const Vector3& rkVector ) const
        {
            return Vector3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        
        inline Vector3 MidPoint( const Vector3& vec ) const
        {
            return Vector3(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

      
        inline bool operator < ( const Vector3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        
        inline bool operator > ( const Vector3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }
 
        
        inline Vector3 Perpendicular(void) const
        {
            static const float fSquareZero = (float)(1e-06 * 1e-06);

			Vector3 perp = this->Cross( Vector3::UNIT_X );

            // Check length
            if( perp.SquaredLength() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a scalar, so we have
                   to use another axis.
                */
				perp = this->Cross( Vector3::UNIT_Y );
            }
			perp.Normalise();

            return perp;
        }
       
		/** Gets the angle between 2 vectors.
		*/
		inline float AngleBetween(const Vector3& dest) const
		{
			float lenProduct = Length() * dest.Length();

			// Divide by zero check
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;

		    float f = Dot(dest) / lenProduct;

			f = Clamp<float>(f, (float)-1.0, (float)1.0);
			return Math::ACosf(f);

		}
        /** Gets the shortest arc quaternion to rotate this vector to the destination
            vector.
        @remarks
            If you call this with a dest vector that is close to the inverse
            of this vector, we will rotate 180 degrees around the 'fallbackAxis'
			(if specified, or a generated axis if not) since in this case
			ANY axis of rotation is valid.
        */
        Quaternion GetRotationTo(const Vector3& dest,
			const Vector3& fallbackAxis = Vector3::ZERO) const
        {
            // Based on Stan Melax's article in Game Programming Gems
            Quaternion q;
            // Copy, since cannot modify local
            Vector3 v0 = *this;
            Vector3 v1 = dest;
            v0.Normalise();
            v1.Normalise();

			float d = v0.Dot(v1);
            // If dot == 1, vectors are the same
            if (d >= 1.0f)
            {
                return Quaternion::IDENTITY;
            }
			if (d < (1e-6f - 1.0f))
			{
				if (fallbackAxis != Vector3::ZERO)
				{
					// rotate 180 degrees about the fallback axis
					q.FromAngleAxis(float(Math::PI), fallbackAxis);
				}
				else
				{
					// Generate an axis
					Vector3 axis = Vector3::UNIT_X.Cross(*this);
					if (axis.IsZeroLength()) // pick another if colinear
						axis = Vector3::UNIT_Y.Cross(*this);
					axis.Normalise();
					q.FromAngleAxis(float(Math::PI), axis);
				}
			}
			else
			{
                float s = Math::Sqrtf( (1+d)*2 );
	            float invs = 1 / s;

				Vector3 c = v0.Cross(v1);

    	        q.x = c.x * invs;
        	    q.y = c.y * invs;
            	q.z = c.z * invs;
            	q.w = s * 0.5f;
				q.Normalise();
			}
            return q;
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
        inline Vector3 Reflect(const Vector3& normal) const
        {
            return Vector3( *this - ( 2 * this->Dot(normal) * normal ) );
        }

		/** Returns whether this vector is within a positional tolerance
			of another vector.
		@param rhs The vector to compare with
		@param tolerance The amount that each element of the vector may vary by
			and still be considered equal
		*/
		inline bool PositionEquals(const Vector3& rhs, float tolerance = 1e-03) const
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
		inline bool DirectionEquals(const Vector3& rhs,
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
		inline Vector3 PrimaryAxis() const
		{
			float absx = Abs<float>(x);
			float absy = Abs<float>(y);
			float absz = Abs<float>(z);
			if (absx > absy)
				if (absx > absz)
					return x > 0 ? Vector3::UNIT_X : Vector3::NEGATIVE_UNIT_X;
				else
					return z > 0 ? Vector3::UNIT_Z : Vector3::NEGATIVE_UNIT_Z;
			else // absx <= absy
				if (absy > absz)
					return y > 0 ? Vector3::UNIT_Y : Vector3::NEGATIVE_UNIT_Y;
				else
					return z > 0 ? Vector3::UNIT_Z : Vector3::NEGATIVE_UNIT_Z;
		}

		// special points
        static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
    };
 
    class  Vector4
    {
    public:
        float x, y, z, w;

    public:
        inline Vector4()
        {
        }

        inline Vector4( const float fX, const float fY, const float fZ, const float fW )
            : x( fX ), y( fY ), z( fZ ), w( fW)
        {
        }

		inline Vector4( const double dX, const double dY, const double dZ, const double dW )
		{
			x = (float)dX;
			y = (float)dY;
			z = (float)dZ;
			w = (float)dW;
		}

        inline explicit Vector4( const float afCoordinate[4] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] ),
              w( afCoordinate[3] )
        {
        }

        inline explicit Vector4( const int afCoordinate[4] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
            w = (float)afCoordinate[3];
        }

        inline explicit Vector4( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
        {
        }

        inline explicit Vector4( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
            , w( scaler )
        {
        }

        inline explicit Vector4(const Vector3& rhs)
            : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
        {
        }

		/** Exchange the contents of this vector with another. 
		*/
		inline void Swap(Vector4& other)
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

 
        inline Vector4& operator = ( const Vector4& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;
            w = rkVector.w;

            return *this;
        }

		inline Vector4& operator = ( const float fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return *this;
		}

        inline bool operator == ( const Vector4& rkVector ) const
        {
            return ( x == rkVector.x &&
                y == rkVector.y &&
                z == rkVector.z &&
                w == rkVector.w );
        }

        inline bool operator != ( const Vector4& rkVector ) const
        {
            return ( x != rkVector.x ||
                y != rkVector.y ||
                z != rkVector.z ||
                w != rkVector.w );
        }

        inline Vector4& operator = (const Vector3& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = 1.0f;
            return *this;
        }

        // arithmetic operations
        inline Vector4 operator + ( const Vector4& rkVector ) const
        {
            return Vector4(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z,
                w + rkVector.w);
        }

        inline Vector4 operator - ( const Vector4& rkVector ) const
        {
            return Vector4(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z,
                w - rkVector.w);
        }

        inline Vector4 operator * ( const float fScalar ) const
        {
            return Vector4(
                x * fScalar,
                y * fScalar,
                z * fScalar,
                w * fScalar);
        }

        inline Vector4 operator * ( const Vector4& rhs) const
        {
            return Vector4(
                rhs.x * x,
                rhs.y * y,
                rhs.z * z,
                rhs.w * w);
        }

        inline Vector4 operator / ( const float fScalar ) const
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector4(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline Vector4 operator / ( const Vector4& rhs) const
        {
            return Vector4(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w);
        }

        inline const Vector4& operator + () const
        {
            return *this;
        }

        inline Vector4 operator - () const
        {
            return Vector4(-x, -y, -z, -w);
        }

        inline friend Vector4 operator * ( const float fScalar, const Vector4& rkVector )
        {
            return Vector4(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z,
                fScalar * rkVector.w);
        }

        inline friend Vector4 operator / ( const float fScalar, const Vector4& rkVector )
        {
            return Vector4(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z,
                fScalar / rkVector.w);
        }

        inline friend Vector4 operator + (const Vector4& lhs, const float rhs)
        {
            return Vector4(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs,
                lhs.w + rhs);
        }

        inline friend Vector4 operator + (const float lhs, const Vector4& rhs)
        {
            return Vector4(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z,
                lhs + rhs.w);
        }

        inline friend Vector4 operator - (const Vector4& lhs, float rhs)
        {
            return Vector4(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs,
                lhs.w - rhs);
        }

        inline friend Vector4 operator - (const float lhs, const Vector4& rhs)
        {
            return Vector4(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z,
                lhs - rhs.w);
        }

        // arithmetic updates
        inline Vector4& operator += ( const Vector4& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;
            w += rkVector.w;

            return *this;
        }

        inline Vector4& operator -= ( const Vector4& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;
            w -= rkVector.w;

            return *this;
        }

        inline Vector4& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            w *= fScalar;
            return *this;
        }

        inline Vector4& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            w += fScalar;
            return *this;
        }

        inline Vector4& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            w -= fScalar;
            return *this;
        }

        inline Vector4& operator *= ( const Vector4& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;
            w *= rkVector.w;

            return *this;
        }

        inline Vector4& operator /= ( const float fScalar )
        {
            BOOST_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

        inline Vector4& operator /= ( const Vector4& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;
            w /= rkVector.w;

            return *this;
        }

         
        inline float Dot(const Vector4& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }
		/// Check whether this vector contains valid values
		inline bool IsNaN() const
		{
			return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z) || Math::IsNaN(w);
		}
        
        // special
        static const Vector4 ZERO;
		static const Vector4 ONE;
    };
 }
#endif