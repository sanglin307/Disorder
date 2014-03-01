#include "EngineInclude.h"

namespace Disorder
{

Component::Component(std::string const& name,EComponentType componentType)
	:Name(name),ComponentType(componentType)
{
	_vPos = glm::vec3(0);
	_vScale = glm::vec3(1);
	_vRot = glm::quat(1,0,0,0);
}





}