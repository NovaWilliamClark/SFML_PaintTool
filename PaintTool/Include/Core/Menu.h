/*********************************************************************

	@Filename: Menu.h
	@Author: William Clark
	@Date: 16th March 2023
	@Description:
	The Menu class is responsible for rendering the user interface using imgui.
	It handles input handling for the UI elements, such as buttons and sliders,
	and communicates with the other classes to modify the state of the application.

*********************************************************************/
#pragma once

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include "Canvas.h"
#include "Tools/Tool.h"

enum class FileHandlingFlag
{
	OPEN_FILE,
	SAVE_FILE
};

union GuiColorValues
{
	struct 
	{
		float r, g, b, a;
	};
	float values[4];
};

class Menu {
public:
	explicit Menu();
	~Menu();
	void update(sf::RenderWindow& t_window, sf::Clock& t_clock, Canvas& t_canvas);
	void draw(sf::RenderWindow& t_window, sf::Clock& t_clock, Canvas& t_canvas);
	void addTool(Tool* t_tool);

	void addStamp(sf::Texture* t_stampTexture);

	[[nodiscard]] Tool* getSelectedTool() const;
	[[nodiscard]] int getSelectedToolNum() const;

private:
	static sf::Texture* loadStamp();
	static std::string openFileExplorer(FileHandlingFlag t_flag);

	void drawToolbar(Canvas& t_canvas, const sf::RenderWindow& t_window);
	void drawToolSettings(const sf::RenderWindow& t_window);
	int m_selectedTool{};

	std::vector<Tool*> m_tools;
	std::vector<sf::Texture*> m_stamps;

	sf::Texture* m_selectedStamp;

	GuiColorValues m_guiColorValues {{0.f, 0.f, 0.f, 1.f}};
};

