#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		std::string                             Name;
		std::vector<Vector3>                    ControllPositions; // Controll point, no duplicate
		std::vector<int>		                Positions;   // Index to ControllPositions
		std::vector<Vector4>                    Colors;      
		std::vector<Vector3>                    Normals;     
		std::vector<Vector3>                    Tangents;
		std::vector<Vector3>                    Binormals;  
		std::vector<Vector2>                    Texcoords;   
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