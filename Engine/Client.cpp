#include "EngineInclude.h"

namespace Disorder
{
	Client* GClient;

	Client::Client()
	{
	}

	void Client::Tick(float delta)
	{
	}

	void Client::CreateViewport(int x,int y,int sizeX,int sizeY,void* hWnd)
	{
	}

	Client::~Client()
	{
		for (size_t i = 0; i < _viewports.size(); i++)
		{
			delete _viewports[i];
		}
		_viewports.empty();

		delete _inputManager;
		_inputManager = NULL;

		_keyboardEvents.empty();
		_mouseEvents.empty();

		// we don't delete the obj of inputlistenner,other place will delete it!
	    _inputListenerList.empty();
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

	void Client::AddInputListener(InputListener* listener)
	{
		if( _inputListenerList.empty() )
		{
			_inputListenerList.push_back(listener);
			return;
		}

		std::list<InputListener*>::iterator iter = _inputListenerList.begin();
		while( iter != _inputListenerList.end() )
		{
			if( *iter == listener )
				break;

			iter++;
		}

		if( iter == _inputListenerList.end() )
			_inputListenerList.push_back(listener);
		
	}
		
	void Client::ReleaseInputListener(InputListener* listener)
	{
		if( _inputListenerList.empty() )
			return;
 
		_inputListenerList.remove(listener);
	}
  
}