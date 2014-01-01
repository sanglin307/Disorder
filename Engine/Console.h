#ifndef _DISORDER_CONSOLE_H_
#define _DISORDER_CONSOLE_H_


namespace Disorder
{

	class Console : public InputListener
	{
	public:

		virtual bool KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds);
		virtual bool MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds);

		static ConsolePtr Create();
	private:
		Console();

	};

}



#endif