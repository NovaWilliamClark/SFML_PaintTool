#include "Tools/LineTool.h"

#include "Core/Canvas.h"

LineTool::LineTool(const char* t_iconPath)
{
	setIcon(t_iconPath);

}

void LineTool::handleMousePressed(const sf::Vector2f t_position)
{
	if (m_bisInitPoint)
	{
		m_line = new sf::RectangleShape(sf::Vector2f(1, m_size));
		m_line->setPosition(t_position);
		m_line->setFillColor(m_color);
		m_line->setOrigin(0.f, m_line->getSize().y / 2.f);

		// Push back to be drawn. The pointer can still update until the mouse is released
		Canvas::drawables.push_back(m_line);
		m_bisInitPoint = false;
	}
	else if (m_line != nullptr)
	{
		const sf::Vector2f point1 = m_line->getPosition();
		const sf::Vector2f point2 = t_position;

		/* Use Trigonometry to calculate the length (hypotenuse) and
		/ angle between the two points */
		constexpr float PI = 3.141592654f;
		const auto opposite = (point2.y - point1.y);
		const auto adjacent = (point2.x - point1.x);
		const float hypotenuse = sqrtf(powf(opposite, 2) + powf(adjacent, 2));
		const float angle = (-atan2f(adjacent, opposite) + 1.57f) * (180.f / PI);

		// Update rectangle length and angle towards new mouse position
		m_line->setRotation(angle);

		// Update the length
		m_line->setSize(sf::Vector2f(hypotenuse, m_size));
	}
}

void LineTool::handleMouseReleased(sf::Vector2f t_position)
{
	m_bisInitPoint = true;
	m_line = nullptr;
}

void LineTool::setColor(const sf::Color t_color)
{
	m_color = t_color;
}

void LineTool::setSize(const float t_size)
{
	m_size = t_size;
}

void LineTool::setTexture(sf::Texture* t_texture)
{
	// Nothing to do here - this tool doesn't use a texture
}


