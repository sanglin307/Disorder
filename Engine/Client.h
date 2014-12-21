#ifndef _DISORDER_CLIENT_H_
#define _DISORDER_CLIENT_H_

namespace Disorder
{
	class Client
	{
	public:
        Client();
		virtual ~Client();
		virtual void Tick(float delta);
		virtual void Close(){};

		void AddKeyboardEvent(KeyboardInputEvent const& keyEvent);
		void AddMouseEvent(MouseInputEvent const& mouseEvent);

		void AddInputListener(InputListener* listener);
		void ReleaseInputListener(InputListener* listener);

		inline Viewport* const& GetViewport(unsigned int Index)
		{
			BOOST_ASSERT(Index >= 0 && Index <_viewports.size());
			return _viewports[Index];
		}

		inline InputManager *GetInputManager() const
		{
			return _inputManager;
		}
		
	protected:
		virtual void CreateViewport(int x,int y,int sizeX,int sizeY,void* hWnd);

	protected:
	    std::vector<Viewport *> _viewports;
	    InputManager* _inputManager;
		std::list<KeyboardInputEvent> _keyboardEvents;
		std::list<MouseInputEvent>  _mouseEvents;
		std::list<InputListener*> _inputListenerList;

	};

	
}
 

#endif