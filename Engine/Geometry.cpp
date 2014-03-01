#include "EngineInclude.h"

namespace Disorder
{
	/////////////////////////////////////////////////////////////////////////
	GeometryPtr Geometry::Create(std::string const& name)
	{
		Geometry *pGeometry = new Geometry();
		pGeometry->Name = name;
		return GeometryPtr(pGeometry);

	}

	Geometry::Geometry()
	{

	}

	void Geometry::CalculateBounding()
	{
		if( ControllPositions.size() == 0 )
			return;

		BoundingBox = BoxSphereBounds(ControllPositions.data(),ControllPositions.size());
	}
 
	//////////////////////////////////////////////////////////////////////////
	

	GeometryPtr GeometryGenerator::CreateCube(float length)
	{
		GeometryPtr geometry = Geometry::Create("DefaultCube");
		geometry->PrimitiveType = TT_TriangleList;

        //0
		geometry->ControllPositions.push_back(glm::vec3( -1.0f, 1.0f, -1.0f )*length);
	 	geometry->Positions.push_back(0); geometry->Normals.push_back(glm::vec3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 0.0f ));
  
        //1
		geometry->ControllPositions.push_back(glm::vec3( 1.0f, 1.0f, -1.0f )*length);
        geometry->Positions.push_back(1); geometry->Normals.push_back(glm::vec3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 0.0f ));
  
        //2
		geometry->ControllPositions.push_back(glm::vec3( 1.0f, 1.0f, 1.0f )*length);
        geometry->Positions.push_back(2); geometry->Normals.push_back(glm::vec3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 1.0f ));
 
        //3
		geometry->ControllPositions.push_back(glm::vec3( -1.0f, 1.0f, 1.0f )*length);
        geometry->Positions.push_back(3); geometry->Normals.push_back(glm::vec3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 1.0f ));
 
        //4
		geometry->ControllPositions.push_back(glm::vec3( -1.0f, -1.0f, -1.0f )*length);
        geometry->Positions.push_back(4); geometry->Normals.push_back(glm::vec3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 0.0f ));
 
	    //5
		geometry->ControllPositions.push_back(glm::vec3( 1.0f, -1.0f, -1.0f )*length);
        geometry->Positions.push_back(5); geometry->Normals.push_back(glm::vec3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 0.0f ));
 		 
        //6
		geometry->ControllPositions.push_back(glm::vec3( 1.0f, -1.0f, 1.0f )*length);
        geometry->Positions.push_back(6); geometry->Normals.push_back(glm::vec3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 1.0f ));
 
        //7
		geometry->ControllPositions.push_back(glm::vec3( -1.0f, -1.0f, 1.0f )*length);
        geometry->Positions.push_back(7); geometry->Normals.push_back(glm::vec3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 1.0f ));
 
	    //8
        geometry->Positions.push_back(7); geometry->Normals.push_back(glm::vec3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 0.0f ));
      
	    //9
        geometry->Positions.push_back(4); geometry->Normals.push_back(glm::vec3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 0.0f ));
 
	    //10
        geometry->Positions.push_back(0); geometry->Normals.push_back(glm::vec3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 1.0f ));
 
	    //11
        geometry->Positions.push_back(3); geometry->Normals.push_back(glm::vec3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 1.0f ));
 
	    //12
        geometry->Positions.push_back(6); geometry->Normals.push_back(glm::vec3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 0.0f ));
 
	    //13
        geometry->Positions.push_back(5); geometry->Normals.push_back(glm::vec3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 0.0f ));
 
	    //14
        geometry->Positions.push_back(1); geometry->Normals.push_back(glm::vec3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 1.0f ));

	    //15
        geometry->Positions.push_back(2); geometry->Normals.push_back(glm::vec3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 1.0f ));
 
	    //16
        geometry->Positions.push_back(4); geometry->Normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 0.0f ));
 
	    //17
        geometry->Positions.push_back(5); geometry->Normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 0.0f ));
 		
	    //18
        geometry->Positions.push_back(1); geometry->Normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 1.0f ));
 	 
	    //19
        geometry->Positions.push_back(0); geometry->Normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 1.0f ));
 
	    //20
        geometry->Positions.push_back(7); geometry->Normals.push_back(glm::vec3( 0.0f, 0.0f, 1.0f)); geometry->Texcoords.push_back(glm::vec2( 0.0f, 0.0f ));
 
	    //21
        geometry->Positions.push_back(6); geometry->Normals.push_back(glm::vec3( 0.0f, 0.0f, 1.0f)); geometry->Texcoords.push_back(glm::vec2( 1.0f, 0.0f ));
 
	    //22
        geometry->Positions.push_back(2); geometry->Normals.push_back(glm::vec3( 0.0f, 0.0f, 1.0f)); geometry->Texcoords.push_back( glm::vec2( 1.0f, 1.0f ));
 
	    //23
        geometry->Positions.push_back(3); geometry->Normals.push_back(glm::vec3( 0.0f, 0.0f, 1.0f)); geometry->Texcoords.push_back( glm::vec2(0.0f, 1.0f ));
 
		//triangles
        geometry->Indices.push_back(3); geometry->Indices.push_back(1); geometry->Indices.push_back(0);
        geometry->Indices.push_back(2); geometry->Indices.push_back(1); geometry->Indices.push_back(3);
        geometry->Indices.push_back(6); geometry->Indices.push_back(4); geometry->Indices.push_back(5);
        geometry->Indices.push_back(7); geometry->Indices.push_back(4); geometry->Indices.push_back(6);
        geometry->Indices.push_back(11); geometry->Indices.push_back(9); geometry->Indices.push_back(8);
        geometry->Indices.push_back(10); geometry->Indices.push_back(9); geometry->Indices.push_back(11);
        geometry->Indices.push_back(14); geometry->Indices.push_back(12);geometry->Indices.push_back(13);
        geometry->Indices.push_back(15); geometry->Indices.push_back(12); geometry->Indices.push_back(14);
        geometry->Indices.push_back(19); geometry->Indices.push_back(17); geometry->Indices.push_back(16);
        geometry->Indices.push_back(18); geometry->Indices.push_back(17); geometry->Indices.push_back(19);
        geometry->Indices.push_back(22); geometry->Indices.push_back(20); geometry->Indices.push_back(21); 
        geometry->Indices.push_back(23); geometry->Indices.push_back(20); geometry->Indices.push_back(22);
 
		return geometry;
	}
}