#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		std::string                              Name;
		std::vector<Vector3f>                    ControllPositions; // Controll point, no duplicate
		std::vector<int>		                 Positions;   // Index to ControllPositions
		std::vector<Vector4f>                    Colors;      
		std::vector<Vector3f>                    Normals;     
		std::vector<Vector3f>                    Tangents;
		std::vector<Vector3f>                    Binormals;  
		std::vector<Vector2f>                    Texcoords;   
		std::vector<unsigned int>				 Indices;     // conterclock

		BoxSphereBounds                         BoundingBox;

		TopologyType PrimitiveType;

		void CalculateBounding();

		static GeometryPtr Create(std::string const& name);

	private:
		Geometry();
	};

	class GeometryGenerator
	{
	public :

		static GeometryPtr CreateCube(float length);
	};
}






#endif