#include "Tools/EllipseTool.h"

#include "Core/Canvas.h"

EllipseTool::EllipseTool(const char* t_iconPath)
{
	setIcon(t_iconPath);
}

void EllipseTool::handleMousePressed(const sf::Vector2f t_position)
{
	if(m_bisInitPoint)
	{
		m_ellipse = new EllipseShape(sf::Vector2f(0, 0));
		m_ellipse->setPosition(t_position);
		m_ellipse->setFillColor(m_color);

		// Push back to be canvas to be drawn. The pointer can still update until the mouse is released
		Canvas::drawables.push_back(m_ellipse);
		m_bisInitPoint = false;
	}
	else if (m_ellipse != nullptr)
	{
		const sf::Vector2f startPoint = m_ellipse->getPosition();
		m_ellipse->setRadius(sf::Vector2f(t_position.x - startPoint.x, t_position.y - startPoint.y));
	}
}

void EllipseTool::handleMouseReleased(sf::Vector2f t_position)
{
	m_bisInitPoint = true;
	m_ellipse = nullptr;
}

void EllipseTool::setColor(const sf::Color t_color)
{
	m_color = t_color;
}

void EllipseTool::setSize(const float t_size)
{
	m_size = t_size;
}

void EllipseTool::setTexture(sf::Texture* t_texture)
{
	// Nothing to do here - this tool doesn't use a texture
}

