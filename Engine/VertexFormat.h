#ifndef _DISORDER_VERTEXFORMAT_H_
#define _DISORDER_VERTEXFORMAT_H_

namespace Disorder
{
	struct BatchTileVertex
	{
		Vector3 position;
		Vector4 color;
		Vector2 texcoord;
	};

	struct BatchLineVertex
	{
		Vector3 position;
		Vector4 color;
	};

	struct TileTexVertex
	{
		Vector3 position;
		Vector2 texcoord;
	};



}






#endif