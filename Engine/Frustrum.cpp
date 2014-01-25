#include "EngineInclude.h"

namespace Disorder
{
	Frustrum::Frustrum()
	{	
		memset(_Planes,0,6*sizeof(Plane));
		memset(_Points,0,8*sizeof(Vector3));
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

	void Frustrum::Construct(const Matrix4 &ViewProj)
	{
		/*
		 | 0  4  8  12 |
		 | 1  5  9  13 |
		 | 2  6  10 14 |
		 | 3  7  11 15 |
		*/

		_Planes[PS_Near].Normal[0] = ViewProj[3][0] + ViewProj[2][0];
		_Planes[PS_Near].Normal[1] = ViewProj[3][1] + ViewProj[2][1];
		_Planes[PS_Near].Normal[2] = ViewProj[3][2] + ViewProj[2][2];
		_Planes[PS_Near].D         = ViewProj[3][3] + ViewProj[2][3];

		_Planes[PS_Right].Normal[0] = ViewProj[3][0] - ViewProj[0][0];
		_Planes[PS_Right].Normal[1] = ViewProj[3][1] - ViewProj[0][1];
		_Planes[PS_Right].Normal[2] = ViewProj[3][2] - ViewProj[0][2];
		_Planes[PS_Right].D         = ViewProj[3][3] - ViewProj[0][3];

		_Planes[PS_Left].Normal[0] = ViewProj[3][0] + ViewProj[0][0];
		_Planes[PS_Left].Normal[1] = ViewProj[3][1] + ViewProj[0][1];
		_Planes[PS_Left].Normal[2] = ViewProj[3][2] + ViewProj[0][2];
		_Planes[PS_Left].D         = ViewProj[3][3] + ViewProj[0][3];

		_Planes[PS_Bottom].Normal[0] = ViewProj[3][0] + ViewProj[1][0];
		_Planes[PS_Bottom].Normal[1] = ViewProj[3][1] + ViewProj[1][1];
		_Planes[PS_Bottom].Normal[2] = ViewProj[3][2] + ViewProj[1][2];
		_Planes[PS_Bottom].D         = ViewProj[3][3] + ViewProj[1][3];

		_Planes[PS_Top].Normal[0] = ViewProj[3][0] - ViewProj[1][0];
		_Planes[PS_Top].Normal[1] = ViewProj[3][1] - ViewProj[1][1];
		_Planes[PS_Top].Normal[2] = ViewProj[3][2] - ViewProj[1][2];
		_Planes[PS_Top].D         = ViewProj[3][3] - ViewProj[1][3];

		_Planes[PS_Far].Normal[0] = ViewProj[3][0] - ViewProj[2][0];
		_Planes[PS_Far].Normal[1] = ViewProj[3][1] - ViewProj[2][1];
		_Planes[PS_Far].Normal[2] = ViewProj[3][2] - ViewProj[2][2];
		_Planes[PS_Far].D         = ViewProj[3][3] - ViewProj[2][3];

		 for(int i=0;i<6;i++ )
		 {
			 _Planes[i].D = -_Planes[i].D;
			 _Planes[i].Normalise();
		 }
		 
		 
	}

	void Frustrum::Construct(const Matrix4& viewMatrix,const Matrix4& projMatrix)
	{
		//near
		_Points[0].x = -1.f;
		_Points[0].y = 1.0f;
		_Points[0].z = 0.f;

		_Points[1].x = 1.f;
		_Points[1].y = 1.f;
		_Points[1].z = 0.f;

		_Points[2].x = 1.f;
		_Points[2].y = -1.f;
		_Points[2].z = 0.f;

		_Points[3].x = -1.f;
		_Points[3].y = -1.f;
		_Points[3].z = 0.f;

		// far
		_Points[4].x = -1.f;
		_Points[4].y = 1.0f;
		_Points[4].z = 1.f;

		_Points[5].x = 1.f;
		_Points[5].y = 1.f;
		_Points[5].z = 1.f;

		_Points[6].x = 1.f;
		_Points[6].y = -1.f;
		_Points[6].z = 1.f;

		_Points[7].x = -1.f;
		_Points[7].y = -1.f;
		_Points[7].z = 1.f;

		Matrix4 viewProjMatrix = projMatrix.Transpose() * viewMatrix.Transpose();
		Matrix4 inverseMatrix = viewProjMatrix.Inverse();

		for(int i=0;i<8;i++ )
		{ 
			_Points[i] =  inverseMatrix * _Points[i];
		}

		_Planes[PS_Near]   = Plane(_Points[0],_Points[2],_Points[1]);
		_Planes[PS_Far]    = Plane(_Points[4],_Points[5],_Points[6]);
		_Planes[PS_Left]   = Plane(_Points[0],_Points[4],_Points[7]);
		_Planes[PS_Right]  = Plane(_Points[2],_Points[6],_Points[5]);
		_Planes[PS_Top]    = Plane(_Points[1],_Points[5],_Points[4]);
		_Planes[PS_Bottom] = Plane(_Points[3],_Points[7],_Points[6]);
	}

	void Frustrum::Draw()
	{
		Vector4 color(1.0f,0,1,1.0f);

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
		GEngine->GameCanvas->DrawLine(_Points[3],color,_Points[3] + _Planes[PS_Bottom].Normal,Vector4(0,0,0,1.0f));
	}

}