#ifndef _DISORDER_SCENEMANAGER_H_
#define _DISORDER_SCENEMANAGER_H_



namespace Disorder
{
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		void Init();
		void Exit();
		void Render();

		void AddRenderer(RendererPtr const& renderer);

		CameraPtr SceneCamera;

	private:
		std::list<RendererPtr> _vRenderObjects;
		SceneImporterPtr _sceneImporter;

	};

	#define GSceneManager SceneManager::GetSingleton()
}





#endif