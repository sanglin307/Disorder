#include "EngineInclude.h"
 

namespace Disorder
{
   InitialiseSingleton(Engine);

   Engine::Engine()
   {
	   FileManager = FileSystem::Create();
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
	 
	   //Client first
	   GameClient->Tick(deltaSeconds);
 
	   //GWorld
	   GWorld->Tick(deltaSeconds);

	   //tick camera here
	   GSceneManager->Tick(deltaSeconds);

	   Stat.Tick(deltaSeconds);

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
 
		 
		CanvasPtr canvas = GEngine->GameCanvas;

		std::stringstream strstream;
		strstream << "Drawed triangle num:" << GDrawTriNumber;
		std::string drawstr = strstream.str();
		float length = canvas->GetStringLength(0.04f,drawstr);
		canvas->DrawString(0.98f - length ,0.01f,0.04f,Vector4f(1.f,1.f,1.f,1.f),drawstr);
   }

   EnginePtr Engine::Create()
   {
	   Engine *pEngine = new Engine;
	   return EnginePtr(pEngine);
   }

   void EngineStat::Tick(float deltaSeconds)
   {
	   static int frameCount = 0;
	   static double timeDelta = 0.0;

	   timeDelta += deltaSeconds;
	   frameCount ++;

	   CanvasPtr canvas = GEngine->GameCanvas;
	   float x = 10.f;
	   float y = 10.f;
	   Vector4f color(1.0f,0,0,1.0f);

	   static int fps = (int)(1/deltaSeconds);
	   if( timeDelta > 1 )
	   {
		   fps = (int)(frameCount/timeDelta);
		   frameCount = 0;
		   timeDelta = 0.0;
	   }

	    std::stringstream strstream;
		strstream << "Fps:" << fps << "  Frame Delta:"<< deltaSeconds;
		canvas->DrawString(0.01f,0.01f,0.04f,color,strstream.str());
		strstream.clear();

   }
 
}