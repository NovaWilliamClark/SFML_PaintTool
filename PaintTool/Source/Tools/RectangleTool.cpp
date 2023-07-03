#include "Tools/RectangleTool.h"
#include "Core/Canvas.h"

// Constructor for RectangleTool, takes the path of an icon image as a parameter
RectangleTool::RectangleTool(const char* t_iconPath)
{
	setIcon(t_iconPath);
}

// Handles the event where the mouse is pressed while using the tool
void RectangleTool::handleMousePressed(const sf::Vector2f t_position)
{
	if (m_bisInitPoint)
	{
		 // Create a new rectangle shape object with set position, fill color and size of (0, 0)
		m_rectangle = new sf::RectangleShape(sf::Vector2f(0, 0));
		m_rectangle->setPosition(t_position);
		m_rectangle->setFillColor(m_color);

		// Push back the newly created rectangle to the list of drawables in Canvas 
        // so that it can be drawn until the mouse is released
		Canvas::drawables.push_back(m_rectangle);

		// Change flag so that next time the function updates the size of the rectangle marking the end point
		m_bisInitPoint = false;
	}
	else if (m_rectangle != nullptr) // if rectangle wasn't null when drawing is started, continue to update its dimensions
	{
		const sf::Vector2f startPoint = m_rectangle->getPosition();
		// Update the size of the rectangle in relation to the new mouse position
		m_rectangle->setSize(sf::Vector2f(t_position.x - startPoint.x, t_position.y - startPoint.y));
	}
}

// Handles the event where the user releases the left mouse button
void RectangleTool::handleMouseReleased(sf::Vector2f t_position)
{
	// Reset flag because now we need to start from scratch next time mouse is pressed
	m_bisInitPoint = true;

	// Clean up rectangle variable and prep for next item
	m_rectangle = nullptr;
}

// Setter method for setting the color value that will be used to draw shapes
void RectangleTool::setColor(const sf::Color t_color)
{
	m_color = t_color;
}

// Setter method for setting the dimensions of the brush being currently used
void RectangleTool::setSize(const float t_size)
{
	m_size = t_size;
}

// Setter method for loading the texture into the toolbox if there is one
void RectangleTool::setTexture(sf::Texture* t_texture)
{
	// Nothing to do here - this tool doesn't use a texture
}
