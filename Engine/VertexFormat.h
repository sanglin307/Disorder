#ifndef _DISORDER_VERTEXFORMAT_H_
#define _DISORDER_VERTEXFORMAT_H_

namespace Disorder
{
	struct BatchTileVertex
	{
		Vector3f position;
		Vector4f color;
	    Vector2f texcoord;
	};

	struct BatchLineVertex
	{
		Vector3f position;
		Vector4f color;
	};

	struct TileTexVertex
	{
		Vector3f position;
		Vector2f texcoord;
	};



}






#endif