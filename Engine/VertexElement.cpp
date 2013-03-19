#include "EngineInclude.h"

namespace Disorder
{
	std::string VertexElement::PositionSemantic = "POSITION";
	std::string VertexElement::NormalSemantic = "NORMAL";
	std::string VertexElement::TexCoordSemantic = "TEXCOORDS";
	std::string VertexElement::BoneIDSemantic = "BONEIDS";
	std::string VertexElement::BoneWeightSemantic = "BONEWEIGHTS";
	std::string VertexElement::TangentSemantic = "TANGENT";

	//--------------------------------------------------------------------------------
	VertexElement::VertexElement( int tuple, int elementCount )
	{
		_iTuple = tuple;
		_iCount = elementCount;
		_pfData = new float[ _iTuple * _iCount ];
	}
	//--------------------------------------------------------------------------------
	VertexElement::~VertexElement()
	{
		delete[] _pfData;
	}
	//--------------------------------------------------------------------------------
	int	VertexElement::SizeInBytes()
	{
		return( _iTuple * sizeof(float) );
	}
	//--------------------------------------------------------------------------------
	int	VertexElement::Tuple()
	{
		return( _iTuple );
	}
	//--------------------------------------------------------------------------------
	int VertexElement::Count()
	{
		return( _iCount );
	}
	//--------------------------------------------------------------------------------
	float* VertexElement::Get1f( int i )
	{
		return( &_pfData[ i ] );
	}
	//--------------------------------------------------------------------------------
	Vector2* VertexElement::Get2f( int i )
	{
		return( (Vector2*)&_pfData[ 2*i ] );
	}
	//--------------------------------------------------------------------------------
	Vector3* VertexElement::Get3f( int i )
	{
		return( (Vector3*)&_pfData[ 3*i ] );
	}
	//--------------------------------------------------------------------------------
	Vector4* VertexElement::Get4f( int i )
	{
		return( (Vector4*)&_pfData[ 4*i ] );
	}
	//--------------------------------------------------------------------------------
	int* VertexElement::Get1i( int i )
	{
		return( (int*)&_pfData[ i ] );
	}
	//--------------------------------------------------------------------------------
	unsigned int* VertexElement::Get1ui( int i )
	{
		return( (unsigned int*)&_pfData[ i ] );
	}
	//--------------------------------------------------------------------------------
	float* VertexElement::operator[]( int i )
	{
		return( &_pfData[ _iTuple * i ] );
	}
	//--------------------------------------------------------------------------------
	const float* VertexElement::operator[]( int i ) const
	{
		return( &_pfData[ _iTuple * i ] );
	}
	//--------------------------------------------------------------------------------
	void* VertexElement::GetPtr( int i )
	{
		return( &_pfData[ _iTuple * i ] );
	}
//--------------------------------------------------------------------------------
}