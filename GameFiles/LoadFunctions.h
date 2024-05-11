#ifndef PM_GAMELOAD_H
#define PM_GAMELOAD_H

#include "Scene.h"
#include "SceneManager.h"

#include "Component.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "InputManager.h"

#include "Components/FPSComponent.h"
#include "Commands/PlaySoundCommand.h"
#include "Configuration.h"

namespace pacman
{

	void LoadGridLayout(pacman::Scene* scenePtr)
	{
		std::ifstream gridLayoutFile("Resources/Files/GridLayout.csv");

		if (not gridLayoutFile.is_open())
		{
			throw std::runtime_error("failed to locate pickup positions");
		}
		std::regex validLineExpression("(\\d+),(\\d+),(\\d+),(\\w+),(\\w+)");
		//do not use until after while loop
		std::smatch matches{};
		std::string line{};

		while (std::getline(gridLayoutFile, line))
		{
			if (std::regex_match(line, matches, validLineExpression))
			{
				//ignoring first match since its entire string in the capture group always
				//ignoring second match since its the index to generate col and row idx
				int row{ std::stoi(matches[2].str()) };
				int col{ std::stoi(matches[3].str()) };

				if (matches[4] == "pathway")
				{
					if (matches[5] == "small")
					{
						std::unique_ptr pickupSmallUPtr{ std::make_unique<pacman::GameObject>() };
						pickupSmallUPtr->AddComponent<pacman::TransformComponent>(pickupSmallUPtr.get(), glm::vec2{ col * pacman::config::CELL_WIDTH, row * pacman::config::CELL_HEIGHT });
						pickupSmallUPtr->AddComponent<pacman::RenderComponent>(pickupSmallUPtr.get(), "Sprites/EatableSmall.png");
						scenePtr->Add(std::move(pickupSmallUPtr));
					}
					if (matches[5] == "big")
					{
						std::unique_ptr pickupBigUPtr{ std::make_unique<pacman::GameObject>() };
						pickupBigUPtr->AddComponent<pacman::TransformComponent>(pickupBigUPtr.get(), glm::vec2{ col * pacman::config::CELL_WIDTH, row * pacman::config::CELL_HEIGHT });
						pickupBigUPtr->AddComponent<pacman::RenderComponent>(pickupBigUPtr.get(), "Sprites/EatableBig.png");
						scenePtr->Add(std::move(pickupBigUPtr));
					}
				}
			}
		}
	}

	void LoadGame(pacman::Scene* scenePtr)
	{
		using SoundId = pacman::sound::SoundId;
		using InpMan = pacman::InputManager;

		auto& inputManager = InpMan::GetInstance();

		std::unique_ptr backgroundUPtr{ std::make_unique<pacman::GameObject>() };
		backgroundUPtr->AddComponent<pacman::TransformComponent>(backgroundUPtr.get(), glm::vec2{ 0, 0 });
		backgroundUPtr->AddComponent<pacman::RenderComponent>(backgroundUPtr.get(), "Sprites/PlayingField.png");

		std::unique_ptr fpsCounterUPtr{ std::make_unique<pacman::GameObject>() };
		fpsCounterUPtr->AddComponent<pacman::TransformComponent>(fpsCounterUPtr.get(), glm::vec2{ 0, 50 });
		fpsCounterUPtr->AddComponent<pacman::TextComponent>(fpsCounterUPtr.get(), "60", "Fonts/Lingua.otf", 36);
		fpsCounterUPtr->AddComponent<pacman::FPSComponent>(fpsCounterUPtr.get());

		scenePtr->Add(std::move(backgroundUPtr));

		LoadGridLayout(scenePtr);

		scenePtr->Add(std::move(fpsCounterUPtr));

		std::unique_ptr playChompCommand{ std::make_unique<pacman::PlayPacmanChomp>() };

		inputManager.AddCommandKeyboard(InpMan::Key::W, InpMan::InputState::Pressed, std::move(playChompCommand));

		std::unique_ptr playDeathCommand{ std::make_unique<pacman::PlayPacmanDeath>() };

		inputManager.AddCommandKeyboard(InpMan::Key::A, InpMan::InputState::Pressed, std::move(playDeathCommand));
	}

}

#endif