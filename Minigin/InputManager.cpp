#include "InputManager.h"

#include <array>
#include <memory>

#pragma comment(lib, "Xinput.lib")

WORD ToXInputButton(dae::ControllerButton button)
{
	switch (button)
	{
	case dae::ControllerButton::DPadUp:    return XINPUT_GAMEPAD_DPAD_UP;
	case dae::ControllerButton::DPadDown:  return XINPUT_GAMEPAD_DPAD_DOWN;
	case dae::ControllerButton::DPadLeft:  return XINPUT_GAMEPAD_DPAD_LEFT;
	case dae::ControllerButton::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;
	case dae::ControllerButton::A:         return XINPUT_GAMEPAD_A;
	case dae::ControllerButton::B:         return XINPUT_GAMEPAD_B;
	case dae::ControllerButton::X:         return XINPUT_GAMEPAD_X;
	case dae::ControllerButton::Y:         return XINPUT_GAMEPAD_Y;
	default:                               return 0;
	}
}

#if __EMSCRIPTEN__
dae::Impl::~Impl()
{
	if (m_pGamepad)
	{
		SDL_CloseGamepad(m_pGamepad);
		m_pGamepad = nullptr;
	}
}

SDL_GamepadButton ToSDLButton(dae::ControllerButton button)
{
	switch (button)
	{
	case dae::ControllerButton::DPadUp:    return SDL_GAMEPAD_BUTTON_DPAD_UP;
	case dae::ControllerButton::DPadDown:  return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
	case dae::ControllerButton::DPadLeft:  return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
	case dae::ControllerButton::DPadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
	case dae::ControllerButton::A:         return SDL_GAMEPAD_BUTTON_SOUTH;
	case dae::ControllerButton::B:         return SDL_GAMEPAD_BUTTON_EAST;
	case dae::ControllerButton::X:         return SDL_GAMEPAD_BUTTON_WEST;
	case dae::ControllerButton::Y:         return SDL_GAMEPAD_BUTTON_NORTH;
	default:                              return SDL_GAMEPAD_BUTTON_INVALID;
	}
}

void dae::Impl::Update()
{
	for (int index{}; index < static_cast<int>(SDL_GAMEPAD_BUTTON_COUNT); ++index)
	{
		m_PreviousButtons[index] = m_CurrentButtons[index];
	}

	OpenGamepadIfNeeded();

	if (!m_pGamepad)
	{
		for (int index{}; index < static_cast<int>(SDL_GAMEPAD_BUTTON_COUNT); ++index)
		{
			m_CurrentButtons[index] = false;
		}
		m_IsConnected = false;
		return;
	}

	m_IsConnected = true;

	for (int index{}; index < static_cast<int>(SDL_GAMEPAD_BUTTON_COUNT); ++index)
	{
		m_CurrentButtons[index] = SDL_GetGamepadButton
		(
			m_pGamepad,
			static_cast<SDL_GamepadButton>(index)
		);
	}
}

bool dae::Impl::IsPressed(ControllerButton button) const
{
	const auto sdlButton = ToSDLButton(button);

	if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID)
	{
		return false;
	}

	return m_CurrentButtons[static_cast<int>(sdlButton)];
}

bool dae::Impl::IsDownThisFrame(ControllerButton button) const
{
	const auto sdlButton = ToSDLButton(button);

	if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID)
	{
		return false;
	}

	const int index = static_cast<int>(sdlButton);

	return !m_PreviousButtons[index] && m_CurrentButtons[index];
}

bool dae::Impl::IsUpThisFrame(ControllerButton button) const
{
	const auto sdlButton = ToSDLButton(button);

	if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID)
	{
		return false;
	}

	const int index = static_cast<int>(sdlButton);

	return m_PreviousButtons[index] && !m_CurrentButtons[index];
}

void dae::Impl::OpenGamepadIfNeeded()
{
	if (m_pGamepad)
	{
		return;
	}

	int count = 0;
	SDL_JoystickID* gamepads = SDL_GetGamepads(&count);

	if (!gamepads)
	{
		return;
	}

	if (m_ControllerIndex < count)
	{
		m_pGamepad = SDL_OpenGamepad(gamepads[m_ControllerIndex]);
	}

	SDL_free(gamepads);
}

#else

dae::Impl::Impl(int controllerIndex)
	: m_ControllerIndex(controllerIndex)
	, m_pCurrentState{ new XINPUT_STATE{} }
	, m_pPreviousState{ new XINPUT_STATE{} }
{
}

void dae::Impl::Update()
{
	m_pPreviousState = m_pCurrentState;
	ZeroMemory(m_pCurrentState, sizeof(XINPUT_STATE));

	const DWORD result{ XInputGetState((DWORD)m_ControllerIndex, m_pCurrentState) };
	m_IsConnected = (result == ERROR_SUCCESS);

	if (!m_IsConnected)
	{
		ZeroMemory(m_pCurrentState, sizeof(XINPUT_STATE));
		m_ButtonsPressedThisFrame = 0;
		m_ButtonsReleasedThisFrame = 0;
		return;
	}

	const auto buttonChanges =
		m_pCurrentState->Gamepad.wButtons ^ m_pPreviousState->Gamepad.wButtons;

	m_ButtonsPressedThisFrame =
		buttonChanges & m_pCurrentState->Gamepad.wButtons;

	m_ButtonsReleasedThisFrame =
		buttonChanges & (~m_pCurrentState->Gamepad.wButtons);
}

