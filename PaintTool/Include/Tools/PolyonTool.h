/*********************************************************************

	@Filename: PolygonTool.h
	@Author: William Clark
	@Date: 20th March 2023
	@Description:
	PolygonTool, a concrete subclass of the abstract Tool class.
	Methods to handle its own mouse events and to set its own properties.
	Responsible for user input and updating the state of the Canvas accordingly.
	Draws a convex shape from 3 or more points

*********************************************************************/

#pragma once
#include "Tool.h"

class PolygonTool final : public Tool {
public:
	explicit PolygonTool(const char* t_iconPath);

	void handleMousePressed(sf::Vector2f t_position) override;
	void handleMouseReleased(sf::Vector2f t_position) override;
	void handleKeyPressed() override;
	void setColor(sf::Color t_color) override;
	void setSize(float t_size) override;
	void setTexture(sf::Texture* t_texture) override;

	float* getSizeRef() override { return &m_size; }
private:
	sf::ConvexShape* m_polygon{};
	sf::VertexArray* m_line{};
	sf::Color m_color;
	sf::Color m_initialColor;
	float m_size = 0;
	bool m_bisInitPoint = true;
};

