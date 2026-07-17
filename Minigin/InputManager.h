#pragma once
#include "Singleton.h"
#include "Commands.h"

#include <SDL3/SDL.h>

#if __EMSCRIPTEN__
#include <SDL3/SDL_gamepad.h>
#else
#include <Windows.h>
#include <Xinput.h>
#endif

namespace dae
{

	enum class ControllerButton
	{
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		A,
		B,
		X,
		Y
	};

	enum class ButtonState
	{
		Down,
		Up,
		Pressed,
		None
	};

	class Controller final
	{
	public:

		explicit Controller(int controllerIndex);
		~Controller();

		void Update();
		bool IsPressed(ControllerButton button) const;
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button) const;

	private:
		class Impl;
		Impl* m_pImpl;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:

		bool ProcessInput();

		void BindKeyboardCommand(SDL_Scancode key, ButtonState state, BaseGameObjectCommand* command);
		void BindControllerCommand(ControllerButton button, ButtonState state, BaseGameObjectCommand* command, int controllerIndex = 0);

		void UnbindKeyboardCommand(SDL_Scancode key, ButtonState state);
		void UnbindControllerCommand(ControllerButton button, ButtonState state, int controllerIndex = 0);


	private:
		struct KeyboardBinding
		{
			SDL_Scancode key;
			ButtonState state;
			BaseGameObjectCommand* command;
		};

		struct ControllerBinding
		{
			ControllerButton button;
			ButtonState state;
			BaseGameObjectCommand* command;
			int controllerIndex;
		};

		std::vector<KeyboardBinding> m_KeyboardBindings{};
		std::vector<ControllerBinding> m_ControllerBindings{};
		std::vector<std::unique_ptr<Controller>> m_Controllers{};

		std::vector<uint8_t> m_PreviousKeyboardState{};
	};
}