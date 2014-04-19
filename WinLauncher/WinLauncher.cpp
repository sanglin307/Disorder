#include "WinLauncher.h"

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
	 //set environment
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);//
	std::wstring fullPath = exeFullPath;
	boost::to_lower(fullPath);
	std::wstring fstr(TEXT("disorder"));
	int pos = fullPath.find(fstr);
	fullPath = fullPath.substr(0,pos+fstr.size());	
	Disorder::GConfig->sRootPath = boost::locale::conv::from_utf(fullPath,"UTF-8");
	Disorder::GConfig->sConfigPath = Disorder::GConfig->sRootPath + "\\Config\\";
	Disorder::GConfig->sResourceFBXPath = Disorder::GConfig->sRootPath + "\\Resource\\Fbx\\";
	Disorder::GConfig->sResourceFXPath = Disorder::GConfig->sRootPath + "\\Resource\\FX\\";
	Disorder::GConfig->sResourceTexPath = Disorder::GConfig->sRootPath + "\\Resource\\Texture\\";
	Disorder::GConfig->sResourceFontPath = Disorder::GConfig->sRootPath + "\\Resource\\Font\\";
	Disorder::GConfig->Load();
 
	Disorder::GEngine->GameClient = Disorder::WinClient::Create();

	if( Disorder::GConfig->pRenderConfig->RenderEngine == Disorder::RET_DirectX )
	{
		Disorder::GEngine->RenderEngine = Disorder::DX11RenderEngine::Create();
	    Disorder::GEngine->RenderResourceMgr = Disorder::DX11RenderResourceManager::Create();
	    
	}
	else if( Disorder::GConfig->pRenderConfig->RenderEngine == Disorder::RET_OpenGL )
	{
		Disorder::GEngine->RenderEngine = Disorder::GLRenderEngine::Create();
	    Disorder::GEngine->RenderResourceMgr = Disorder::GLRenderResourceManager::Create();
	}

	Disorder::GEngine->RenderSurfaceCache = Disorder::RenderSurfaceCache::Create();

	Disorder::GEngine->SceneImporter = Disorder::FbxSceneImporter::Create();
	Disorder::GMainLoop->Init();

 }

int WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	GAppInstance = hInInstance;

	InitGame();
		
    while( !Disorder::GIsRequestingExit )
	{
	    Disorder::GMainLoop->Tick();
		AppPeekMessage();
	}

	Disorder::GMainLoop->Exit();
 
	return 0;
}
