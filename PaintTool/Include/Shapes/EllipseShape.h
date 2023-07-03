/*********************************************************************

	@Filename: EllipseShape.h
	@Author: William Clark
	@Date: 19th March 2023
	@Description:
	The EllipseShape class is a custom shape class that allows the user
	to draw ellipses using its own algorithm.

*********************************************************************/

#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <cmath>

class EllipseShape final : public sf::Shape
{
public:

	explicit EllipseShape(const sf::Vector2f& t_radius = sf::Vector2f(0.f, 0.f)) :
		m_radius(t_radius)
	{
		update();
	}

	void setRadius(const sf::Vector2f& t_radius)
	{
		m_radius = t_radius;
		update();
	}

	const sf::Vector2f& getRadius() const
	{
		return m_radius;
	}

	std::size_t getPointCount() const override
	{
		return 30; // fixed, but could be an attribute of the class if needed
	}

	sf::Vector2f getPoint(const std::size_t t_index) const override
	{
		constexpr float PI = 3.141592654f;

		const float angle = static_cast<float>(t_index) * 2.f * PI / static_cast<float>(getPointCount()) - PI / 2.f;
		const float x = std::cos(angle) * m_radius.x;
		const float y = std::sin(angle) * m_radius.y;

		return {x, y};
	}

private:

	sf::Vector2f m_radius;
};
