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
		Frustrum();
		void Construct(const glm::mat4& viewProjInvMatrix);
		bool Overlaps(BoxBounds & bounds) const;
		bool Overlaps(SphereBounds const& bounds) const;

		bool Inside(SphereBounds const& bounds) const;
		bool Inside(BoxBounds & bounds) const;
		void Draw();
	private:
		Plane _Planes[6];
		glm::vec3 _Points[8]; // clock-wise, near - far
	};

}



#endif