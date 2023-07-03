 #include "Core/Menu.h"
#include "Tools/StampTool.h"
#include <Windows.h>

#include "Core/Application.h"
#include "Tools/BrushTool.h"

/**
 *@brief Init the class and set the active tool and stamp to null
 */
Menu::Menu(): m_selectedStamp(nullptr)
{
	m_selectedTool = -1; // Initialized selected tool to -1 (no tool selected)

	// Hides console window 
	const HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
}

/**
 *@brief Delete any heap allocations made by this class upon destruction
 */
Menu::~Menu()
{
	// Delete tool pointers
	for (const auto tool : m_tools)
	{
		delete tool;
	}

	// Delete stamp textures
	for (const auto stamp : m_stamps)
	{
		delete stamp;
	}
}

/**
 *@brief Updates the UI and draws the UI to the winodow
 */
void Menu::update(sf::RenderWindow& t_window, sf::Clock& t_clock, Canvas& t_canvas)
{
	ImGui::SFML::Update(t_window, t_clock.restart()); // Update imgui UI with window and delta time
	drawToolbar(t_canvas, t_window); // Draw the toolbar using the canvas and window
	ImGui::SFML::Render(t_window); // Render the UI to the window
}

/**
 *@brief Calls the update function to update the UI and draw the UI to the window
 */
void Menu::draw(sf::RenderWindow& t_window, sf::Clock& t_clock, Canvas& t_canvas)
{
	update(t_window, t_clock, t_canvas); // call Update to draw & update the menu
}

/**
 *@brief Add a new tool to the vector of tools
 */
void Menu::addTool(Tool* t_tool)
{
	m_tools.push_back(t_tool); // Add a new tool to the vector of tools
}

/**
 *@brief after successfully loading a stamp texture add the texture to the vector of stamps
 */
void Menu::addStamp(sf::Texture* t_stampTexture)
{
	if (t_stampTexture != nullptr) // Ensure the texture is not null
	{
		m_stamps.push_back(t_stampTexture); // Add the texture to the vector of stamps textures
	}
}

/**
 *@brief Open a folder explorer to load a stamp texture, this stamp can then be used by the stamp tool
 *@return Returns the a pointer of the new stamp texture that will be used by the stamp tool.
 */
sf::Texture* Menu::loadStamp()
{
	if (const std::string filepath = openFileExplorer(FileHandlingFlag::OPEN_FILE); !filepath.empty()) // Open the file explorer to load a stamp texture
	{
		auto* newStamp = new sf::Texture(); // Create a new texture pointer
		newStamp->loadFromFile(filepath); // Load the texture from file

		return newStamp; // Return the new texture pointer
	}
	return nullptr; // If no file was selected, return null
}


/**
 * A function to open the file explorer dialog box for either opening or saving a file
 * @param t_flag A flag that indicates whether the dialog box should open to open or save a file
 * @return A string containing the path of the selected file or an empty string if no file was selected
 */
std::string Menu::openFileExplorer(const FileHandlingFlag t_flag)
{
	// Check if the flag is for opening a file
	if (t_flag == FileHandlingFlag::OPEN_FILE)
	{
		// Initialize the OPENFILENAME struct and the buffer for the selected file path
		OPENFILENAME ofn = { 0 };
		wchar_t szFile[260] = { 0 };

		// Set the properties of the dialog box
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = Application::systemWindowHandle;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"png(.png)\0*.png\0bmp(.bmp)\0*.bmp\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// Show the file explorer dialog box and get the selected file path
		if (GetOpenFileNameW(&ofn))
		{
			// Convert the selected file path from wide string to string and return it
			std::wstring fileName = ofn.lpstrFile;
			auto rString = std::string(fileName.begin(), fileName.end());

			return rString;
		}
	}

	// Check if the flag is for saving a file
	if (t_flag == FileHandlingFlag::SAVE_FILE)
	{
		// Initialize the OPENFILENAME struct and the buffer for the selected file path
		OPENFILENAME ofn = { 0 };
		wchar_t szFile[260] = L"Untitled.png";

		// Set the properties of the dialog box
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = Application::systemWindowHandle;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"png(.png)\0*.png\0bmp(.bmp)\0*.bmp\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// Show the file explorer dialog box and get the selected file path
		if (GetSaveFileNameW(&ofn))
		{
			// Convert the selected file path from wide string to string and return it
			std::wstring fileName = ofn.lpstrFile;
			auto rString = std::string(fileName.begin(), fileName.end());

			return rString;
		}
	}
	// Return empty string if no file was selected
	return "";
}

