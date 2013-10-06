#include "WinLauncher.h"


void PeekMessage()
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
	Disorder::GConfig->Load();
 

	Disorder::GEngine->GameClient = boost::make_shared<Disorder::WinClient>();
	Disorder::GEngine->RenderEngine = boost::make_shared<Disorder::DX11RenderEngine>();
	Disorder::GEngine->FileManager = boost::make_shared<Disorder::WinFileSystem>();
	Disorder::GEngine->SceneImporter = boost::make_shared<Disorder::FbxSceneImporter>();

	Disorder::GMainLoop->Init();

 }

int WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	Disorder::GAppInstance = hInInstance;

	InitGame();
		
    while( !Disorder::GIsRequestingExit )
	{
	    Disorder::GMainLoop->Tick();
		PeekMessage();
	}

	Disorder::GMainLoop->Exit();
 
	return 0;
}
