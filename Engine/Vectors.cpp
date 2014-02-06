#include "EngineInclude.h"

namespace Disorder
{
	const Vector2f Vector2f::ZERO( 0.f, 0.f);

    const Vector2f Vector2f::UNIT_X( 1.f, 0.f);
    const Vector2f Vector2f::UNIT_Y( 0.f, 1.f);
    const Vector2f Vector2f::NEGATIVE_UNIT_X( -1.f,  0.f);
    const Vector2f Vector2f::NEGATIVE_UNIT_Y(  0.f, -1.f);
    const Vector2f Vector2f::UNIT_SCALE(1.f, 1.f);

	//----------------------------------------------
	const Vector3f Vector3f::ZERO( 0.f, 0.f, 0.f );

    const Vector3f Vector3f::UNIT_X( 1.f, 0.f, 0.f );
    const Vector3f Vector3f::UNIT_Y( 0.f, 1.f, 0.f );
    const Vector3f Vector3f::UNIT_Z( 0.f, 0.f, 1.f );
    const Vector3f Vector3f::NEGATIVE_UNIT_X( -1.f,  0.f,  0.f );
    const Vector3f Vector3f::NEGATIVE_UNIT_Y(  0.f, -1.f,  0.f );
    const Vector3f Vector3f::NEGATIVE_UNIT_Z(  0.f,  0.f, -1.f );
    const Vector3f Vector3f::UNIT_SCALE(1.f, 1.f, 1.f);

	//----------------------------------------------
	 const Vector4f Vector4f::ZERO( 0.f, 0.f, 0.f, 0.f );
	 const Vector4f Vector4f::ONE( 1.f, 1.f, 1.f, 1.f );
}