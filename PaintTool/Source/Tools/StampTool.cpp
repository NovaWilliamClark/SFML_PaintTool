#include "Tools/StampTool.h"
#include "Core/Canvas.h"

// StampTool constructor taking a texture pointer and icon file path as arguments
StampTool::StampTool(const sf::Texture *t_texture, const char *t_iconPath)
{
	// Set the icon path
	setIcon(t_iconPath);

	// Instantiate a preview sprite and set its origin
	m_previewSprite = new sf::Sprite();
	m_previewSprite->setTexture(*t_texture, true);
	m_previewSprite->setOrigin(t_texture->getSize().x / 2, t_texture->getSize().y / 2);
}

// Event handling function for when the user presses the mouse button
void StampTool::handleMousePressed(const sf::Vector2f t_position)
{
	// Create a new stamp sprite at the given position with size and color attributes of the tool
	const auto stamp = new sf::Sprite();
	stamp->setTexture(*m_previewSprite->getTexture(), true);
	stamp->setScale(m_size, m_size);
	stamp->setOrigin(m_previewSprite->getTexture()->getSize().x / 2, m_previewSprite->getTexture()->getSize().y / 2);
	stamp->setPosition(t_position);
	stamp->setColor(m_color);

	// Add the stamp to the list of drawables in Canvas
	Canvas::drawables.push_back(stamp);
}

// Event handling function for when the user releases the mouse button
void StampTool::handleMouseReleased(sf::Vector2f t_position)
{
	// No action necessary here
}

// Function to toggle the visibility of the preview sprite
void StampTool::setVisibility(const bool t_bIsVisible)
{
	m_bIsPreviewVisible = t_bIsVisible;
	if (m_bIsPreviewVisible == true)
	{
		// If visible, add the preview sprite to the list of drawables in Canvas
		Canvas::drawables.push_back(m_previewSprite);
	}
	else
	{
		// If not visible, remove the preview sprite from the list of drawables in Canvas
		if (const auto it = std::ranges::find(Canvas::drawables, m_previewSprite); it != Canvas::drawables.end())
		{
			Canvas::drawables.erase(it);
		}
	}
}

// Function to update the position, size and color of the preview sprite based on user input (e.g. moving the cursor)
void StampTool::updatePreview(const sf::Vector2f t_position) const
{
	m_previewSprite->setPosition(t_position);
	m_previewSprite->setScale(m_size, m_size);
	m_previewSprite->setColor(m_color);
	m_previewSprite->setOrigin(m_previewSprite->getTexture()->getSize().x / 2, m_previewSprite->getTexture()->getSize().y / 2);
}

// Setter functions for the color, size, and texture of the tool
void StampTool::setColor(const sf::Color t_color)
{
	m_color = t_color;
}

void StampTool::setSize(const float t_size)
{
	m_size = t_size;
}

void StampTool::setTexture(sf::Texture *t_texture)
{
	m_previewSprite->setTexture(*t_texture, true);
}

// Function to get a reference to the size attribute of the tool
float *StampTool::getSizeRef()
{
	return &m_size;
}
