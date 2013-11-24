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
	   RenderEngine->CreateViewport(viewport->GetWindow());
	   RenderEngine->ResourceManager->Init();
	   GameCanvas = boost::make_shared<Canvas>(viewport->SizeX,viewport->SizeY);

	   GSceneManager->Init();
 
	   GWorld->Init();
	   
   }

   void Engine::Tick(float deltaSeconds)
   {
	   Stat.Tick(deltaSeconds);
	   //Client first
	   GameClient->Tick(deltaSeconds);
 
	   //GWorld
	   GWorld->Tick(deltaSeconds);

	   //tick camera here
	   GSceneManager->Tick(deltaSeconds);

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
 
		Vector4 color(1.0f);
		CanvasPtr canvas = GEngine->GameCanvas;

		std::stringstream strstream;
		strstream << "Drawed triangle num:" << GDrawTriNumber;
		std::string drawstr = strstream.str();
		int length = canvas->GetStringLength(_iDrawSize,drawstr);
		canvas->DrawString(Vector3(canvas->GetWidth() - length - 5.f ,10.f,0.f),_iDrawSize,color,drawstr);
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
	   float x = 10.f;
	   float y = 10.f;
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
		canvas->DrawString(Vector3(x,y,0.f),_iDrawSize,color,strstream.str());
		strstream.clear();

   }
 
}