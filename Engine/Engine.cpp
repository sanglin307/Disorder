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
	   RenderEngine->Init();
	   GameClient->Init();

	   ViewportPtr const& viewport = GameClient->GetViewport(0);
	   RenderEngine->CreateViewport(viewport->GetWindow());	 

	   RenderResourceMgr->Init();
	 //  GFontManager->LoadFontImageData();

	   viewport->SetRenderPath(RPT_DeferredShading);
	   SceneImporter->Init();

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

	   Stat.FrameCounter++;

   }

   void Engine::Exit()
   {
	   RenderResourceMgr->Exit();
	 
	   GWorld->Exit();
	   GSceneManager->Exit();
	   GameClient->Exit();
	   SceneImporter->Exit();
	   RenderEngine->Exit();
	   GLogger->Exit();

   }

   void EngineStat::DrawStat()
   {
	   if (!_bDrawEnable)
			return;
 
		CanvasPtr canvas = GEngine->GameCanvas;

		std::stringstream strstream;
		strstream << "Drawed triangle num:" << _lastFrameDrawTriNumber;
		std::string drawstr = strstream.str();
		unsigned int length = canvas->GetStringLength(1.0f,drawstr);
		canvas->DrawString(GConfig->pRenderConfig->SizeX - 5 - length, 10, drawstr);
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
	  
	   static int fps = (int)(1/deltaSeconds);
	   if( timeDelta > 1 )
	   {
		   fps = (int)(frameCount/timeDelta);
		   frameCount = 0;
		   timeDelta = 0.0;
	   }

	    std::stringstream strstream;
		strstream << "Fps:" << fps << "  Frame Delta:"<< deltaSeconds;
		canvas->DrawString(5,10,strstream.str());
		strstream.clear();

   }
 
}