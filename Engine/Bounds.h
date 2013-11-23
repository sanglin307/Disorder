#ifndef _DISORDER_BOUNDS_H_
#define _DISORDER_BOUNDS_H_

namespace Disorder
{
	struct BoxBounds
	{
		BoxBounds() 
		{
			BMin = Vector3( Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
			BMax = Vector3( Math::NEG_INFINITY, Math::NEG_INFINITY, Math::NEG_INFINITY);
		}

		BoxBounds(const Vector3 &p) : BMin(p), BMax(p) { }
		BoxBounds(const Vector3 &p1, const Vector3 &p2) 
		{
			BMin = Vector3(Min(p1.x, p2.x), Min(p1.y, p2.y), Min(p1.z, p2.z));
			BMax = Vector3(Max(p1.x, p2.x), Max(p1.y, p2.y), Max(p1.z, p2.z));
		}

		bool IsValid() const
		{
			return BMin < BMax;
		}

		Vector3 GetCenter() const
		{
			return Vector3( ( BMin + BMax ) * 0.5f );
		}
		// Returns the extent around the center
		Vector3 GetExtent() const
		{
			return 0.5f*(BMax - BMin);
		}

		void GetCenterAndExtents( Vector3 & center, Vector3 & Extents ) const
		{
			Extents = GetExtent();
			center = BMin + Extents;
		}

		inline BoxBounds Union(const Vector3 &p)
		{
			BoxBounds ret = *this;
			ret.BMin.x = Min(BMin.x, p.x);
			ret.BMin.y = Min(BMin.y, p.y);
			ret.BMin.z = Min(BMin.z, p.z);
			ret.BMax.x = Max(BMax.x, p.x);
			ret.BMax.y = Max(BMax.y, p.y);
			ret.BMax.z = Max(BMax.z, p.z);
			
			return ret;
		}

		inline BoxBounds Union(const BoxBounds &b)
		{			 
			BoxBounds ret;
			ret.BMin.x = Min(BMin.x, b.BMin.x);
			ret.BMin.y = Min(BMin.y, b.BMin.y);
			ret.BMin.z = Min(BMin.z, b.BMin.z);
			ret.BMax.x = Max(BMax.x, b.BMax.x);
			ret.BMax.y = Max(BMax.y, b.BMax.y);
			ret.BMax.z = Max(BMax.z, b.BMax.z);
			return ret;
		}

		inline  bool Overlaps(const BoxBounds &b) const 
		{
			bool x = (BMax.x >= b.BMin.x) && (BMin.x <= b.BMax.x);
			if ( !x )
				return false;

			bool y = (BMax.y >= b.BMin.y) && (BMin.y <= b.BMax.y);
			if( !y )
				return false;

			bool z = (BMax.z >= b.BMin.z) && (BMin.z <= b.BMax.z);
			if( !z )
				return false;

			return (x && y && z);
		}

		inline  bool Inside(const Vector3 &pt) const 
		{
			return (pt.x >= BMin.x && pt.x <= BMax.x &&
					pt.y >= BMin.y && pt.y <= BMax.y &&
					pt.z >= BMin.z && pt.z <= BMax.z);
		}

		inline  void Expand(float delta) 
		{
			BMin -= Vector3(delta, delta, delta);
			BMax += Vector3(delta, delta, delta);
		}

		inline  float SurfaceArea() const 
		{
			Vector3 d = BMax - BMin;
			return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
		}

		inline float Volume() const 
		{
			Vector3 d = BMax - BMin;
			return d.x * d.y * d.z;
		}

		inline int MaximumExtent() const 
		{
			Vector3 diag = BMax - BMin;
			if (diag.x > diag.y && diag.x > diag.z)
				return 0;
			else if (diag.y > diag.z)
				return 1;
			else
				return 2;
		}
		 
		inline Vector3 Lerp(float tx, float ty, float tz) const 
		{
			return Vector3(Math::Lerp(tx, BMin.x, BMax.x), Math::Lerp(ty, BMin.y, BMax.y),
						   Math::Lerp(tz, BMin.z, BMax.z));
		}

		inline Vector3 Offset(const Vector3 &p) const
		{
			return Vector3((p.x - BMin.x) / (BMax.x - BMin.x),
						  (p.y - BMin.y) / (BMax.y - BMin.y),
						  (p.z - BMin.z) / (BMax.z - BMin.z));
		}

		inline bool operator==(const BoxBounds &b) const 
		{
			return b.BMin == BMin && b.BMax == BMax;
		}

		inline bool operator!=(const BoxBounds &b) const 
		{
			return b.BMin != BMin || b.BMax != BMax;
		}

		Vector3 BMin, BMax;
	};

	struct SphereBounds
	{
		Vector3 Origin;
		float Radius;

