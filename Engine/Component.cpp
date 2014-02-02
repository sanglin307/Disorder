#include "EngineInclude.h"

namespace Disorder
{

Component::Component(std::string const& name,EComponentType componentType)
	:Name(name),ComponentType(componentType)
{
	_vPos.setOnes();
	_vScale.setOnes();
	_vRot.setIdentity();
}





}