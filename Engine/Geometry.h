#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		std::string                             Name;
		std::vector<Vector3>		            Positions;
		std::vector<Vector4>                    Colors;
		std::vector<Vector3>                    Normals;
		std::vector<Vector2>                    Texcoords;
		std::vector<unsigned int>				Indices;


		TopologyType PrimitiveType;
	};

	class GeometryGenerator
	{
	public :

		static GeometryPtr CreateCube(float length);
	};
}






#endif