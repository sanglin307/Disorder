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