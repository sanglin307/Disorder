#include "EngineInclude.h"

namespace Disorder
{
	Frustrum::Frustrum()
	{	
		memset(_Points,0,8*sizeof(Eigen::Vector4f));
	}

	bool Frustrum::Overlaps(BoxBounds & bounds) const
	{
		for(int i=0;i<6;i++)
		{
			int side = bounds.GetPlaneSide(_Planes[i]);
			if( side > 0 )
				return false;
		}

		return true;
	}

	bool Frustrum::Overlaps(SphereBounds const& bounds) const
	{
		for(int i=0;i<6;i++)
		{
			int side = Math::GetPlaneSide(_Planes[i],bounds.Origin,bounds.Radius);
			if( side > 0 )
				return false;
		}

		return true;
	}


	bool Frustrum::Inside(SphereBounds const& bounds) const
	{
		for(int i=0;i<6;i++)
		{
			int side = Math::GetPlaneSide(_Planes[i],bounds.Origin,bounds.Radius);
			if( side >= 0 )
				return false;
		}

		return true;
	}

	bool Frustrum::Inside(BoxBounds & bounds) const
	{
		for(int i=0;i<6;i++)
		{
			int side = bounds.GetPlaneSide(_Planes[i]);
			if( side >= 0 )
				return false;
		}

		return true;
	}
 

	void Frustrum::Construct(const Eigen::Matrix4f& viewMatrix,const Eigen::Matrix4f& projMatrix)
	{
		//near
		_Points[0] << -1.f,1.0f,0.f,1.0f;
		_Points[1] << 1.f,1.f,0.f,1.0f;
		_Points[2] << 1.f,-1.f,0.f,1.0f;
		_Points[3] << -1.f,-1.f,0.f,1.0f;

		// far
		_Points[4] << -1.f,1.0f,1.0f,1.0f;
		_Points[5] << 1.f,1.f,1.f,1.0f;
		_Points[6] << 1.f,-1.f,1.f,1.0f;
		_Points[7] << -1.f,-1.f,1.f,1.0f;

		Eigen::Matrix4f viewProjMatrix = projMatrix.transpose() * viewMatrix.transpose();
		Eigen::Matrix4f inverseMatrix = viewProjMatrix.inverse();

		for(int i=0;i<8;i++ )
		{ 
			_Points[i] =  inverseMatrix * _Points[i];
		}

		_Planes[PS_Near]   = Eigen::Hyperplane<float,3>::Through(_Points[0].topLeftCorner<3,1>(),_Points[2].topLeftCorner<3,1>(),_Points[1].topLeftCorner<3,1>());
		_Planes[PS_Far]    = Eigen::Hyperplane<float,3>::Through(_Points[4].topLeftCorner<3,1>(),_Points[5].topLeftCorner<3,1>(),_Points[6].topLeftCorner<3,1>());
		_Planes[PS_Left]   = Eigen::Hyperplane<float,3>::Through(_Points[0].topLeftCorner<3,1>(),_Points[4].topLeftCorner<3,1>(),_Points[7].topLeftCorner<3,1>());
		_Planes[PS_Right]  = Eigen::Hyperplane<float,3>::Through(_Points[2].topLeftCorner<3,1>(),_Points[6].topLeftCorner<3,1>(),_Points[5].topLeftCorner<3,1>());
		_Planes[PS_Top]    = Eigen::Hyperplane<float,3>::Through(_Points[1].topLeftCorner<3,1>(),_Points[5].topLeftCorner<3,1>(),_Points[4].topLeftCorner<3,1>());
		_Planes[PS_Bottom] = Eigen::Hyperplane<float,3>::Through(_Points[3].topLeftCorner<3,1>(),_Points[7].topLeftCorner<3,1>(),_Points[6].topLeftCorner<3,1>());
	}

	void Frustrum::Draw()
	{
		Eigen::Vector4f color(1.0f,0,1,1.0f);

		GEngine->GameCanvas->DrawLine(_Points[0].topLeftCorner<3,1>(),color,_Points[1].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[1].topLeftCorner<3,1>(),color,_Points[2].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[2].topLeftCorner<3,1>(),color,_Points[3].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[3].topLeftCorner<3,1>(),color,_Points[0].topLeftCorner<3,1>(),color);

		GEngine->GameCanvas->DrawLine(_Points[4].topLeftCorner<3,1>(),color,_Points[5].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[5].topLeftCorner<3,1>(),color,_Points[6].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[6].topLeftCorner<3,1>(),color,_Points[7].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[7].topLeftCorner<3,1>(),color,_Points[4].topLeftCorner<3,1>(),color);

		GEngine->GameCanvas->DrawLine(_Points[0].topLeftCorner<3,1>(),color,_Points[4].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[1].topLeftCorner<3,1>(),color,_Points[5].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[2].topLeftCorner<3,1>(),color,_Points[6].topLeftCorner<3,1>(),color);
		GEngine->GameCanvas->DrawLine(_Points[3].topLeftCorner<3,1>(),color,_Points[7].topLeftCorner<3,1>(),color);

		color = Eigen::Vector4f(0,1.f,0,1.f);
		GEngine->GameCanvas->DrawLine(_Points[0].topLeftCorner<3,1>(),color,_Points[0].topLeftCorner<3,1>() + _Planes[PS_Near].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[4].topLeftCorner<3,1>(),color,_Points[4].topLeftCorner<3,1>() + _Planes[PS_Far].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[0].topLeftCorner<3,1>(),color,_Points[0].topLeftCorner<3,1>() + _Planes[PS_Left].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[2].topLeftCorner<3,1>(),color,_Points[2].topLeftCorner<3,1>() + _Planes[PS_Right].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[1].topLeftCorner<3,1>(),color,_Points[1].topLeftCorner<3,1>() + _Planes[PS_Top].normal(),Eigen::Vector4f(0,0,0,1.0f));
		GEngine->GameCanvas->DrawLine(_Points[3].topLeftCorner<3,1>(),color,_Points[3].topLeftCorner<3,1>() + _Planes[PS_Bottom].normal(),Eigen::Vector4f(0,0,0,1.0f));
	}

}