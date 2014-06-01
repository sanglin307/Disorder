#ifndef _DISORDER_VERTEXFORMAT_H_
#define _DISORDER_VERTEXFORMAT_H_

namespace Disorder
{
	struct BatchTileVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texcoord;
	};

	struct BatchLineVertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};

	struct TileTexVertex
	{
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	 

}






#endif