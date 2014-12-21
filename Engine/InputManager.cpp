#include "EngineInclude.h"


namespace Disorder
{
	//===========================InputEventHandler=========================
	InputEventHandler::InputEventHandler()
	{

	}

	bool InputEventHandler::keyPressed( const OIS::KeyEvent &arg )
	{
	/*	std::cout << " KeyPressed {" << arg.key
			<< ", " << ((Keyboard*)(arg.device))->getAsString(arg.key)
			<< "} || Character (" << (char)arg.text << ")" << std::endl;*/

		KeyboardInputEvent keyEvent;
		keyEvent.key = arg.key;
		keyEvent.state = IS_Press;
		keyEvent.text = arg.text;

		GClient->AddKeyboardEvent(keyEvent);

	    return true;
	}

	bool InputEventHandler::keyReleased( const OIS::KeyEvent &arg ) 
	{
		/*if( arg.key == KC_ESCAPE || arg.key == KC_Q )
			appRunning = false;
		std::cout << "KeyReleased {" << ((Keyboard*)(arg.device))->getAsString(arg.key) << "}\n";*/

		KeyboardInputEvent keyEvent;
		keyEvent.key = arg.key;
		keyEvent.state = IS_Release;
		keyEvent.text = arg.text;

		GClient->AddKeyboardEvent(keyEvent);


		return true;
	}

	bool InputEventHandler::mouseMoved( const OIS::MouseEvent &arg ) 
	{
		const OIS::MouseState& s = arg.state;
		/*std::cout << "\nMouseMoved: Abs("
				  << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
				  << s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";*/

		MouseInputEvent mouseEvent;
		 
		mouseEvent.AbsoluteX = s.X.abs;
		mouseEvent.AbsoluteY = s.Y.abs;
		mouseEvent.RelativeX = s.X.rel;
		mouseEvent.RelativeY = s.Y.rel;
		mouseEvent.AbsoluteZ = s.Z.abs;
		mouseEvent.RelativeZ = s.Z.rel;

		mouseEvent.buttons = s.buttons;

		GClient->AddMouseEvent(mouseEvent);
 
		return true;
	}

	bool InputEventHandler::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) 
	{
		const OIS::MouseState& s = arg.state;
		/*std::cout << "\nMouse button #" << id << " pressed. Abs("
				  << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
				  << s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";*/

		MouseInputEvent mouseEvent;
		 
		mouseEvent.AbsoluteX = s.X.abs;
		mouseEvent.AbsoluteY = s.Y.abs;
		mouseEvent.RelativeX = s.X.rel;
		mouseEvent.RelativeY = s.Y.rel;
		mouseEvent.AbsoluteZ = s.Z.abs;
		mouseEvent.RelativeZ = s.Z.rel;
		mouseEvent.buttons = s.buttons;

		GClient->AddMouseEvent(mouseEvent);

		return true;
	}

	bool InputEventHandler::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) 
	{
		const OIS::MouseState& s = arg.state;
		/*std::cout << "\nMouse button #" << id << " released. Abs("
				  << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
				  << s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";*/

		MouseInputEvent mouseEvent;
		 
		mouseEvent.AbsoluteX = s.X.abs;
		mouseEvent.AbsoluteY = s.Y.abs;
		mouseEvent.RelativeX = s.X.rel;
		mouseEvent.RelativeY = s.Y.rel;
		mouseEvent.AbsoluteZ = s.Z.abs;
		mouseEvent.RelativeZ = s.Z.rel;
		mouseEvent.buttons = s.buttons;

		GClient->AddMouseEvent(mouseEvent);

		return true;
	}
	//================================InputManager===========================================

	InputManager::InputManager(unsigned int hWnd)
	{
		std::multimap<std::string,std::string> paramList;
		//paramList.insert(std::pair<std::string,std::string>("w32_mouse","DISCL_FOREGROUND"));
		//paramList.insert(std::pair<std::string,std::string>("w32_mouse", "DISCL_NONEXCLUSIVE"));
		std::stringstream stream;
		stream << hWnd;
		paramList.insert(std::pair<std::string,std::string>("WINDOW",stream.str()));

		_oisInputManager = OIS::InputManager::createInputSystem(paramList);
		_oisInputManager->enableAddOnFactory(OIS::InputManager::AddOn_All);

		_eventHandler = new InputEventHandler;

		_oisKeyboard = (OIS::Keyboard*)_oisInputManager->createInputObject( OIS::OISKeyboard, true );
		_oisKeyboard->setEventCallback(_eventHandler);
		_oisMouse    = (OIS::Mouse*)_oisInputManager->createInputObject( OIS::OISMouse, true );
		_oisMouse->setEventCallback(_eventHandler);

		const OIS::MouseState &ms = _oisMouse->getMouseState();
		ms.width = GClient->GetViewport(0)->SizeX;
		ms.height = GClient->GetViewport(0)->SizeX;


	}

	InputManager::~InputManager()
	{
		if (_eventHandler!=NULL)
		   delete _eventHandler;
		OIS::InputManager::destroyInputSystem(_oisInputManager);
	}

	int InputManager::GetMousePosX()
	{
		return _oisMouse->getMouseState().X.abs;
	}

	int InputManager::GetMousePosY()
	{
		return _oisMouse->getMouseState().Y.abs;
	}

	int InputManager::GetMousePosWheel()
	{
		return _oisMouse->getMouseState().Z.abs;
	}

	bool InputManager::IsKeyDown(OIS::KeyCode key)
	{
		return _oisKeyboard->isKeyDown(key);
	}

	void InputManager::Tick(float delta)
	{
		if( _oisKeyboard )
			_oisKeyboard->capture();

		if( _oisMouse )
			_oisMouse->capture();
	}
}