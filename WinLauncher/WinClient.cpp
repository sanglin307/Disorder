#include "WinLauncher.h"
 

namespace Disorder
{
	HINSTANCE GAppInstance;

	WindowsViewport::WindowsViewport(int x,int y,int sizeX,int sizeY,void *hWnd)
		:Viewport(x,y,sizeX,sizeY),_window((HWND)hWnd)
	{		
	}

	void WindowsViewport::Draw()
	{
		Viewport::Draw();
		
	}

	WindowsViewportPtr WindowsViewport::Create(int x,int y,int sizeX,int sizeY,void* hWnd)
	{
		WindowsViewport *pViewport = new WindowsViewport(x,y,sizeX,sizeY,hWnd);
		return WindowsViewportPtr(pViewport);
	}

	//==========================WindowsClient=================================

	WinClientPtr WinClient::Create()
	{
		WinClient* pClient = new WinClient;
		return WinClientPtr(pClient);
	}

	void WinClient::Init()
	{

		TCHAR *winClassName = TEXT("Disorder");

		WNDCLASSEXW Cls;
		memset( &Cls, 0,sizeof(Cls));
		Cls.cbSize			= sizeof(Cls);
		// disable dbl-click messages in the game as the dbl-click event is sent instead of the key pressed event, which causes issues with e.g. rapid firing
		Cls.style			= CS_HREDRAW | CS_VREDRAW;
		Cls.lpfnWndProc		= StaticWndProc;
		Cls.hInstance		= GAppInstance;		 
		Cls.lpszClassName	= winClassName;
		Cls.hIcon			=(HICON)LoadImage(NULL,L"Icon.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE);
		Cls.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
		Cls.hbrBackground	= static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
		Cls.lpszMenuName    = NULL;

		 
		BOOST_ASSERT(RegisterClassExW(&Cls));
			 
		DWORD WindowStyle;
		WindowStyle = WS_OVERLAPPEDWINDOW;
		// Obtain width and height of primary monitor.
		INT ScreenWidth  = ::GetSystemMetrics( SM_CXSCREEN );
		INT ScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
		INT WindowWidth  = GConfig->pRenderConfig->SizeX;
		INT WindowHeight = GConfig->pRenderConfig->SizeY;
		INT WindowPosX = (ScreenWidth - WindowWidth ) / 2;
		INT WindowPosY = (ScreenHeight - WindowHeight ) / 2;
 
		// Create the window
		HWND windows = CreateWindowW(winClassName,winClassName, WindowStyle, WindowPosX, WindowPosY, WindowWidth, WindowHeight, NULL, NULL, GAppInstance, NULL);
		BOOST_ASSERT(windows);
 
		ShowWindow( windows, SW_SHOWNORMAL );
 
		UpdateWindow(windows);

		//Update RenderConfig
		GConfig->pRenderConfig->X = WindowPosX;
		GConfig->pRenderConfig->Y = WindowPosY;
	 
		// Create viewport
		CreateViewport(WindowPosX,WindowPosY,WindowWidth,WindowHeight,windows);

		//Create InputManager
		_inputManager = InputManager::Create((unsigned int)windows);

	}


	void WinClient::ProcessInput(float delta)
	{
		if( _inputListenerList.size() > 0 )
		{
			if( _keyboardEvents.size() > 0 )
			{
				std::list<KeyboardInputEvent>::iterator iterKey = _keyboardEvents.begin();
				while( iterKey != _keyboardEvents.end() )
				{
					std::list<InputListenerPtr>::iterator inputIter = _inputListenerList.begin();
					while( inputIter != _inputListenerList.end() )
					{
						(*inputIter)->KeyboardEvent(iterKey->key,iterKey->text,iterKey->state,delta);
						inputIter++;
					}

					iterKey++;
				}

				_keyboardEvents.clear();
			}

			if( _mouseEvents.size() > 0 )
			{
				std::list<MouseInputEvent>::iterator iterKey = _mouseEvents.begin();
				while( iterKey != _mouseEvents.end() )
				{
					std::list<InputListenerPtr>::iterator inputIter = _inputListenerList.begin();
					while( inputIter != _inputListenerList.end() )
					{
						(*inputIter)->MouseEvent(*iterKey,delta);
						inputIter++;
					}

					iterKey++;
				}

				_mouseEvents.clear();
			}
		}
	}

	void WinClient::Tick(float delta)
	{
		Client::Tick(delta);

		_inputManager->Tick(delta);

		//Process Input
	    ProcessInput(delta);
	}

	void WinClient::Exit()
	{
		Client::Exit();
	}

	void WinClient::CreateViewport(int x,int y,int sizeX,int sizeY,void* hWnd)
	{
		// only support one viewport now!
		BOOST_ASSERT(_Viewports.empty());
		_Viewports.push_back(WindowsViewport::Create(x,y,sizeX,sizeY,hWnd));

	}

	LONG APIENTRY WinClient::StaticWndProc( HWND hWnd, UINT Message, UINT wParam, LONG lParam )
	{	
		// Prevent power management
		if (Message == WM_POWERBROADCAST)
		{
			switch( wParam )
			{
				case PBT_APMQUERYSUSPEND:
				case PBT_APMQUERYSTANDBY:
					return BROADCAST_QUERY_DENY;
			}
		}
 
		if(_Viewports.empty())
		{
			return DefWindowProc(hWnd, Message, wParam, lParam);
		}

		std::vector<ViewportPtr>::iterator viewportIter = std::find_if(_Viewports.begin(),_Viewports.end(),FinderViewport(hWnd));
	    
		if(viewportIter == _Viewports.end())
		{
			return DefWindowProc(hWnd, Message, wParam, lParam);
		}

		switch (Message)
		{
		case WM_ACTIVATE:
			if (WA_INACTIVE == LOWORD(wParam))
			{
				(*viewportIter)->Active(false);
			}
			else
			{
				(*viewportIter)->Active(true);
			}
			break;

		case WM_ERASEBKGND:
			return 1;

		case WM_PAINT:
			(*viewportIter)->Draw();
			break;
 
		//case WM_SIZE:
		//	// Check to see if we are losing or gaining our window.  Set the
		//	// active flag to match
		//	if ((SIZE_MAXHIDE == wParam) || (SIZE_MINIMIZED == wParam))
		//	{
		//		this->OnSize()(*this, false);
		//	}
		//	else
		//	{
		//		this->OnSize()(*this, true);
		//	}
		//	break;

		//case WM_GETMINMAXINFO:
		//	// Prevent the window from going smaller than some minimu size
		//	reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 100;
		//	reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 100;
		//	break;

		/*case WM_SETCURSOR:
		    this->OnSetCursor()(*this);
			break;*/

		case WM_CLOSE:
			GIsRequestingExit = true;
	        PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	void WinClient::Close()
	{
		GIsRequestingExit = true;
	    PostQuitMessage(0);
	}

}