/**
 * @brief Draws the main menu toolbar with options for file handling and selecting a tool.
 * @param t_canvas A reference to the Canvas object to handle saving and loading files.
 * @param t_window A const reference to the RenderWindow object for drawing the tool settings.
 */
void Menu::drawToolbar(Canvas& t_canvas, const sf::RenderWindow& t_window)
{
	// Begin the main menu bar
	ImGui::BeginMainMenuBar();

	// Begin the "File" menu
	if (ImGui::BeginMenu("File"))
	{
		// Button to export the canvas to an image file
		if (ImGui::Button("Export Image"))
		{
			t_canvas.saveToFile(openFileExplorer(FileHandlingFlag::SAVE_FILE));
		}

		// Button to open an image file and load it into the canvas
		if (ImGui::Button("Open Image"))
		{
			t_canvas.loadFromFile(openFileExplorer(FileHandlingFlag::SAVE_FILE));
		}
		ImGui::EndMenu();
	}

	// Begin the "Toolbar" menu
	if (ImGui::BeginMenu("Toolbar"))
	{
		// Draw an image button for each tool
		for (int i = 0; i < m_tools.size(); i++)
		{
			Tool* tool = m_tools[i];
			bool bisSelected = (i == m_selectedTool);
			if (ImGui::ImageButton(tool->getIcon(), sf::Vector2f(32.f, 32.f)))
			{
				m_selectedTool = i;
			}

			ImGui::SameLine();
		}
		ImGui::EndMenu();
	}

	// End the main menu bar
	ImGui::EndMainMenuBar();

	// Draw the settings for the selected tool
	drawToolSettings(t_window);
}

/**
 * @brief Returns a pointer to the selected tool.
 * @return A pointer to the selected tool, or nullptr if no tool is selected.
 */
Tool* Menu::getSelectedTool() const
{
	if (!m_tools.empty() && m_selectedTool != -1)
	{
		return m_tools.at(m_selectedTool);
	}
	return nullptr;
}

/**
 *@brief Returns the index of the currently selected tool.
 *@return An integer representing the index of the currently selected tool.
 */
int Menu::getSelectedToolNum() const
{
	return m_selectedTool;
}

/**
 * @brief Draws the main tool setting, different modifiable properties will be shown dependent on the current active tool.
 * @param t_window A const reference to the RenderWindow object for drawing the tool settings.
 */
