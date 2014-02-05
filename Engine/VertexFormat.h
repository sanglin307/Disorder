#ifndef _DISORDER_VERTEXFORMAT_H_
#define _DISORDER_VERTEXFORMAT_H_

namespace Disorder
{
	_declspec(align(16)) struct BatchTileVertex
	{
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Eigen::Vector3f position;
		Eigen::Vector4f color;
		Eigen::Vector2f texcoord;
	};

	_declspec(align(16)) struct BatchLineVertex
	{
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Eigen::Vector3f position;
		Eigen::Vector4f color;
	};

	_declspec(align(16)) struct TileTexVertex
	{
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Eigen::Vector3f position;
		Eigen::Vector2f texcoord;
	};



}






#endif