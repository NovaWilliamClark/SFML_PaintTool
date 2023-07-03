/*********************************************************************

	@File: Application.h
	@Author: William Clark
	@Date: 16th March 2023
	@Description:
	The Application class is responsible for running the main
	application loop and handling user input. It initializes the window,
	manages the event queue, and updates the state of the application accordingly.
    
*********************************************************************/
#pragma once

#include <SFML/Graphics.hpp>
#include "Canvas.h"
#include "Menu.h"

class Application {
public:
	Application(); // Constructor
	void run(); // Runs main application loop
	void handleToolInput(sf::Event& t_inputEvent) const;

	// stores the system window handle for use in the file explorer
	inline static sf::WindowHandle systemWindowHandle;

private:
	sf::RenderWindow m_canvasWindow;	// main window of the application
	Canvas m_canvas;					// canvas object
	Menu m_menu;						// menu object used to display the user interface

};
