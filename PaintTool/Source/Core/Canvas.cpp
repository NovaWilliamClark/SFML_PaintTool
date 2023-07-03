#include "Core/Canvas.h"

Canvas::Canvas()
= default;

Canvas::~Canvas()
= default;

/**
 *@brief update & draw all drawable objects to the window
 */
void Canvas::draw(sf::RenderWindow& t_window)
{

	// Update the render texture with any changes made to the drawables
	m_renderTexture.display();

	// Draw the background sprite to the window
	m_renderTexture.draw(m_bgSprite);
	t_window.draw(m_bgSprite);

	// Draw all the drawables on top of the background sprite
	for (const auto& drawable : drawables)
	{
		m_renderTexture.draw(*drawable);
		t_window.draw(*drawable);
	}
}

/**
 *@brief  Clears the render texture of all drawables
 */
void Canvas::clear()
{
	// Clear the render texture of all drawables and set it to white
	m_renderTexture.clear(sf::Color::White);
}

/**
 *@brief  Saves the contents of the render texture to an image file with the given filename
 */
void Canvas::saveToFile(const std::string& t_filename) const
{
	m_renderTexture.getTexture().copyToImage().saveToFile(t_filename);
}

/**
 *@brief  Loads an image file with the given filename and set it as the background sprite for the canvas
 */
void Canvas::loadFromFile(const std::string& t_filename)
{
	// If the image file was successfully loaded
	if (m_bgTexture.loadFromFile(t_filename)) {
		// Create a new render texture with the same size as the window
		m_renderTexture.create(1280, 720);

		// Set the background sprite to the loaded texture, and scale it to fit the window
		m_bgSprite.setTexture(m_bgTexture);
		m_bgSprite.setTextureRect(sf::IntRect(0, 0, m_bgTexture.getSize().x, m_bgTexture.getSize().y));
		m_bgSprite.setPosition(0, 0);
		m_bgSprite.setScale(1280 / m_bgSprite.getLocalBounds().width, 720 / m_bgSprite.getLocalBounds().height);
		m_bgSprite.setOrigin(0, 0);

		// Draw the background sprite to the render texture
		m_renderTexture.draw(m_bgSprite);

		// Delete all existing drawables and clear the list
		for (const auto element : drawables)
		{
			delete element;
		}
		drawables.clear();
	}
}
