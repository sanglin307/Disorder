#ifndef _DISORDER_VERTEXFORMAT_H_
#define _DISORDER_VERTEXFORMAT_H_

namespace Disorder
{
	struct BatchTileVertex
	{
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Eigen::Vector3f position;
		Eigen::Vector4f color;
		Eigen::Vector2f texcoord;
	};

	struct BatchLineVertex
	{
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Eigen::Vector3f position;
		Eigen::Vector4f color;
	};

	struct TileTexVertex
	{
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		Eigen::Vector3f position;
		Eigen::Vector2f texcoord;
	};



}






#endif