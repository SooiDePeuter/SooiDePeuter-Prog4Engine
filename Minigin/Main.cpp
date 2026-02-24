#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
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
	auto go = std::make_unique<dae::GameObject>();
	go->SetTexture("background.png");
	scene.Add(std::move(go));

	//load logo
	go = std::make_unique<dae::GameObject>();
	go->SetTexture("logo.png");
	go->SetPosition(358, 180);
	scene.Add(std::move(go));

	//load a font using the provided texture
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//text initializer
	auto goText = std::make_unique<dae::GameObject>();

	//load top text
	goText->SetPosition(292, 20);
	goText->AddComponent<dae::TextComponent>(std::string("SooiDePeuter-Prog4Engine"), font, SDL_Color(255, 255, 0, 255));
	scene.Add(std::move(goText));

	//load fps text
	goText = std::make_unique<dae::GameObject>();
	goText->SetPosition(20, 20);
	goText->AddComponent<dae::TextComponent>(std::string("0 FPS"), font, SDL_Color(255, 255, 255, 255));
	goText->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(goText));
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
