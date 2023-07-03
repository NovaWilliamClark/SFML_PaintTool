#include "Tools/BrushTool.h"
#include "Core/Canvas.h"
#include "Shapes/SqrBrushStroke.h"

BrushTool::BrushTool(const char* t_iconPath)
{
	setIcon(t_iconPath);
}

void BrushTool::handleMousePressed(const sf::Vector2f t_position)
{

	if (m_brushShape == BrushShape::ROUND)
	{
		const auto brush = new sf::CircleShape;
		// Set size, position, origin and origin
		brush->setFillColor(m_color);
		brush->setPosition(t_position);
		brush->setOrigin(m_size, m_size);
		brush->setRadius(m_size);
		Canvas::drawables.push_back(brush);
	}

	if (m_brushShape == BrushShape::SQUARE)
	{
		const auto brush = new SqrBrushStroke();
		// Set size, position, origin and origin
		brush->setFillColor(m_color);
		brush->setPosition(t_position);
		brush->setOrigin(m_size, m_size);
		brush->setSize(m_size);
		Canvas::drawables.push_back(brush);
	}
}

void BrushTool::handleMouseReleased(sf::Vector2f t_position)
{
	// Nothing to do here - no event required on input handleMouseReleased
}

void BrushTool::setColor(const sf::Color t_color)
{
	m_color = t_color;
}

void BrushTool::setSize(const float t_size)
{
	m_size = t_size;
}

void BrushTool::setTexture(sf::Texture* t_texture)
{
	// Nothing to do here - this tool doesn't use a texture
}

void BrushTool::setBrushShape(const BrushShape t_brushShape)
{
	m_brushShape = t_brushShape;
}
