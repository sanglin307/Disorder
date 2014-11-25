#ifndef _DISORDER_CONSOLE_H_
#define _DISORDER_CONSOLE_H_


namespace Disorder
{

	class Console : public InputListener
	{
	public:
		Console();
		virtual bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		virtual bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);
	
	};

	extern Console* GConsole;

}



#endif