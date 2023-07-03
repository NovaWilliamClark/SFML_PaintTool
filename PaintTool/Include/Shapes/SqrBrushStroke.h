/*********************************************************************

	@Filename: SqrBrushStroke.h
	@Author: William Clark
	@Date: 20th March 2023
	@Description:
	The SqrBrushStroke class is a custom shape class that is used for
	drawing square brushes that angle in the direction of the cursor movement.
	This allows for more natural and fluid brush strokes when painting.

*********************************************************************/

#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <cmath>

class SqrBrushStroke final : public sf::Shape
{
public:
	std::size_t getPointCount() const override
	{
		return 4;
	}

	sf::Vector2f getPoint(std::size_t t_index) const override
	{
		constexpr float PI = 3.141592654f;

		const sf::Vector2f point1(1.f, 0.f);
		const sf::Vector2f point2(getPosition() + sf::Vector2f(1.0, 0.0) - getPosition());
		sf::Vector2f drawOffset;
		int flipScalar;

		if (t_index < 2)
		{
			drawOffset = point2;
			flipScalar = 1;
		}
		else
		{
			drawOffset = point1;
			flipScalar = -1;
			t_index -= 2;
		}

		const float start = (-atan2(point1.y - point2.y, point2.x - point1.x));
		const float angle = static_cast<float>(t_index) * (PI / 2) - (PI / 4) + start;
		const float x = std::cos(angle) * m_size;
		const float y = std::sin(angle) * m_size;

		return { drawOffset.x + x * static_cast<float>(flipScalar), drawOffset.y + y * static_cast<float>(flipScalar) };
	}

	void setSize(const float t_size)
	{
		m_size = t_size;
		update();
	}

private:
	float m_size{};

};

