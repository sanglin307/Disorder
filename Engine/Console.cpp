#include "EngineInclude.h"

namespace Disorder
{

	Console::Console()
	{
	}

	bool Console::KeyboardEvent(OIS::KeyCode key,unsigned int text, InputState state,float deltaSeconds)
	{
		if( state == IS_Release && key == OIS::KC_I )  // Info
		{
			GSceneManager->EnableDebugDraw = !GSceneManager->EnableDebugDraw;
			return true;
		}

		if( state == IS_Release && key == OIS::KC_R ) // RenderPath
		{
			GEngine->GameClient->GetViewport(0)->ToggleRenderPath();
			return true;
		}

		if( state == IS_Release && key == OIS::KC_C ) // Camera update strategy
		{
			CameraPtr mainCamera = GSceneManager->GetDefaultCamera();
			if( mainCamera )
			{
				mainCamera->ToggleUpdateStratety();
			}
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