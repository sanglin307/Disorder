#include "EngineInclude.h"

namespace Disorder
{

	GeometryPtr GeometryGenerator::CreateCube(float length)
	{
		GeometryPtr geometry = boost::make_shared<Geometry>();
		geometry->PrimitiveType = TT_TriangleList;

		// vertex..
		// 0
		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, -1.0f )*length);

		//1
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, -1.0f )*length);
	 
		//2
		geometry->Positions.push_back(Vector3( 1.0f, 1.0f, 1.0f )*length);

		//3
		geometry->Positions.push_back(Vector3( -1.0f, 1.0f, 1.0f )*length);

		//4
		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, -1.0f )*length);

		//5
		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, -1.0f )*length);
		 
		//6
		geometry->Positions.push_back(Vector3( 1.0f, -1.0f, 1.0f )*length);

		//7
		geometry->Positions.push_back(Vector3( -1.0f, -1.0f, 1.0f )*length);
 
		//0
		geometry->Indices.push_back(3); geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
		geometry->Indices.push_back(1); geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		geometry->Indices.push_back(0); geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		//1
		geometry->Indices.push_back(2); geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		geometry->Indices.push_back(1); geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		geometry->Indices.push_back(3); geometry->Normals.push_back(Vector3(0.0f,1.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));

		//2
		geometry->Indices.push_back(6); geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		geometry->Indices.push_back(4); geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		geometry->Indices.push_back(5);	geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));

		//3
		geometry->Indices.push_back(7); geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
		geometry->Indices.push_back(4); geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		geometry->Indices.push_back(6); geometry->Normals.push_back(Vector3( 0.0f, -1.0f, 0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));

		//4
		geometry->Indices.push_back(3); geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
		geometry->Indices.push_back(4);	geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		geometry->Indices.push_back(7); geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		//5
		geometry->Indices.push_back(0); geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		geometry->Indices.push_back(4); geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		geometry->Indices.push_back(3); geometry->Normals.push_back(Vector3(-1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));

		//6
		geometry->Indices.push_back(1); geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		geometry->Indices.push_back(6); geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		geometry->Indices.push_back(5); geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));

		//7
		geometry->Indices.push_back(2); geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
		geometry->Indices.push_back(6); geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		geometry->Indices.push_back(1); geometry->Normals.push_back(Vector3(1.0f,0.0f,0.0f)); geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));

		//8
		geometry->Indices.push_back(0); geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));
		geometry->Indices.push_back(5); geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		geometry->Indices.push_back(4); geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));

		//9
		geometry->Indices.push_back(1); geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));geometry->Texcoords.push_back(Vector2( 1.0f, 1.0f ));
		geometry->Indices.push_back(5); geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));
		geometry->Indices.push_back(0); geometry->Normals.push_back(Vector3(0.0f, 0.0f, -1.0f));geometry->Texcoords.push_back(Vector2( 0.0f, 1.0f ));

		//10
		geometry->Indices.push_back(2); geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));geometry->Texcoords.push_back( Vector2( 1.0f, 1.0f ));
		geometry->Indices.push_back(7); geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		geometry->Indices.push_back(6); geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));geometry->Texcoords.push_back(Vector2( 1.0f, 0.0f ));

		//11
		geometry->Indices.push_back(3); geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));geometry->Texcoords.push_back( Vector2(0.0f, 1.0f ));
		geometry->Indices.push_back(7); geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));geometry->Texcoords.push_back(Vector2( 0.0f, 0.0f ));
		geometry->Indices.push_back(2); geometry->Normals.push_back(Vector3( 0.0f, 0.0f, 1.0f));geometry->Texcoords.push_back( Vector2( 1.0f, 1.0f ));

		 
		return geometry;
	}
}