void Menu::drawToolSettings(const sf::RenderWindow& t_window)
{
	ImGui::Begin("Tool Settings", nullptr , ImGuiWindowFlags_AlwaysAutoResize);

	// BRUSH & LINE TOOL SETTING
	////////////////////////////////////
	if (m_selectedTool == 0 || m_selectedTool == 1)
	{
		// Display color picker, size slider for brush and line tool
		ImGui::ColorPicker4("Color", m_guiColorValues.values, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Uint8);
		ImGui::SliderFloat("Brush Size", m_tools[m_selectedTool]->getSizeRef(), 1.f, 255.f, "%0.1f");


		// CHANGE BRUSH SHAPE OPTIONS
		////////////////////////////////////
		if (m_selectedTool == 0)
		{
			if (ImGui::Button("Round Brush"))
			{
				if (const auto brush = dynamic_cast<BrushTool*>(m_tools[m_selectedTool]))
				{
					brush->setBrushShape(BrushShape::ROUND);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Square Brush"))
			{
				if (const auto brush = dynamic_cast<BrushTool*>(m_tools[m_selectedTool]))
				{
					brush->setBrushShape(BrushShape::SQUARE);
				}
			}
		}
	}

	// SHAPE TOOL SETTINGS
	////////////////////////////////////
	if (m_selectedTool == 2 || m_selectedTool == 3 || m_selectedTool == 4)
	{
		ImGui::ColorPicker4("Color", m_guiColorValues.values, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Uint8);
	}

	// STAMP TOOL SETTINGS
	////////////////////////////////////
	if (m_selectedTool == 5)
	{
		// Dynamic cast the selected tool to Stamp tool so we can set the visibility of the preview stamp
		if (const auto stampTool = dynamic_cast<StampTool*>(m_tools[5]))
		{
			stampTool->setVisibility(true);
		}

		// Add new stamp to the list by opening the file explorer
		// Loaded stamp images, will be put onto a carousel that can be scrolled through to be selected again
		if (ImGui::Button("Load Stamp"))
		{
			addStamp(loadStamp());
		}

		// Delete selected stamp from the list
		if (ImGui::SameLine(); ImGui::Button("Delete Selected Stamp"))
		{
			// If only one stamp is left, show an error popup and return
			if (m_stamps.size() == 1)
			{
				ImGui::OpenPopup("Error");
				return;
			}

			// Find the selected stamp in the list, delete it and select the first remaining stamp
			for (const auto stamp : m_stamps)
			{
				if (stamp == m_selectedStamp)
				{
					std::erase(m_stamps, stamp);

					m_selectedStamp = m_stamps[0];
					m_tools[m_selectedTool]->setTexture(m_selectedStamp);
					break;
				}
			}
		}

		// Show error popup if only one stamp is left
		if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Can't delete the final stamp, must have at least 1 stamp");

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		// Show stamp carousel and set the selected stamp to the one that is clicked
		for (const auto stamp : m_stamps)
		{
			if (ImGui::ImageButton(*stamp, sf::Vector2f(32.f, 32.f)))
			{
				if (const auto stampTool = dynamic_cast<StampTool*>(m_tools[5]))
				{
					stampTool->setTexture(stamp);
					m_selectedStamp = stamp;
				}
			}
			ImGui::SameLine();
		}


		// Display color picker and size slider for stamp tool
		// Create slider that will modify the stamp scale
		ImGui::ColorPicker4("Color", m_guiColorValues.values, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Uint8);
		ImGui::SliderFloat("Stamp Scale", m_tools[m_selectedTool]->getSizeRef(), 0.01f, 10.f, "%0.2f");

		// Dynamic cast and update the preview stamp under the mouse cursor each frame
		if (const auto stampTool = dynamic_cast<StampTool*>(m_tools[5]))
		{
			stampTool->updatePreview(static_cast<sf::Vector2f>(sf::Mouse::getPosition(t_window)));
		}
	}
	else
	{
		// if you do not have the stamp tool as your current tool, we hide the preview stamp
		if (const auto stampTool = dynamic_cast<StampTool*>(m_tools[5]))
		{
			stampTool->setVisibility(false);
		}
	}

	ImGui::End();

	// Modify the color values to be used with SFML sf::Color
	// 0.0f - 1.0f values are converted to 0 - 255 for SFML use

	if (m_selectedTool != -1)
	{
		// Update color of selected tool
		m_tools[m_selectedTool]->setColor(
			sf::Color(static_cast<sf::Uint8>(255 * m_guiColorValues.r), // Red
				static_cast<sf::Uint8>(255 * m_guiColorValues.g), // Green
				static_cast<sf::Uint8>(255 * m_guiColorValues.b), // Blue
				static_cast<sf::Uint8>(255 * m_guiColorValues.a) // Alpha
			));
	}
}
