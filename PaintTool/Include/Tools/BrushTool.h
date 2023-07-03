/*********************************************************************

	@Filename: BrushTool.h
	@Author: William Clark
	@Date: 17th March 2023
	@Description:
	BrushTool, a concrete subclass of the abstract Tool class.
	Methods to handle its own mouse events and to set its own properties.
	Responsible for user input and updating the state of the Canvas accordingly.
	Draws either a round or square brush shape.

*********************************************************************/

#pragma once
#include "Tool.h"

enum class BrushShape
{
	ROUND,
	SQUARE,
};

class BrushTool final : public Tool {
public:
	explicit BrushTool(const char* t_iconPath);

	void handleMousePressed(sf::Vector2f t_position) override;
	void handleMouseReleased(sf::Vector2f t_position) override;
	void setColor(sf::Color t_color) override;
	void setSize(float t_size) override;
	void setTexture(sf::Texture* t_texture) override;

	void setBrushShape(BrushShape t_brushShape);

	float* getSizeRef() override { return &m_size; }

private:
	BrushShape m_brushShape = BrushShape::ROUND;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_sprite;
	sf::Color m_color;
	float m_size = 10;
};

