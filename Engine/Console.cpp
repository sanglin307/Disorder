#include "EngineInclude.h"

namespace Disorder
{

	Console::Console()
	{
	}

	bool Console::KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds)
	{
		if( state == IS_Release && key == OIS::KC_H )
		{
			GSceneManager->EnableDebugDraw = !GSceneManager->EnableDebugDraw;
			return true;
		}

		return false;
	}

	bool Console::MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		return false;
	}

	ConsolePtr Console::Create()
	{
		Console *pConsole = new Console;
		ConsolePtr console(pConsole);
		GEngine->GameClient->AddInputListener(console);
		return console;
	}




















}