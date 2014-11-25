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

void CreateRenderEngine()
{
	if (GConfig->pRenderConfig->RenderEngine == Disorder::RET_DirectX)
	{
		GRenderEngine = new DX11RenderEngine;
		GRenderResourceMgr = new DX11RenderResourceManager;
	}
	else if (GConfig->pRenderConfig->RenderEngine == Disorder::RET_OpenGL)
	{
		GRenderEngine = new GLRenderEngine;
		GRenderResourceMgr = new GLRenderResourceManager;
	}
	else
	{
		BOOST_ASSERT(0);
	}
}

 void InitGame()
 {
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

	CreateRenderEngine();

	GMainLoop->Init();
	GEngine->Init();
	GClient->Init();
 }

 void ExitGame()
 {
	 GRenderEngine->Exit();
	 GRenderResourceManager->Exit();
	 GClient->Exit();
	 GEngine->Exit();
	 GMainLoop->Exit();

	 delete GRenderEngine;
	 GRenderEngine = NULL;

	 delete GRenderResourceManager;
	 GRenderResourceManager = NULL;

	 delete GClient;
	 GClient = NULL;

	 delete GEngine;
	 GEngine = NULL;

	 delete GMainLoop;
	 GMainLoop = NULL;

	 delete GConfig;
	 GConfig = NULL;
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
