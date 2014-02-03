#ifndef _DISORDER_FRUSTRUM_H_
#define _DISORDER_FRUSTRUM_H_


namespace Disorder
{
	class Frustrum
	{
		enum PlaneSide
		{
			PS_Near,
			PS_Far,
			PS_Left,
			PS_Right,
			PS_Top,
			PS_Bottom
		};
 
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Frustrum();
		void Construct(const Eigen::Matrix4f& viewProjInvMatrix);
		bool Overlaps(BoxBounds & bounds) const;
		bool Overlaps(SphereBounds const& bounds) const;

		bool Inside(SphereBounds const& bounds) const;
		bool Inside(BoxBounds & bounds) const;
		void Draw();
	private:
		Eigen::Hyperplane<float,3> _Planes[6];
		Eigen::Vector3f _Points[8]; // clock-wise, near - far
	};

}



#endif