bool dae::Impl::IsPressed(ControllerButton button) const
{
	const WORD xButton = ToXInputButton(button);
	return (m_pCurrentState->Gamepad.wButtons & xButton) != 0;
}

bool dae::Impl::IsUpThisFrame(ControllerButton button) const
{
	const WORD xButton = ToXInputButton(button);
	return (m_ButtonsPressedThisFrame & xButton) != 0;
}

bool dae::Impl::IsDownThisFrame(ControllerButton button) const
{
	const WORD xButton = ToXInputButton(button);
	return (m_ButtonsReleasedThisFrame & xButton) != 0;
}



#endif

dae::Controller::Controller(int controllerIndex)
	: m_pImpl{ new dae::Impl{controllerIndex} }
{
}

dae::Controller::~Controller()
{
	delete m_pImpl;
}

void dae::Controller::Update()
{
	m_pImpl->Update();
}

bool dae::Controller::IsPressed(ControllerButton button) const
{
	return m_pImpl->IsPressed(button);
}

bool dae::Controller::IsDownThisFrame(ControllerButton button) const
{
	return m_pImpl->IsDownThisFrame(button);
}

bool dae::Controller::IsUpThisFrame(ControllerButton button) const
{
	return m_pImpl->IsUpThisFrame(button);
}	

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			return false;
		}

	}

	const bool* keyboardState = SDL_GetKeyboardState(nullptr);

	// Check and execute all valid keyboard bindings
	for (auto& binding : m_KeyboardBindings)
	{
		bool execute = false;

		switch (binding.state)
		{
		case ButtonState::None:
			break;

		case ButtonState::Pressed:
			execute = keyboardState[binding.key];
			break;

			// 0 0 - nothing
			// 0 1 - down
			// 1 1 - pressed
			// 1 0 - up

		case ButtonState::Down:
			execute = !m_PreviousKeyboardState[binding.key] && keyboardState[binding.key];
			break;

		case ButtonState::Up:
			execute = m_PreviousKeyboardState[binding.key] && !keyboardState[binding.key];
			break;
		}

		if (execute)
		{
			binding.command->Execute();
		}
	}

	m_PreviousKeyboardState.assign(
		keyboardState,
		keyboardState + SDL_SCANCODE_COUNT
	);

	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	// Check and execute all valid controller bindings
	for (auto& binding : m_ControllerBindings)
	{
		bool execute = false;

		if (binding.controllerIndex < 0 ||
			static_cast<std::size_t>(binding.controllerIndex) >= m_Controllers.size())
		{
			continue;
		}

		auto& controller = m_Controllers[static_cast<std::size_t>(binding.controllerIndex)];

		switch (binding.state)
		{
		case ButtonState::None:
			break;

		case ButtonState::Pressed:
			execute = controller->IsPressed(binding.button);
			break;

		case ButtonState::Down:
			execute = controller->IsDownThisFrame(binding.button);
			break;

		case ButtonState::Up:
			execute = controller->IsUpThisFrame(binding.button);
			break;
		}

		if (execute)
		{
			binding.command->Execute();
		}
	}

	return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Scancode key, ButtonState state, BaseGameObjectCommand* command)
{
	assert(command && "BindKeyboardCommand received nullptr command");

	if (key == SDL_SCANCODE_UNKNOWN || state == ButtonState::None)
	{
		return;
	}

	m_KeyboardBindings.push_back(KeyboardBinding{ key, state, std::move(command) });

	if(key + 1 > m_PreviousKeyboardState.size())
	{
		m_PreviousKeyboardState.reserve(key + 1);

		const unsigned int loopCount{unsigned int(m_PreviousKeyboardState.capacity() - m_PreviousKeyboardState.size()) };
		for (unsigned int index{}; index < loopCount; index++)
		{
			m_PreviousKeyboardState.emplace_back(0);
		}
	}
}

void dae::InputManager::BindControllerCommand(ControllerButton button, ButtonState state, BaseGameObjectCommand* command, int controllerIndex)
{
	assert(command && "BindControllerCommand received nullptr command");

	if (state == ButtonState::None || controllerIndex < 0)
	{
		return;
	}

	if (m_Controllers.size() == 0)
	{
		m_Controllers.reserve(1);
		m_Controllers.emplace_back(std::make_unique<dae::Controller>(0));
	}

	m_ControllerBindings.push_back(ControllerBinding{ button, state, std::move(command), controllerIndex });
}

void dae::InputManager::UnbindKeyboardCommand(SDL_Scancode key, ButtonState state)
{
	if (key == SDL_SCANCODE_UNKNOWN || state == ButtonState::None)
	{
		return;
	}

	for (auto iterator = m_KeyboardBindings.begin(); iterator != m_KeyboardBindings.end(); ++iterator)
	{
		if (iterator->key == key && iterator->state == state)
		{
			m_KeyboardBindings.erase(iterator);
			return;
		}
	}
}

void dae::InputManager::UnbindControllerCommand(ControllerButton button, ButtonState state, int controllerIndex)
{
	if (state == ButtonState::None || controllerIndex < 0)
	{
		return;
	}

	for (auto iterator = m_ControllerBindings.begin(); iterator != m_ControllerBindings.end(); ++iterator)
	{
		if (iterator->button == button &&
			iterator->state == state &&
			iterator->controllerIndex == controllerIndex)
		{
			m_ControllerBindings.erase(iterator);
			return;
		}
	}
}