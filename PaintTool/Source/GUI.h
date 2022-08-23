#pragma once
#include <SFML/Graphics/Texture.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "Application.h"
#include "Brushes/Brush.h"
class GUI
{
public:
	GUI();
	~GUI();
private:
	// Icons to Load
	struct Icons
	{
		sf::Texture BrushTool;
		sf::Texture LineTool;
		sf::Texture RectangleTool;
		sf::Texture EllipseTool;
		sf::Texture Polygon;
		sf::Texture Stamp;

		sf::Texture CircleBrushShape;
		sf::Texture SquareBrushShape;
	};
	Icons m_ButtonIcns;
	bool m_bShouldDisplayDebug;

	// An array containing 3 floats to make up the color (RGB 0 - 255)
	float* m_ColorVector{};
	StrokeType m_BrushStroke;
	static const char* getStateStr();
public:

	void UpdateGUI(const sf::Vector2i _MousePosition);
	float* getColorVector() const
	{
		return m_ColorVector;
	};
};

