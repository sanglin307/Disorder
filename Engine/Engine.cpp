#include "EngineInclude.h"
#include "Logger.h"

namespace Disorder
{
   InitialiseSingleton(Engine);

   Engine::Engine()
   {
   }

   Engine::~Engine()
   {
   }

   void Engine::Init()
   {
	   //Client Init
	   GLogger->Init();
	   SceneImporter->Init();
	   GameClient->Init();
	   RenderEngine->Init();
	   

	   ViewportPtr const& viewport = GameClient->GetViewport(0);
	   RenderTargetPtr renderTarget = RenderEngine->CreateRenderTarget(GConfig->sRenderConfig,viewport->GetWindow());

	  // GameCanvas = boost::make_shared<Canvas>(viewport->SizeX,viewport->SizeY);

	   GSceneManager->Init();

	   //create camera
	   CameraPtr sceneCamera = boost::make_shared<Camera>();
	   sceneCamera->LookAt(Vector3(0.0f,1.0f,-5.0f),Vector3(0.0f,1.0f,0.0f),Vector3(0.0f,1.0f,0.0f));
	   sceneCamera->ProjCalculate(Math::PI/2,viewport->SizeX*1.0f/viewport->SizeY,1.0f,1000.0f);
	   GSceneManager->SceneCamera = sceneCamera;
	   GameClient->AddInputListener(sceneCamera);

	   GWorld->Init();
	   
   }

   void Engine::Tick(float deltaSeconds)
   {
	   Stat.Tick(deltaSeconds);
	   //Client first
	   GameClient->Tick(deltaSeconds);

	   //tick camera here
	   GSceneManager->SceneCamera->Tick(deltaSeconds);

	   //GWorld
	   GWorld->Tick(deltaSeconds);

	   //Draw Evertything~
	   GameClient->GetViewport(0)->Draw();


   }

   void Engine::Exit()
   {
	   GWorld->Exit();
	   GSceneManager->Exit();
	   RenderEngine->Exit();
	   GameClient->Exit();
	   SceneImporter->Exit();
	   GLogger->Exit();

   }

   void EngineStat::DrawStat()
   {
		if( !_bEnable )
			return;

		int x = 450;
		int y = 10;
		int charSize = 16;
		Vector4 color(1.0f);
		CanvasPtr canvas = GEngine->GameCanvas;

		std::stringstream strstream;
		strstream << "Drawed triangle num:" << GDrawTriNumber;
		canvas->DrawString(x,y,charSize,color,strstream.str());
   }

   void EngineStat::Tick(float deltaSeconds)
   {
	   if( !_bEnable )
		   return;

	   static int frameCount = 0;
	   static float timeDelta = 0.0f;

	   timeDelta += deltaSeconds;
	   frameCount ++;

	   CanvasPtr canvas = GEngine->GameCanvas;
	   int x = 10;
	   int y = 10;
	   int charSize = 16;
	   Vector4 color(1.0f);

	   static int fps = (int)(1/deltaSeconds);
	   if( timeDelta > 1 )
	   {
		   fps = (int)(frameCount/timeDelta);
		   frameCount = 0;
		   timeDelta = 0.0f;
	   }

	    std::stringstream strstream;
		strstream << "Fps:" << fps;
		canvas->DrawString(x,y,charSize,color,strstream.str());
		strstream.clear();

   }
 
}