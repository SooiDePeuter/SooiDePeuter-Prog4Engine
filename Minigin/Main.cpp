#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Commands.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "FPSComponent.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "Timer.h"

#include <string> 
#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	//load timer
	auto timer = dae::Timer{};
	timer.Reset();

	//load scene manager
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//load background
	auto backGround = std::make_unique<dae::GameObject>();
	backGround->SetTexture("background.png");
	scene.Add(std::move(backGround));

	//load decoy 1
	auto player1 = std::make_unique<dae::GameObject>();
	player1->SetTexture("Decoy.png");
	player1->SetLocalPosition(400, 200);
	player1->AddComponent<dae::HealthComponent>(3);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::X,
		dae::ButtonState::Down,
		new dae::MoveLeft{},
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::B,
		dae::ButtonState::Down,
		new dae::MoveRight{},
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::Y,
		dae::ButtonState::Down,
		new dae::MoveUp{},
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::A,
		dae::ButtonState::Down,
		new dae::MoveDown{},
		0);
	scene.Add(std::move(player1));

	//load decoy 2
	auto player2 = std::make_unique<dae::GameObject>();
	player2->SetTexture("Decoy.png");
	player2->SetLocalPosition(450, 200);
	player2->AddComponent<dae::HealthComponent>(3);
	dae::InputManager::GetInstance().BindKeyboardCommand(
		SDL_SCANCODE_A,
		dae::ButtonState::Down,
		new dae::MoveLeft{});
	dae::InputManager::GetInstance().BindKeyboardCommand(
		SDL_SCANCODE_D,
		dae::ButtonState::Down,
		new dae::MoveRight{});
	dae::InputManager::GetInstance().BindKeyboardCommand(
		SDL_SCANCODE_W,
		dae::ButtonState::Down,
		new dae::MoveUp{});
	dae::InputManager::GetInstance().BindKeyboardCommand(
		SDL_SCANCODE_S,
		dae::ButtonState::Down,
		new dae::MoveDown{});
	player2->movementSpeed = 2.f;
	scene.Add(std::move(player2));

	//load logo
	player2 = std::make_unique<dae::GameObject>();
	player2->SetTexture("logo.png");
	player2->SetLocalPosition(358, 180);
	scene.Add(std::move(player2));

	//load a font using the provided texture
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//load top text
	auto topText = std::make_unique<dae::GameObject>();
	topText->SetLocalPosition(292, 20);
	topText->AddComponent<dae::TextComponent>(std::string("SooiDePeuter-Prog4Engine"), font, SDL_Color(255, 255, 0, 255));
	scene.Add(std::move(topText));

	//load fps text
	auto fps = std::make_unique<dae::GameObject>();
	fps->SetLocalPosition(20, 20);
	fps->AddComponent<dae::TextComponent>(std::string("[INVALID] FPS"), font, SDL_Color(255, 255, 255, 255));
	fps->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(fps));

	//load hp1 object
	auto hp1 = std::make_unique<dae::GameObject>();
	hp1->SetLocalPosition(20, 200);
	hp1->AddComponent<dae::TextComponent>(std::string("player 1 hp: 0"), font, SDL_Color(255, 255, 255, 255));
	hp1->AddComponent<dae::HealthComponent>();
	scene.Add(std::move(hp1));

	//load hp2 object
	auto hp2 = std::make_unique<dae::GameObject>();
	hp2->SetLocalPosition(20, 240);
	hp2->AddComponent<dae::TextComponent>(std::string("player 2 hp: 0"), font, SDL_Color(255, 255, 255, 255));
	hp2->AddComponent<dae::HealthComponent>();
	scene.Add(std::move(hp2));

	//load points1 object
	auto points1 = std::make_unique<dae::GameObject>();
	points1->SetLocalPosition(20, 220);
	points1->AddComponent<dae::TextComponent>(std::string("player 1 points: 0"), font, SDL_Color(255, 255, 255, 255));
	scene.Add(std::move(points1));

	//load points2 object
	auto points2 = std::make_unique<dae::GameObject>();
	points2->SetLocalPosition(20, 260);
	points2->AddComponent<dae::TextComponent>(std::string("player 2 points: 0"), font, SDL_Color(255, 255, 255, 255));
	scene.Add(std::move(points2));

	//events
	player1.get()->AddObserver(hp1.get());
	player1.get()->AddObserver(points1.get());
	player2.get()->AddObserver(hp2.get());
	player2.get()->AddObserver(points2.get());
}

int main(int, char*[]) 
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
