#include "FileReaders.h"
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
			levelPart.get()->SetParent(level.get());
			levelPart.get()->SetLocalPosition(xIndex * scale, yIndex * scale);
			levelPart.get()->SetTextureDimensions(scale, scale);

			switch (letter)
			{
			//|
			case 124:
				levelPart->SetTexture("Ladder.png");
				break;
			//B
			case 66:
				levelPart->SetTexture("Ladder_Blue.png");
				break;
			//C
			case 67:
				levelPart->SetTexture("Ladder_Cyan.png");
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