		SphereBounds() 
		{
			Origin = Vector3( 0.f, 0.f, 0.f);
			Radius = Math::POS_INFINITY;
		}

		SphereBounds(const Vector3 &p) : Origin(p), Radius(Math::POS_INFINITY) { }
		SphereBounds(const Vector3 &origin, const float radius):Origin(origin),Radius(radius)
		{
		}

		inline SphereBounds Union(const Vector3 &p)
		{
			SphereBounds ret = *this;
			float radius = Origin.Distance(p);
			if( ret.Radius < radius )
				ret.Radius = radius;

			return ret;
		}

		inline  bool Overlaps(const SphereBounds &b) const 
		{
			float distance = Origin.Distance(b.Origin);

			return distance < Radius + b.Radius;
		}

		inline  bool Inside(const Vector3 &pt) const 
		{
			float distance = Origin.Distance(pt);
			return distance < Radius;
		}

		inline  void Expand(float delta) 
		{
			Radius += delta;
		}

		inline  float SurfaceArea() const 
		{
			return Math::PI * Radius * Radius;
		}

		inline float Volume() const 
		{
			return Math::PI * Radius * Radius * Radius;
		}
 
		inline float Offset(const Vector3 &p) const
		{
			float distance = Origin.Distance(p);
			return distance / Radius;
		}
	 
		inline bool operator==(const SphereBounds &b) const 
		{
			return b.Origin == Origin && Math::FloatEqual(b.Radius,Radius);
		}

		inline bool operator!=(const SphereBounds &b) const 
		{
			return !Math::FloatEqual(b.Radius,Radius) || b.Origin != Origin;
		}

	};

	struct BoxSphereBounds
	{
		Vector3	Origin,
				BoxExtent;
		float   SphereRadius;

		// Constructor.

		BoxSphereBounds() {}

		BoxSphereBounds(const Vector3& InOrigin,const Vector3& InBoxExtent,float InSphereRadius):
			Origin(InOrigin),
			BoxExtent(InBoxExtent),
			SphereRadius(InSphereRadius)
		{}

		BoxSphereBounds(const SphereBounds& Sphere)
		{
			Origin = Sphere.Origin;
			SphereRadius = Sphere.Radius;
			float v1 = 1.0f / Math::Sqrtf(3);
			float v2 = Math::Sqrtf(2) * v1;
			BoxExtent = Vector3(SphereRadius * v1,SphereRadius * v2,SphereRadius * v1);
		}

		BoxSphereBounds(const BoxBounds& Box)
		{
			Box.GetCenterAndExtents(Origin,BoxExtent);
			SphereRadius = BoxExtent.Length();
		}

		BoxSphereBounds(const Vector3* Points,unsigned int NumPoints)
		{
			// Find an axis aligned bounding box for the points.
			BoxBounds BoundingBox;
			for(unsigned int PointIndex = 0;PointIndex < NumPoints;PointIndex++)
				BoundingBox = BoundingBox.Union(Points[PointIndex]);
			BoundingBox.GetCenterAndExtents(Origin,BoxExtent);

			// Using the center of the bounding box as the origin of the sphere, find the radius of the bounding sphere.
			SphereRadius = 0.0f;
			for(unsigned int PointIndex = 0;PointIndex < NumPoints;PointIndex++)
				SphereRadius = Max(SphereRadius,Points[PointIndex].Distance(Origin));
		}

		// GetBox

		BoxBounds GetBox() const
		{
			return BoxBounds(Origin - BoxExtent,Origin + BoxExtent);
		}

		// GetSphere
		SphereBounds GetSphere() const
		{
			return SphereBounds(Origin,SphereRadius);
		}

		BoxSphereBounds Union(const BoxSphereBounds& B) const
		{
			Vector3 AMin = Origin - BoxExtent;
			Vector3 AMax = Origin + BoxExtent;
			Vector3 BMin = B.Origin - B.BoxExtent;
			Vector3 BMax = B.Origin + B.BoxExtent;

			BoxBounds box;
			box.BMin.x = Min(AMin.x, BMin.x);
			box.BMin.y = Min(AMin.y, BMin.y);
			box.BMin.z = Min(AMin.z, BMin.z);
			box.BMax.x = Max(AMax.x, BMax.x);
			box.BMax.y = Max(AMax.y, BMax.y);
			box.BMax.z = Max(AMax.z, BMax.z);

			BoxSphereBounds Result;
			box.GetCenterAndExtents(Result.Origin,Result.BoxExtent);
			Result.SphereRadius = Result.BoxExtent.Length();
 
			return Result;
		}
		/**
		 * Constructs a bounding volume containing both A and B.
		 */
		BoxSphereBounds operator+(const BoxSphereBounds& B) const
		{
			return Union(B);
		}
     };
}

#endif