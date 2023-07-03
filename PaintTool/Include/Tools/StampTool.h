/*********************************************************************

	@Filename: StampTool.h
	@Author: William Clark
	@Date: 19th March 2023
	@Description:
	Bitmap, a concrete subclasses of the abstract Tool class.
	Methods to handle its own mouse events and to set its own properties.
	Responsible for user input and updating the state of the Canvas accordingly.
	Draws a bitmap image stamp selected from the toolbar

*********************************************************************/

#pragma once
#include "Tool.h"

class StampTool final : public Tool {
public:
	explicit StampTool(const sf::Texture* t_texture, const char* t_iconPath);
	void handleMousePressed(sf::Vector2f t_position) override;
	void handleMouseReleased(sf::Vector2f t_position) override;

	void setVisibility(bool t_bIsVisible);
	void updatePreview(sf::Vector2f t_position) const;

	void setColor(sf::Color t_color) override;
	void setSize(float t_size) override;
	void setTexture(sf::Texture* t_texture) override;

	float* getSizeRef() override;

private:
	sf::Sprite* m_previewSprite;
	sf::Color m_color;
	sf::Color m_outlineColor;
	float m_size = 1;
	bool m_bIsPreviewVisible = false;
};


