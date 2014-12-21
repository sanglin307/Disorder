#include "EngineInclude.h"
 

namespace Disorder
{
   Engine *GEngine = NULL;

   Engine::Engine()
   {
	   
   }

   Engine::~Engine()
   {
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

   void EngineStat::DrawStat()
   {
	   if (!_bDrawEnable)
			return;

		std::stringstream strstream;
		strstream << "Drawed triangle num:" << _lastFrameDrawTriNumber;
		std::string drawstr = strstream.str();
		unsigned int length = GCanvas->GetStringLength(drawstr);
		GCanvas->DrawString(GConfig->pRenderConfig->SizeX - 5 - length, 10, drawstr);
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
		GCanvas->DrawString(5, 10, strstream.str());
		strstream.clear();

   }
 
}