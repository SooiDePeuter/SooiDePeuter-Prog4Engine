#include "FileReaders.h"
#include "HitboxComponent.h"
#include <fstream>
#include <iostream>

std::unique_ptr<dae::GameObject> dae::LevelReader::ReadLevelFile(const std::string& filePath, dae::Scene& scene, float scale)
{
	std::ifstream levelFile{filePath};

	std::unique_ptr<GameObject> level = std::make_unique<GameObject>();
	level->SetLocalPosition(200, 100);
	
	//level is 19 by 19

	for (int yIndex{}; yIndex < 19; yIndex++)
	{
		for (int xIndex{}; xIndex < 20; xIndex++) //20th for endline
		{
			int letter{ levelFile.get() };

			//_
			if (letter == 95)
			{
				continue;
			}

			auto levelPart = std::make_unique<dae::GameObject>();
			levelPart->SetParent(level.get());
			levelPart->SetLocalPosition(xIndex * scale, yIndex * scale);
			levelPart->SetTextureDimensions(scale, scale);

			std::string levelPartTask1{"LadderBox"};
			std::string levelPartTask2{"PlatformBox"};

			switch (letter)
			{
			//|
			case 124:
				levelPart->SetTexture("Ladder.png");
				levelPart->AddComponent<dae::HitboxComponent>(0.375f * scale, 0.f, 0.250f * scale, scale, &levelPartTask1);
				break;
			//B
			case 66:
				levelPart->SetTexture("Ladder_Blue.png");
				levelPart->AddComponent<dae::HitboxComponent>(0.375f * scale, 0.f, 0.250f * scale, scale, &levelPartTask1);
				levelPart->AddComponent<dae::HitboxComponent>(0.f, 0.750f * scale, scale, 0.250f * scale, &levelPartTask2);
				break;
			//C
			case 67:
				levelPart->SetTexture("Ladder_Cyan.png");
				levelPart->AddComponent<dae::HitboxComponent>(0.375f * scale, 0.f, 0.250f * scale, scale, &levelPartTask1);
				levelPart->AddComponent<dae::HitboxComponent>(0.f, 0.750f * scale, scale, 0.250f * scale, &levelPartTask2);
				break;
			//L
			case 76:
				levelPart->SetTexture("Platform_Cyan_Left.png");
				break;
			//R
			case 82:
				levelPart->SetTexture("Platform_Cyan_Right.png");
				break;
			//b
			case 98:
				levelPart->SetTexture("Platform_Blue.png");
				break;
			//c
			case 99:
				levelPart->SetTexture("Platform_Cyan.png");
				break;
			//l
			case 108:
				levelPart->SetTexture("Platform_Left.png");
				break;
			//r
			case 114:
				levelPart->SetTexture("Platform_Right.png");
				break;
			default:
				std::cout << "char was not recognised:" << letter << "\n";
				break;
			}

			scene.Add(levelPart);
		}
	}

	return level;
}
