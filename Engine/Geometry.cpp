#include "EngineInclude.h"

namespace Disorder
{

	GeometryPtr GeometryGenerator::CreateCube(float length)
	{
		GeometryPtr geometry = boost::make_shared<Geometry>();
		geometry->PrimitiveType = TT_TriangleList;

		// vertex..
		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
	 
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));

		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));

		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		 
		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));

		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));

		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));

		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		
		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
		 
		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		
		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
 
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
 
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
			 
		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		 
		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, -1.0f )*length);
		geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
 
		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));
		geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
 
		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));
		geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));
		geometry->Texcoords.push_back( Vector2( 1.0f, 1.0f ));
	 
		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, 1.0f )*length);
		geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));
		geometry->Texcoords.push_back( Vector2(0.0f, 1.0f ));

		geometry->Indices.push_back(3);
		geometry->Indices.push_back(1);
		geometry->Indices.push_back(0);

		geometry->Indices.push_back(2);
		geometry->Indices.push_back(1);
		geometry->Indices.push_back(3);

		geometry->Indices.push_back(6);
		geometry->Indices.push_back(4);
		geometry->Indices.push_back(5);

		geometry->Indices.push_back(7);
		geometry->Indices.push_back(4);
		geometry->Indices.push_back(6);

		geometry->Indices.push_back(11);
		geometry->Indices.push_back(9);
		geometry->Indices.push_back(8);

		geometry->Indices.push_back(10);
		geometry->Indices.push_back(9);
		geometry->Indices.push_back(11);

		geometry->Indices.push_back(14);
		geometry->Indices.push_back(12);
		geometry->Indices.push_back(13);

		geometry->Indices.push_back(15);
		geometry->Indices.push_back(12);
		geometry->Indices.push_back(14);

		geometry->Indices.push_back(19);
		geometry->Indices.push_back(17);
		geometry->Indices.push_back(16);

		geometry->Indices.push_back(18);
		geometry->Indices.push_back(17);
		geometry->Indices.push_back(19);

		geometry->Indices.push_back(22);
		geometry->Indices.push_back(20);
		geometry->Indices.push_back(21);

		geometry->Indices.push_back(23);
		geometry->Indices.push_back(20);
		geometry->Indices.push_back(22);

		 
		return geometry;
	}
}