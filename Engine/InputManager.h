#ifndef _DISORDER_INPUTMANAGER_H_
#define _DISORDER_INPUTMANAGER_H_

//we use ois to implement it.

#include "..\..\External\ois-v1-3\includes\OIS.h"

namespace Disorder
{
	enum InputState
	{
		IS_Press,
		IS_Release
	};

	typedef struct _KeyboardInputEvent
	{
		//! KeyCode of event
		OIS::KeyCode key;
		//! Text character, depends on current TextTranslationMode
		unsigned int text;
		InputState state;

	} KeyboardInputEvent;

	typedef struct _MouseInputEvent
	{
		int buttons;
		int AbsoluteX;
		int AbsoluteY;
		int RelativeX;
		int RelativeY;
		int AbsoluteZ;
		int RelativeZ;

		//! Button down test
		inline bool buttonDown( OIS::MouseButtonID button ) const
		{
			return ((buttons & ( 1L << button )) == 0) ? false : true;
		}

	} MouseInputEvent;

	class InputListener
	{
	public:
		virtual bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds) = 0;
		virtual bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds) = 0;
	};


	class InputEventHandler : public OIS::KeyListener, public OIS::MouseListener
	{
	public:
		bool keyPressed( const OIS::KeyEvent &arg );
        bool keyReleased( const OIS::KeyEvent &arg );
	    bool mouseMoved( const OIS::MouseEvent &arg );
	    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );	 
	    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ); 
		InputEventHandler();
	};

	class InputManager
	{
	public:
		
		~InputManager();

	    virtual void Tick(float delta);
		virtual bool IsKeyDown(OIS::KeyCode key);
		int  GetMousePosX();
		int GetMousePosY();
		int GetMousePosWheel();
		InputManager(unsigned int hWnd);
	private:
		

		OIS::InputManager*  _oisInputManager;
		OIS::Keyboard*      _oisKeyboard;
		OIS::Mouse*         _oisMouse;

		InputEventHandler*  _eventHandler;

	};
}


#endif