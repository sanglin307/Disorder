#include "EngineInclude.h"

namespace Disorder
{
	GameObjectPtr SceneObjectGenerator::CreateCamera(std::string const& name,Vector3 const& position,Vector3 const& lookAt,Vector3 const& upVec,
			                                     float aspect,float FOV, float nearPlane,float farPlane)
	{
	   GameObjectPtr go = boost::make_shared<GameObject>(name,position);
	   CameraPtr sceneCamera = boost::make_shared<Camera>(name);
	   sceneCamera->LookAt(position,lookAt,upVec);
	   sceneCamera->ProjCalculate(FOV,aspect,nearPlane,farPlane);
	   GEngine->GameClient->AddInputListener(sceneCamera);
	   go->AddComponent(sceneCamera);
	   
	   return go;

	}
}