#include "Tools/PolyonTool.h"

#include "Core/Canvas.h"

PolygonTool::PolygonTool(const char* t_iconPath)
{
	setIcon(t_iconPath);
}

void PolygonTool::handleMousePressed(const sf::Vector2f t_position)
{
	if (m_bisInitPoint)
	{
		m_polygon = new sf::ConvexShape();
		m_line = new sf::VertexArray(sf::LinesStrip);

		// Set Polygon and Line Color
		m_initialColor = m_color;
		m_polygon->setFillColor(m_color);

		// Append points to each array
		m_polygon->setPointCount(1);
		m_polygon->setPoint(0, t_position);

		m_line->append(sf::Vertex(t_position, m_color));

		/* Push back to canvas to be drawn, these pointers can still be modified here
		 * until the enter key is hit */
		Canvas::drawables.push_back(m_polygon);
		Canvas::drawables.push_back(m_line);
		m_bisInitPoint = false;
	}
	else if (m_polygon != nullptr)
	{
		// Resize Polygon
		m_polygon->setPointCount(m_polygon->getPointCount() + 1);
		m_polygon->setPoint(m_polygon->getPointCount() - 1, t_position);

		// Add new point to line
		m_line->append(sf::Vertex(t_position, m_initialColor));
	}
}

void PolygonTool::handleMouseReleased(sf::Vector2f t_position)
{
	// Nothing to do here - this tool doesn't use mouse release
}

void PolygonTool::handleKeyPressed()
{
	// Complete the polygon

	// Find the pointer to the m_line instance and delete it from Canvas. This is just a small optimization
	if (const auto it = std::ranges::find(Canvas::drawables, m_line); it != Canvas::drawables.end())
	{
		Canvas::drawables.erase(it);
	}

	// delete the pointer otherwise it will cause a memory leaks
	delete m_line;
	m_line = nullptr;

	m_bisInitPoint = true;
	m_polygon = nullptr;
}

void PolygonTool::setColor(const sf::Color t_color)
{
	m_color = t_color;
}

void PolygonTool::setSize(float t_size)
{
	// Nothing to do here - this tool doesn't use size;
}

void PolygonTool::setTexture(sf::Texture* t_texture)
{
	// Nothing to do here - this tool doesn't use a texture
}
