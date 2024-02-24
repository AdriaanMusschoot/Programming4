#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Header\SceneManager.h"
#include "Header\ResourceManager.h"
#include "Header\GameObject.h"
#include "Header\Scene.h"

#include <filesystem>

#include "SDL_egl.h"
#include "Header/FPSComponent.h"
#include "Header/TextComponent.h"
#include "Header/TextureComponent.h"
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Programming 4 Assignment");

	std::shared_ptr<dae::GameObject> gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<TextureComponent>();
	if (gameObject->ComponentAdded<TextureComponent>())
	{
		//ASK TOM IF THIS IS THEN THE CORRECT WAY TO HANDLE AN EXCEPTION
		//try
		//{
			gameObject->GetComponent<TextureComponent>()->SetTexture("background.tga");
		//}
		//catch(...)
		//{
			//display some text on the window??
		//}
	}
	scene.Add(gameObject);
	
	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<TextureComponent>();
	if (gameObject->ComponentAdded<TextureComponent>())
	{
		gameObject->GetComponent<TextureComponent>()->SetTexture("logo.tga");
		gameObject->GetComponent<TextureComponent>()->SetPosition(216, 180);
	}
	scene.Add(gameObject);

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<TextComponent>("Programming 4 Assignment", "Lingua.otf", 36);
	if (gameObject->ComponentAdded<TextComponent>())
	{
		gameObject->GetComponent<TextComponent>()->SetPosition(80, 20);
	}
	scene.Add(gameObject);

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<TextComponent>("60", "Lingua.otf", 36);
	if (gameObject->ComponentAdded<TextComponent>())
	{
		gameObject->GetComponent<TextComponent>()->SetPosition(0, 0);
	}
	gameObject->AddComponent<FPSComponent>(gameObject);
	scene.Add(gameObject);
}

int main(int, char*[]) {
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
