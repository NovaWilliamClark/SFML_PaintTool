/*********************************************************************

	@Filename: RectangleTool.h
	@Author: William Clark
	@Date: 19th March 2023
	@Description:
	RectangleTool, a concrete subclass of the abstract Tool class.
	Methods to handle its own mouse events and to set its own properties.
	Responsible for user input and updating the state of the Canvas accordingly.
	Draws an a rectangle from 1 point to another

*********************************************************************/

#pragma once
#include "Tool.h"

class RectangleTool final : public Tool
{
public:
	explicit RectangleTool(const char* t_iconPath);
	void handleMousePressed(sf::Vector2f t_position) override;
	void handleMouseReleased(sf::Vector2f t_position) override;
	void setColor(sf::Color t_color) override;
	void setSize(float t_size) override;
	void setTexture(sf::Texture* t_texture) override;

	float* getSizeRef() override { return &m_size; }

private:
	sf::RectangleShape* m_rectangle{};
	sf::Sprite m_sprite;
	sf::Color m_color;
	float m_size = 10;
	bool m_bisInitPoint = true;
};

