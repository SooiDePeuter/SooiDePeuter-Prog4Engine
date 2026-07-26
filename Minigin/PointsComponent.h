#pragma once
#include "BaseComponent.h"

namespace dae
{
	class PointsComponent final : public BaseComponent
	{
	public:

		PointsComponent(GameObject* owner, int points = 0, std::string* task = nullptr);

		void Update(float deltaTime) override;
		void Render() const override;

		int GetPoints();
		void SetPoints(int newPoints);

	private:
		int m_Points;
	};
}