#ifndef _DISORDER_WINCLIENT_H_
#define _DISORDER_WINCLIENT_H_
 

namespace Disorder
{
	 class WinViewport : public Viewport
	 {
	 public:
		 WinViewport(int sizeX, int sizeY, void* hWnd);
		 virtual void Render();
		 virtual inline void* GetWindow()
		 {
			 return _window;
		 }

		 bool MessageProcess(HWND hWnd, UINT Message, UINT wParam, LONG lParam);

	 private:
		 HWND _window;
	 };
 
	class WinClient : public Client
	{
	public:
		WinClient();
		~WinClient();
		virtual void Tick(float delta);
		virtual void Close();
		void ProcessInput(float delta);
	protected:
		virtual void CreateViewport(int sizeX,int sizeY,void* hWnd);
	    static LONG APIENTRY StaticWndProc( HWND hWnd, UINT Message, UINT wParam, LONG lParam );
		bool MessageProcess(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
	
	};


}

#endif