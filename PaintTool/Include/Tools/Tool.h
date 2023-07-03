/*********************************************************************

	@Filename: Tool.h
	@Author: William Clark
	@Date: 16th March 2023
	@Description:
	Tool, an abstract class for all other tools.
	Methods to handle its own mouse events and to set its own properties.
	Responsible for user input and updating the state of the Canvas accordingly.
	Draws an a line from 1 point to another

*********************************************************************/

#pragma once
#include <SFML/Graphics.hpp>

class Tool {
public:
	virtual ~Tool() = default;
	virtual void handleMousePressed(sf::Vector2f t_position) = 0;
	virtual void handleMouseReleased(sf::Vector2f t_position) = 0;
	virtual void handleKeyPressed() { }

	virtual void setSize(float t_size) = 0;
	virtual void setColor(sf::Color t_color) = 0;
	virtual void setTexture(sf::Texture* t_texture) = 0;

	virtual float* getSizeRef() = 0;

	void setIcon(const char* t_iconPath)
	{
		m_iconTexture.loadFromFile(t_iconPath);
		m_icon.setTexture(m_iconTexture);
	}
	sf::Sprite getIcon() { return m_icon; }

private:
	sf::Texture m_iconTexture;
	sf::Sprite m_icon;
};