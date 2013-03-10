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
	int pos = fullPath.find(TEXT("Disorder"));
	fullPath = fullPath.substr(0,pos+8);	
	Disorder::GConfig->sRunningDictioary = boost::locale::conv::from_utf(fullPath,"UTF-8");
 
	Disorder::GEngine->GameClient = boost::make_shared<Disorder::WinClient>();
	Disorder::GEngine->RenderEngine = Disorder::CreateDX11RenderEngine();
	Disorder::GEngine->FileManager = boost::make_shared<Disorder::WinFileSystem>();

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
