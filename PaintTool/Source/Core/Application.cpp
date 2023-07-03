#include "Core/Application.h"

#include "Tools/BrushTool.h"
#include "Tools/EllipseTool.h"
#include "Tools/LineTool.h"
#include "Tools/PolyonTool.h"
#include "Tools/RectangleTool.h"
#include "Tools/StampTool.h"

Application::Application() : m_canvasWindow(sf::VideoMode(1280, 720), "Paint Tool"), m_canvas(Canvas())
{
	// Get system window handle and store it
	systemWindowHandle = m_canvasWindow.getSystemHandle();

	// Create a menu for our application
	m_menu = Menu();

	// Add different tools that can be used on the canvas to the menu
	m_menu.addTool(new BrushTool("Assets/Icons/BrushTool.png"));
	m_menu.addTool(new LineTool("Assets/Icons/LineTool.png"));
	m_menu.addTool(new RectangleTool("Assets/Icons/RectangleTool.png"));
	m_menu.addTool(new EllipseTool("Assets/Icons/EllipseTool.png"));
	m_menu.addTool(new PolygonTool("Assets/Icons/PolygonTool.png"));

	// Set a base image for the stamp tool and add it to the menu
	const auto baseStamp = new sf::Texture;
	baseStamp->loadFromFile("Assets/TestStamps/WClarkArtSignage.png");
	m_menu.addTool(new StampTool(baseStamp, "Assets/Icons/StampTool.png"));
	m_menu.addStamp(baseStamp);

	ImGui::SFML::Init(m_canvasWindow);
}

// method to run the application
void Application::run() {

	// Init a clock to track the delta time
	sf::Clock deltaClock;

	// Main Loop
	while (m_canvasWindow.isOpen()) {

		// Handle input events in our application
		sf::Event inputEvent{};
		while (m_canvasWindow.pollEvent(inputEvent)) {

			// Handles any input events performed on the ImGui widgets
			ImGui::SFML::ProcessEvent(inputEvent);

			// Close button event
			if (inputEvent.type == sf::Event::Closed) {
				m_canvasWindow.close();
			}

			// Close button event
			if (inputEvent.type == sf::Event::Resized)
			{
				// Nothing to do here if the window is resized
			}

			// Check if window is focused and mouse event is not captured by ImGui
			// if so, handle tool input or events
			if (m_canvasWindow.hasFocus() && !ImGui::GetIO().WantCaptureMouse == true)
			{
				handleToolInput(inputEvent);
			}
		}

		// Clear Drawing Canvas and window
		m_canvasWindow.clear(sf::Color::White);
		m_canvas.clear();


		// Draw all drawable objects & UI
		m_canvas.draw(m_canvasWindow);
		m_menu.draw(m_canvasWindow, deltaClock, m_canvas);

		// Display newly drawn Canvas and UI to window
		m_canvasWindow.display();
	}
}

// method to handle tool input events
void Application::handleToolInput(sf::Event& t_inputEvent) const
{
	// Update mouse position upon the start of the next frame
	const auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_canvasWindow));

	// convert it to world coordinates
	const sf::Vector2f mouseWorldPos = m_canvasWindow.mapPixelToCoords(static_cast<sf::Vector2i>(mousePosition));

	if (Tool* currentTool = m_menu.getSelectedTool(); currentTool != nullptr)
	{
		/* Cast to both StampTool or Polygon tool to check if mouse events should be single event fire (Click)
		or continuous events (Hold)*/
		if (dynamic_cast<PolygonTool*>(currentTool) || dynamic_cast<StampTool*>(currentTool))
		{
			if (t_inputEvent.type == sf::Event::MouseButtonPressed && t_inputEvent.mouseButton.button == sf::Mouse::Left)
			{
				currentTool->handleMousePressed(mouseWorldPos);
			}

			if (const auto stampTool = dynamic_cast<StampTool*>(currentTool))
			{
				stampTool->updatePreview(mouseWorldPos);
			}
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			/* sf::Mouse::isButtonPressed(sf::Mouse::Left) will constantly fire and is used for
			 * handling events for tools such as Paint Brush, Line & primitive shape drawing */
			currentTool->handleMousePressed(mouseWorldPos);
		}
		if (t_inputEvent.type == sf::Event::MouseButtonReleased)
		{
			currentTool->handleMouseReleased(mouseWorldPos);
		}

		// Handle Keyboard input events, this is only used to complete the instantiation of the polygon
		if (t_inputEvent.type == t_inputEvent.KeyPressed && t_inputEvent.key.code == sf::Keyboard::Enter)
		{
			currentTool->handleKeyPressed();
		}
	}
}

