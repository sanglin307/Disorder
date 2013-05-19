#ifndef _DISORDER_SCENEOBJECTGENERATOR_H_
#define _DISORDER_SCENEOBJECTGENERATOR_H_



namespace Disorder
{
	class SceneObjectGenerator
	{
	public:

		static GameObjectPtr CreateCamera(std::string const& name,Vector3 const& position = Vector3::ZERO,Vector3 const& lookAt = Vector3::NEGATIVE_UNIT_Z,Vector3 const& upVec= Vector3::UNIT_Y,
			                          float aspect = 4.0f/3.0f,float FOV = Math::PI/3, float nearPlane=0.1f,float farPlane = 100.0f);

	};
}



#endif