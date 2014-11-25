#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		std::string                              Name;
		std::vector<glm::vec3>                   ControllPositions; // Controll point, no duplicate
		std::vector<int>		                 Positions;   // Index to ControllPositions
		std::vector<glm::vec4>                   Colors;
		std::vector<glm::vec3>                   Normals;
		std::vector<glm::vec3>                   Tangents;
		std::vector<glm::vec3>                   Binormals;
		std::vector<glm::vec2>                   Texcoords;
		std::vector<unsigned int>				 Indices;     // conterclock

		BoxSphereBounds                         BoundingBox;

		bool CastShadow;
		TopologyType PrimitiveType;

		void CalculateBounding();
		Geometry(std::string const& name);
	private:
		
	};

	class GeometryGenerator
	{
	public :

		static Geometry* CreateCube(float length);
	};
}






#endif