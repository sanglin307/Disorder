#include "EngineInclude.h"

namespace Disorder
{
	Frustrum::Frustrum()
	{	
		memset(_Points,0,8*sizeof(glm::vec3));
	}

	bool Frustrum::Overlaps(BoxBounds & bounds) const
	{
		if( !bounds.IsValid() )
		{
			BOOST_ASSERT(0);
			return false;
		}

		glm::vec3 center;
		glm::vec3 extend;
		bounds.GetCenterAndExtents(center,extend);
		
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(center, extend);
			if( side == Plane::NEGATIVE_SIDE )
				return false;
		}

		return true;
	}

	bool Frustrum::Overlaps(SphereBounds const& bounds) const
	{
		glm::vec3 eCenter(bounds.Origin.x, bounds.Origin.y, bounds.Origin.z);
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(eCenter, bounds.Radius);
			if( side == Plane::NEGATIVE_SIDE )
				return false;
		}

		return true;
	}
 
	bool Frustrum::Inside(SphereBounds const& bounds) const
	{
		glm::vec3 eCenter(bounds.Origin.x, bounds.Origin.y, bounds.Origin.z);
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(eCenter, bounds.Radius);
			if( side != Plane::POSITIVE_SIDE )
				return false;
		}

		return true;
	}

	bool Frustrum::Inside(BoxBounds & bounds) const
	{
		if( !bounds.IsValid() )
		{
			BOOST_ASSERT(0);
			return false;
		}

		glm::vec3 center;
		glm::vec3 extend;
		bounds.GetCenterAndExtents(center,extend);

		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(center, extend);
			if( side != Plane::POSITIVE_SIDE )
				return false;
		}

		return true;
	}
 

	void Frustrum::Construct(const glm::mat4& viewProjInvMatrix)
	{
		//near
		_Points[0] = glm::vec3(-1.f, 1.0f, 0.f);
		_Points[1] = glm::vec3(1.f, 1.f, 0.f);
		_Points[2] = glm::vec3(1.f, -1.f, 0.f);
		_Points[3] = glm::vec3(-1.f, -1.f, 0.f);

		// far
		_Points[4] = glm::vec3(-1.f, 1.0f, 1.0f);
		_Points[5] = glm::vec3(1.f, 1.f, 1.f);
		_Points[6] = glm::vec3(1.f, -1.f, 1.f);
		_Points[7] = glm::vec3(-1.f, -1.f, 1.f);
 
		for(int i=0;i<8;i++ )
		{ 
			Math::Transform(viewProjInvMatrix, _Points[i]);
		}

		_Planes[PS_Near]   = Plane(_Points[0],_Points[1],_Points[2]);
		_Planes[PS_Far]    = Plane(_Points[4], _Points[6], _Points[5]);
		_Planes[PS_Left]   = Plane(_Points[0], _Points[7], _Points[4]);
		_Planes[PS_Right]  = Plane(_Points[2], _Points[5], _Points[6]);
		_Planes[PS_Top]    = Plane(_Points[1], _Points[4], _Points[5]);
		_Planes[PS_Bottom] = Plane(_Points[3], _Points[6], _Points[7]);
	}

	void Frustrum::Draw()
	{

		glm::vec4 color(1.0f,0,1,1.0f);

		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[1],color);
		GEngine->GameCanvas->DrawLine(_Points[1],color,_Points[2],color);
		GEngine->GameCanvas->DrawLine(_Points[2],color,_Points[3],color);
		GEngine->GameCanvas->DrawLine(_Points[3],color,_Points[0],color);

		GEngine->GameCanvas->DrawLine(_Points[4],color,_Points[5],color);
		GEngine->GameCanvas->DrawLine(_Points[5],color,_Points[6],color);
		GEngine->GameCanvas->DrawLine(_Points[6],color,_Points[7],color);
		GEngine->GameCanvas->DrawLine(_Points[7],color,_Points[4],color);

		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[4],color);
		GEngine->GameCanvas->DrawLine(_Points[1],color,_Points[5],color);
		GEngine->GameCanvas->DrawLine(_Points[2],color,_Points[6],color);
		GEngine->GameCanvas->DrawLine(_Points[3],color,_Points[7],color);

	 
		color = glm::vec4(0, 1, 0, 1);
		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[0] + _Planes[PS_Near].Normal,glm::vec4(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[4], color, _Points[4] + _Planes[PS_Far].Normal, glm::vec4(0, 0, 0, 1.0f));
		GEngine->GameCanvas->DrawLine(_Points[0], color, _Points[0] + _Planes[PS_Left].Normal, glm::vec4(0, 0, 0, 1.0f));
		GEngine->GameCanvas->DrawLine(_Points[2], color, _Points[2] + _Planes[PS_Right].Normal, glm::vec4(0, 0, 0, 1.0f));
		GEngine->GameCanvas->DrawLine(_Points[1], color, _Points[1] + _Planes[PS_Top].Normal, glm::vec4(0, 0, 0, 1.0f));
		GEngine->GameCanvas->DrawLine(_Points[3], color, _Points[3] + _Planes[PS_Bottom].Normal, glm::vec4(0, 0, 0, 1.0f));
	}

}