/*********************************************************************

	@Filename: Canvas.h
	@Author: William Clark
	@Date: 16th March 2023
	@Description:
	The Canvas class represents the drawing area where the user can draw,
	and it stores all the drawable objects in the scene.
	It handles the rendering of the drawing area and all the drawables within it.
	
*********************************************************************/
#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Tools/Tool.h"

class Canvas {
public:
	Canvas(); // Constructor
	~Canvas(); // Destructor

	void draw(sf::RenderWindow& t_window);

	void clear();
	void saveToFile(const std::string& t_filename) const;
	void loadFromFile(const std::string& t_filename);

	inline static std::vector<sf::Drawable*> drawables;
private:
	sf::RenderTexture m_renderTexture;
	sf::Texture m_bgTexture;
	sf::Sprite m_bgSprite;
};