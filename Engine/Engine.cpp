#include "EngineInclude.h"
 

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
	   RenderResourceMgr->Init();
	   RenderEngine->Init();  
	   SceneImporter->Init();
	   GameClient->Init();
	   

	   ViewportPtr const& viewport = GameClient->GetViewport(0);
	   RenderEngine->CreateViewport(viewport->GetWindow());
	 
	   GameCanvas = Canvas::Create(viewport->SizeX,viewport->SizeY);
	   GameConsole = Console::Create();
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
	   GameClient->GetViewport(0)->Render();


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
		canvas->DrawString(Vector2(canvas->GetWidth() - length - 5.f ,10.f),_iDrawSize,color,drawstr);
   }

   EnginePtr Engine::Create()
   {
	   Engine *pEngine = new Engine;
	   return EnginePtr(pEngine);
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
	   Vector4 color(1.0f,0,0,1.0f);

	   static int fps = (int)(1/deltaSeconds);
	   if( timeDelta > 1 )
	   {
		   fps = (int)(frameCount/timeDelta);
		   frameCount = 0;
		   timeDelta = 0.0f;
	   }

	    std::stringstream strstream;
		strstream << "Fps:" << fps;
		canvas->DrawString(Vector2(x,y),_iDrawSize,color,strstream.str());
		strstream.clear();

   }
 
}