#ifndef _DISORDER_VERTEXELEMENT_H_
#define _DISORDER_VERTEXELEMENT_H_


namespace Disorder
{
	class VertexElement
	{
	public:

        // Standard semantic names
        static std::string PositionSemantic;
        static std::string NormalSemantic;
        static std::string TexCoordSemantic;
        static std::string BoneIDSemantic;
		static std::string BoneWeightSemantic;
        static std::string TangentSemantic;

	public:
		VertexElement( int tuple, int elementCount );
		~VertexElement();
		
		int				SizeInBytes();
		int				Count();
		int				Tuple();

		void*			GetPtr( int i );

		float*			Get1f( int i );
		Vector2*		Get2f( int i );
		Vector3*		Get3f( int i );
		Vector4*		Get4f( int i );

		int*			Get1i( int i );

		unsigned int*	Get1ui( int i );

		float*					operator[]( int i );
		const float*			operator[]( int i ) const;
 
	protected:
		VertexElement();

		float*							_pfData;
		int								_iTuple;
		int								_iCount;

	};
}


#endif