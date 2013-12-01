#ifndef _DISORDER_WINCLIENT_H_
#define _DISORDER_WINCLIENT_H_
 

namespace Disorder
{
	 class WindowsViewport : public Viewport
	 {
	 public:
		
		 static WindowsViewportPtr Create(int x,int y,int sizeX,int sizeY,void* hWnd);

		 virtual void Draw();

		 virtual inline void* GetWindow()
		 {
			 return _window;
		 }

	 private:

		 WindowsViewport(int x,int y,int sizeX,int sizeY,void* hWnd);
		 HWND _window;
	 };
	


	class FinderViewport
	{
	public:
		FinderViewport(void* hWnd):_hWnd(hWnd)
		{
		}

		bool inline operator()(ViewportPtr const& viewport)
		{
			if(viewport->GetWindow() == _hWnd)
				return true;

			return false;
		}
	private:
		void * _hWnd;
	};

	extern HINSTANCE GAppInstance;

	class WinClient : public Client
	{
	public:

		virtual void Init();
		virtual void Tick(float delta);
		virtual void Exit();
		virtual void Close();
		void ProcessInput(float delta);

		static WinClientPtr Create();

	protected:
		virtual void CreateViewport(int x,int y,int sizeX,int sizeY,void* hWnd);

	    static LONG APIENTRY StaticWndProc( HWND hWnd, UINT Message, UINT wParam, LONG lParam );

		WinClient(){}
	
	};


}

#endif