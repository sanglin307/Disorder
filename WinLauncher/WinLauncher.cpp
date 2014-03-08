#include "WinLauncher.h"


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
	Disorder::GEngine->RenderEngine = Disorder::GLRenderEngine::Create();
	Disorder::GEngine->SceneImporter = Disorder::FbxSceneImporter::Create();
	Disorder::GEngine->RenderResourceMgr = Disorder::DX11RenderResourceManager::Create();
	Disorder::GEngine->RenderSurfaceCache = Disorder::DX11RenderSurfaceCache::Create();
	Disorder::GMainLoop->Init();

 }

int WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	Disorder::GAppInstance = hInInstance;

	InitGame();
		
    while( !Disorder::GIsRequestingExit )
	{
	    Disorder::GMainLoop->Tick();
		AppPeekMessage();
	}

	Disorder::GMainLoop->Exit();
 
	return 0;
}
