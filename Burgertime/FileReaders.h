#include <string>
#include "GameObject.h"
#include "Scene.h"

namespace dae
{
	class LevelReader
	{
	public:
		static std::unique_ptr<GameObject> ReadLevelFile(
			const std::string& filePath,
			Scene& scene,
			float scale);
	};
}