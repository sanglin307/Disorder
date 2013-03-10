#ifndef _DISORDER_CLIENT_H_
#define _DISORDER_CLIENT_H_

namespace Disorder
{
	class Client
	{
	public:
        Client();
		virtual ~Client();
		virtual void Init();
		virtual void Tick(float delta);
		virtual void Exit();

		virtual void Close(){};

		void AddKeyboardEvent(KeyboardInputEvent const& keyEvent);
		void AddMouseEvent(MouseInputEvent const& mouseEvent);

		void AddInputListener(InputListenerPtr listener);
		void ReleaseInputListener(InputListenerPtr listener);
		void ReleaseAllInputListener();
 

		inline ViewportPtr const& GetViewport(unsigned int Index)
		{
			BOOST_ASSERT(Index >= 0 && Index <_Viewports.size());
			return _Viewports[Index];
		}

		inline InputManagerPtr const& GetInputManager() const
		{
			return _inputManager;
		}
		
	protected:
		virtual void CreateViewport(int x,int y,int sizeX,int sizeY,void* hWnd);

	protected:
	    static std::vector<ViewportPtr> _Viewports;
	    InputManagerPtr _inputManager;
		std::list<KeyboardInputEvent> _keyboardEvents;
		std::list<MouseInputEvent>  _mouseEvents;
		std::list<InputListenerPtr> _inputListenerList;

	};

	
}
 

#endif