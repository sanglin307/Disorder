#include "EngineInclude.h"

namespace Disorder
{

Component::Component(std::string const& name,EComponentType componentType)
:Name(name),ComponentType(componentType),_vPos(Vector3::ZERO),_vRot(Quaternion::IDENTITY),_vScale(Vector3::UNIT_SCALE)
{
}





}