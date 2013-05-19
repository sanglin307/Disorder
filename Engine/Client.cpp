#include "EngineInclude.h"

namespace Disorder
{
	std::vector<ViewportPtr> Client::_Viewports;

	Client::Client()
	{
	}
	
	Client::~Client()
	{
		ReleaseAllInputListener();
	}

	void Client::Init()
	{
 
	}

	void Client::Tick(float delta)
	{
	}

	void Client::CreateViewport(int x,int y,int sizeX,int sizeY,void* hWnd)
	{
	}

	void Client::Exit()
	{
	}
 
	void Client::AddKeyboardEvent(KeyboardInputEvent const& keyEvent)
	{
		//close game event
		if( keyEvent.key == OIS::KC_ESCAPE && keyEvent.state == IS_Release )
		{
			Close();
			return;
		}

		_keyboardEvents.push_back(keyEvent);
	}
	
	void Client::AddMouseEvent(MouseInputEvent const& mouseEvent)
	{
		_mouseEvents.push_back(mouseEvent);
	}

	void Client::AddInputListener(InputListenerPtr listener)
	{
		if( _inputListenerList.empty() )
		{
			_inputListenerList.push_back(listener);
			return;
		}

		std::list<InputListenerPtr>::iterator iter = _inputListenerList.begin();
		while( iter != _inputListenerList.end() )
		{
			if( *iter == listener )
				break;

			iter++;
		}

		if( iter == _inputListenerList.end() )
			_inputListenerList.push_back(listener);
		
	}
		
	void Client::ReleaseInputListener(InputListenerPtr listener)
	{
		if( _inputListenerList.empty() )
			return;
 
		_inputListenerList.remove(listener);
	}

	void Client::ReleaseAllInputListener()
	{
		_inputListenerList.clear();
	}
  
}