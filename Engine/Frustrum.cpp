#include "EngineInclude.h"

namespace Disorder
{
	Frustrum::Frustrum()
	{	
		memset(_Points,0,8*sizeof(Eigen::Vector3f));
	}

	bool Frustrum::Overlaps(BoxBounds & bounds) const
	{
		if( !bounds.IsValid() )
		{
			BOOST_ASSERT(0);
			return false;
		}

		Vector3f center;
		Vector3f extend;
		bounds.GetCenterAndExtents(center,extend);
		Eigen::Vector3f eCenter(center.x,center.y,center.z);
		Eigen::Vector3f eExtend(extend.x,extend.y,extend.z);
		
		for(int i=0;i<6;i++)
		{
			int side = Math::GetPlaneSide(_Planes[i],eCenter,eExtend);
			if( side > 0 )
				return false;
		}

		return true;
	}

	bool Frustrum::Overlaps(SphereBounds const& bounds) const
	{
		Eigen::Vector3f eCenter(bounds.Origin.x,bounds.Origin.y,bounds.Origin.z);
		for(int i=0;i<6;i++)
		{
			int side = Math::GetPlaneSide(_Planes[i],eCenter,bounds.Radius);
			if( side > 0 )
				return false;
		}

		return true;
	}


	bool Frustrum::Inside(SphereBounds const& bounds) const
	{
		Eigen::Vector3f eCenter(bounds.Origin.x,bounds.Origin.y,bounds.Origin.z);
		for(int i=0;i<6;i++)
		{
			int side = Math::GetPlaneSide(_Planes[i],eCenter,bounds.Radius);
			if( side >= 0 )
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

		Vector3f center;
		Vector3f extend;
		bounds.GetCenterAndExtents(center,extend);
		Eigen::Vector3f eCenter(center.x,center.y,center.z);
		Eigen::Vector3f eExtend(extend.x,extend.y,extend.z);

		for(int i=0;i<6;i++)
		{
			int side = Math::GetPlaneSide(_Planes[i],eCenter,eExtend);
			if( side >= 0 )
				return false;
		}

		return true;
	}
 

	void Frustrum::Construct(const Matrix4f& viewProjInvMatrix)
	{
		//near
		_Points[0] = Vector3f(-1.f,1.0f,0.f);
		_Points[1] = Vector3f(1.f,1.f,0.f);
		_Points[2] = Vector3f(1.f,-1.f,0.f);
		_Points[3] = Vector3f(-1.f,-1.f,0.f);

		// far
		_Points[4] = Vector3f(-1.f,1.0f,1.0f);
		_Points[5] = Vector3f(1.f,1.f,1.f);
		_Points[6] = Vector3f(1.f,-1.f,1.f);
		_Points[7] = Vector3f(-1.f,-1.f,1.f);
 
		for(int i=0;i<8;i++ )
		{ 
			_Points[i] =  viewProjInvMatrix * _Points[i];
		}

	/*	_Planes[PS_Near]   = Eigen::Hyperplane<float,3>::Through(_Points[0],_Points[1],_Points[2]);
		_Planes[PS_Far]    = Eigen::Hyperplane<float,3>::Through(_Points[4],_Points[6],_Points[5]);
		_Planes[PS_Left]   = Eigen::Hyperplane<float,3>::Through(_Points[0],_Points[7],_Points[4]);
		_Planes[PS_Right]  = Eigen::Hyperplane<float,3>::Through(_Points[2],_Points[5],_Points[6]);
		_Planes[PS_Top]    = Eigen::Hyperplane<float,3>::Through(_Points[1],_Points[4],_Points[5]);
		_Planes[PS_Bottom] = Eigen::Hyperplane<float,3>::Through(_Points[3],_Points[6],_Points[7]);*/
	}

	void Frustrum::Draw()
	{
		Vector4f color(1.0f,0,1,1.0f);

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

		/*color = Vector4f(0,1.f,0,1.f);
		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[0] + _Planes[PS_Near].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[4],color,_Points[4] + _Planes[PS_Far].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[0] + _Planes[PS_Left].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[2],color,_Points[2] + _Planes[PS_Right].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[1],color,_Points[1] + _Planes[PS_Top].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[3],color,_Points[3] + _Planes[PS_Bottom].normal(),Eigen::Vector4f(0,0,0,1.0f));*/
	}

}