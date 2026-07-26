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
#include "HitboxComponent.h"
#include "Scene.h"
#include "Timer.h"
#include "SoundManager.h"

#include "FileReaders.h"

#include <string> 
#include <filesystem>

static void load()
{
	dae::ServiceLocator::RegisterService(std::make_unique<dae::SoundSystem>());

	//register sounds in order of index
	dae::ServiceLocator::GetService().RegisterSound({ 0, "Data/TestSong2.mp3" });
	
	dae::ServiceLocator::GetService().play({ 0, 1.f });

	//load timer
	auto timer = dae::Timer{};
	timer.Reset();

	//load scene manager
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//load level
	auto level = dae::LevelReader::ReadLevelFile("Data/Level_1.txt", scene, 32);


	//load decoy 1
	auto player1 = std::make_unique<dae::GameObject>();
	player1->SetTexture("Peter_Pepper.png");
	player1->SetTextureDimensions(56, 56);
	player1->SetLocalPosition(512, 632);
	player1->AddComponent<dae::HealthComponent>(3);
	player1->AddComponent<dae::PointsComponent>(0);
	player1->AddComponent<dae::HitboxComponent>(0.f, 0.f, 32.f, 32.f);
	player1->movementSpeed = 100.f;


	//load decoy 2
	auto player2 = std::make_unique<dae::GameObject>();
	player2->SetTexture("Decoy_Sad.png");
	player2->SetLocalPosition(450, 200);
	player2->AddComponent<dae::HealthComponent>(3);
	player2->AddComponent<dae::PointsComponent>(0);
	player2->movementSpeed = 200.f;


	//load a font using the provided texture
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//load top text
	auto topText = std::make_unique<dae::GameObject>();
	topText->SetLocalPosition(292, 20);
	topText->AddComponent<dae::TextComponent>(font, SDL_Color(255, 255, 0, 255));
	topText->GetComponent<dae::TextComponent>()->AddTextPart(0, "Burger Time");

	//load fps text
	auto fps = std::make_unique<dae::GameObject>();
	fps->SetLocalPosition(20, 20);
	fps->AddComponent<dae::TextComponent>(font, SDL_Color(255, 255, 255, 255));
	fps->GetComponent<dae::TextComponent>()->AddTextPart(0, "[INVALID] FPS");
	fps->AddComponent<dae::FPSComponent>();

	//load hp1 object
	auto hp1 = std::make_unique<dae::GameObject>();
	hp1->SetLocalPosition(20, 160);
	std::string hp1Task{"UpdateHealth"};
	hp1->AddComponent<dae::TextComponent>(font, SDL_Color(255, 255, 255, 255), &hp1Task);
	hp1->GetComponent<dae::TextComponent>()->AddTextPart(0, "Player 1 Health: ");
	hp1->GetComponent<dae::TextComponent>()->AddTextPart(1, std::to_string(player1->GetComponent<dae::HealthComponent>()->GetHealth()));

	//load points1 object
	auto points1 = std::make_unique<dae::GameObject>();
	points1->SetLocalPosition(20, 240);
	std::string points1Task{ "UpdatePoints" };
	points1->AddComponent<dae::TextComponent>(font, SDL_Color(255, 255, 255, 255), &points1Task);
	points1->GetComponent<dae::TextComponent>()->AddTextPart(0, "Player 1 Points: ");
	points1->GetComponent<dae::TextComponent>()->AddTextPart(1, std::to_string(player1->GetComponent<dae::PointsComponent>()->GetPoints()));

	//events
	player1.get()->AddObserver(hp1->GetComponent<dae::TextComponent>());
	player1.get()->AddObserver(points1->GetComponent<dae::TextComponent>());

	//commands
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::DPadLeft,
		dae::ButtonState::Pressed,
		new dae::MoveLeft{ player1 },
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::DPadRight,
		dae::ButtonState::Pressed,
		new dae::MoveRight{ player1 },
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::DPadUp,
		dae::ButtonState::Pressed,
		new dae::MoveUp{ player1 },
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::DPadDown,
		dae::ButtonState::Pressed,
		new dae::MoveDown{ player1 },
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::X,
		dae::ButtonState::Down,
		new dae::GetPoints{ player1 },
		0);
	dae::InputManager::GetInstance().BindControllerCommand(
		dae::ControllerButton::Y,
		dae::ButtonState::Up,
		new dae::TakeDamage{ player1 },
		0);

	//scene adds in order(invalidates)
	scene.Add(level);
	scene.Add(player1);
	scene.Add(topText);
	scene.Add(fps);
	scene.Add(hp1);
	scene.Add(points1);
}

int main(int, char*[]) 
{
#if __EMSCRIPTEN__
	std::filesystem::path data_location = "";
#else
	std::filesystem::path data_location = "./Data/";
	if(!std::filesystem::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
