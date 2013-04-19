#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		std::string                             Name;
		std::vector<Vector3>		            Positions;   // Controll point, no duplicate
		std::vector<Vector4>                    Colors;      // Foreach Polygon Index.
		std::vector<Vector3>                    Normals;     // Foreach Polygon Index.
		std::vector<Vector3>                    Tangents;
		std::vector<Vector3>                    Binormals;  
		std::vector<Vector2>                    Texcoords;   // Foreach Polygon Index.
		std::vector<unsigned int>				Indices;     // conterclock


		TopologyType PrimitiveType;
	};

	class GeometryGenerator
	{
	public :

		static GeometryPtr CreateCube(float length);
	};
}






#endif