#include "WinLauncher.h"

using namespace Disorder;

HINSTANCE GAppInstance;

void AppPeekMessage()
{
	MSG Msg;
	while( PeekMessage(&Msg,NULL,0,0,PM_REMOVE) )
	{
		TranslateMessage( &Msg );
		DispatchMessage( &Msg );
	}
}


 void InitGame()
 {
	GLogger = new Logger;
	GMainLoop = new MainLoop;

	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);//
	std::wstring fullPath = exeFullPath;
	boost::to_lower(fullPath);
	std::wstring fstr(TEXT("disorder"));
	int pos = fullPath.find(fstr);
	fullPath = fullPath.substr(0, pos + fstr.size());
	GConfig = new Config(boost::locale::conv::from_utf(fullPath, "UTF-8"));

	GEngine = new Engine;
	GClient = new WinClient;
	Viewport* viewport = GClient->GetViewport(0);
	if (GConfig->pRenderConfig->RenderEngine == Disorder::RET_DirectX)
	{	
		GRenderEngine = new DX11RenderEngine((HWND)viewport->GetWindow());
		GRenderResourceMgr = new DX11RenderResourceManager;
	}
	else if (GConfig->pRenderConfig->RenderEngine == Disorder::RET_OpenGL)
	{
		GRenderEngine = new GLRenderEngine;
		GRenderResourceMgr = new GLRenderResourceManager;
	}
	GRenderSurface = new RenderSurfaceCache;

	GCanvas = new Canvas(viewport->SizeX, viewport->SizeY);
	GConsole = new Console;

	GSceneImporter = new FbxSceneImporter;
	GWorld = new World;
	GSceneManager = new SceneManager;
 }

 void ExitGame()
 {
	 delete GWorld;
	 GWorld = NULL;

	 delete GSceneImporter;
	 GSceneImporter = NULL;

	 delete GCanvas;
	 GCanvas = NULL;
	 
	 delete GConsole;
	 GConsole = NULL;

	 delete GRenderSurface;
	 GRenderSurface = NULL;

	 delete GSceneImporter;
	 GSceneImporter = NULL;

	 delete GRenderEngine;
	 GRenderEngine = NULL;

	 delete GRenderResourceMgr;
	 GRenderResourceMgr = NULL;

	 delete GEngine;
	 GEngine = NULL;

	 delete GClient;
	 GClient = NULL;

	 delete GMainLoop;
	 GMainLoop = NULL;

	 delete GConfig;
	 GConfig = NULL;

	 delete GLogger;
	 GLogger = NULL;
 }

int WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	GAppInstance = hInInstance;

	InitGame();
		
    while( !GIsRequestingExit )
	{
	    GMainLoop->Tick();
		AppPeekMessage();
	}

	ExitGame();
 
	return 0;
}
