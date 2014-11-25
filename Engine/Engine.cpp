#include "EngineInclude.h"
 

namespace Disorder
{
   Engine *GEngine = NULL;

   Engine::Engine()
   {
	   FileManager = new FileSystem;
	   SurfaceCache = new RenderSurfaceCache;
	   SceneImporter = new FbxSceneImporter;
   }

   Engine::~Engine()
   {
	   if (FileManager != NULL)
	   {
		   delete FileManager;
		   FileManager = NULL;
	   }

	   if (SurfaceCache != NULL)
	   {
		   delete SurfaceCache;
		   SurfaceCache = NULL;
	   }

	   if (SceneImporter != NULL)
	   {
		   delete SceneImporter;
		   SceneImporter = NULL;
	   }
   }

   void Engine::Init()
   {
	   //Client Init
	   GLogger->Init();
	   RenderEngine->Init();
	   GClient->Init();

	   Viewport* viewport = GClient->GetViewport(0);
	   RenderEngine->CreateViewport(viewport->GetWindow());	 

	   RenderResourceMgr->Init();
	 //  GFontManager->LoadFontImageData();
	   // shadowMap
	   unsigned int shadowSize = GConfig->pRenderConfig->ShadowMapSize;
	   GEngine->SurfaceCache->ShadowMapBuffer = new ShadowMap(shadowSize, shadowSize);

	   viewport->SetRenderPath(RPT_DeferredShading);
	   SceneImporter->Init();

	   GameCanvas = new Canvas(viewport->SizeX,viewport->SizeY);
	   GameConsole = new Console();
	   GSceneManager->Init();
 
	   GWorld->Init();
	   
   }

   void Engine::Tick(float deltaSeconds)
   {
	 
	   //Client first
	   GClient->Tick(deltaSeconds);
 
	   //GWorld
	   GWorld->Tick(deltaSeconds);

	   //tick camera here
	   GSceneManager->Tick(deltaSeconds);

	   Stat.Tick(deltaSeconds);

	   //Draw Evertything~
	   GClient->GetViewport(0)->Render();

	   Stat.FrameCounter++;

   }

   void Engine::Exit()
   {
	   RenderResourceMgr->Exit();
	 
	   GWorld->Exit();
	   GSceneManager->Exit();
	   GClient->Exit();
	   SceneImporter->Exit();
	   RenderEngine->Exit();
	   GLogger->Exit();

   }

   void EngineStat::DrawStat()
   {
	   if (!_bDrawEnable)
			return;

		std::stringstream strstream;
		strstream << "Drawed triangle num:" << _lastFrameDrawTriNumber;
		std::string drawstr = strstream.str();
		unsigned int length = GEngine->GameCanvas->GetStringLength(drawstr);
		GEngine->GameCanvas->DrawString(GConfig->pRenderConfig->SizeX - 5 - length, 10, drawstr);
   }

   void EngineStat::Tick(float deltaSeconds)
   {
	   static int frameCount = 0;
	   static double timeDelta = 0.0;

	   timeDelta += deltaSeconds;
	   frameCount ++;

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
		GEngine->GameCanvas->DrawString(5, 10, strstream.str());
		strstream.clear();

   }
 
}