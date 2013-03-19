#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		
		std::vector<VertexElementPtr>		    Elements;
		std::vector<unsigned int>				Indices;
		
	 
		// The size 
		int VertexSize;
		int VertexCount;

		TopologyType PrimitiveType;
	};
}






#endif