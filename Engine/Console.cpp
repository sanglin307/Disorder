#include "EngineInclude.h"

namespace Disorder
{
	Console* GConsole = NULL;

	Console::Console()
	{
		GClient->AddInputListener(this);
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
			GClient->GetViewport(0)->ToggleRenderPath();
			return true;
		}

		if( state == IS_Release && key == OIS::KC_C ) // Camera update strategy
		{
			Camera* mainCamera = GSceneManager->GetDefaultCamera();
			if( mainCamera )
			{
				mainCamera->ToggleUpdateStratety();
			}
		}

		if (state == IS_Release && key == OIS::KC_F) // FXAA
		{
			GConfig->pRenderConfig->FXAA = !GConfig->pRenderConfig->FXAA;
		}

		return false;
	}

	bool Console::MouseEvent(MouseInputEvent const& mouseEvent,float deltaSeconds)
	{
		return false;
	}
}