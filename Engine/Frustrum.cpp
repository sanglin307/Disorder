#include "EngineInclude.h"

namespace Disorder
{
	Frustrum::Frustrum()
	{	
		memset(_Planes,0,6*sizeof(Plane));
		memset(_Points,0,8*sizeof(Eigen::Vector4f));
	}

	bool Frustrum::Overlaps(BoxBounds const& bounds) const
	{
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(bounds);
			if( side == Plane::NO_SIDE || side == Plane::POSITIVE_SIDE )
				return false;
		}

		return true;
	}

	bool Frustrum::Overlaps(SphereBounds const& bounds) const
	{
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(bounds.Origin,bounds.Radius);
			if( side == Plane::NO_SIDE || side == Plane::POSITIVE_SIDE )
				return false;
		}

		return true;
	}


	bool Frustrum::Inside(SphereBounds const& bounds) const
	{
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(bounds.Origin,bounds.Radius);
			if( side != Plane::NEGATIVE_SIDE )
				return false;
		}

		return true;
	}

	bool Frustrum::Inside(BoxBounds const& bounds) const
	{
		for(int i=0;i<6;i++)
		{
			Plane::Side side = _Planes[i].GetSide(bounds);
			if( side != Plane::NEGATIVE_SIDE )
				return false;
		}

		return true;
	}
 

	void Frustrum::Construct(const Eigen::Matrix4f& viewMatrix,const Eigen::Matrix4f& projMatrix)
	{
		//near
		_Points[0] << -1.f,1.0f,0.f,1.f;
		_Points[1] << 1.f,1.f,0.f,1.f;
		_Points[2] << 1.f,-1.f,0.f,1.f;
		_Points[3] << -1.f,-1.f,0.f,1.f;

		// far
		_Points[4] << -1.f,1.0f,1.0f,1.f;
		_Points[5] << 1.f,1.f,1.f,1.f;
		_Points[6] << 1.f,-1.f,1.f,1.f;
		_Points[7] << -1.f,-1.f,1.f,1.f;

		Eigen::Matrix4f viewProjMatrix = projMatrix.transpose() * viewMatrix.transpose();
		Eigen::Matrix4f inverseMatrix = viewProjMatrix.inverse();

		for(int i=0;i<8;i++ )
		{ 
			_Points[i] =  inverseMatrix * _Points[i];
		}

	/*	_Planes[PS_Near]   = Plane(_Points[0],_Points[2],_Points[1]);
		_Planes[PS_Far]    = Plane(_Points[4],_Points[5],_Points[6]);
		_Planes[PS_Left]   = Plane(_Points[0],_Points[4],_Points[7]);
		_Planes[PS_Right]  = Plane(_Points[2],_Points[6],_Points[5]);
		_Planes[PS_Top]    = Plane(_Points[1],_Points[5],_Points[4]);
		_Planes[PS_Bottom] = Plane(_Points[3],_Points[7],_Points[6]);*/
	}

	void Frustrum::Draw()
	{
		/*Vector4 color(1.0f,0,1,1.0f);

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

		color = Vector3(0,1.f,0);
		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[0] + _Planes[PS_Near].Normal,Vector4(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[4],color,_Points[4] + _Planes[PS_Far].Normal,Vector4(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[0],color,_Points[0] + _Planes[PS_Left].Normal,Vector4(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[2],color,_Points[2] + _Planes[PS_Right].Normal,Vector4(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[1],color,_Points[1] + _Planes[PS_Top].Normal,Vector4(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[3],color,_Points[3] + _Planes[PS_Bottom].Normal,Vector4(0,0,0,1.0f));*/
	}

}