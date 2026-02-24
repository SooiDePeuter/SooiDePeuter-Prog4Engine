#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class TransformComponent final
	{
	public:
		const glm::vec3& GetPosition() const;

		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);
		void SetPosition(float x, float y);
	private:
		glm::vec3 m_position;
	};
}
