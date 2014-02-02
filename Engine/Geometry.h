#ifndef _DISORDER_GEOMETRY_H_
#define _DISORDER_GEOMETRY_H_
 
namespace Disorder
{
	class Geometry
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		std::string                             Name;
		std::vector<Eigen::Vector3f>                    ControllPositions; // Controll point, no duplicate
		std::vector<int>		                        Positions;   // Index to ControllPositions
		std::vector<Eigen::Vector4f>                    Colors;      
		std::vector<Eigen::Vector3f>                    Normals;     
		std::vector<Eigen::Vector3f>                    Tangents;
		std::vector<Eigen::Vector3f>                    Binormals;  
		std::vector<Eigen::Vector2f>                    Texcoords;   
		std::vector<unsigned int>				Indices;     // conterclock

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