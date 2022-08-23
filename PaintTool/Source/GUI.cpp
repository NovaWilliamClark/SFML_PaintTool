#include "GUI.h"
#include <iostream>
#include "Application.h"

/************************************************************************/
/*                               GUI                                    */
/************************************************************************/

GUI::GUI()
{
	m_ColorVector = new float[3] {0, 0, 0};
	m_bShouldDisplayDebug = false;

	m_ButtonIcns.BrushTool.loadFromFile("Resources/Icons/BrushTool.png");
	m_ButtonIcns.LineTool.loadFromFile("Resources/Icons/LineTool.png");
	m_ButtonIcns.RectangleTool.loadFromFile("Resources/Icons/RectangleTool.png");
	m_ButtonIcns.EllipseTool.loadFromFile("Resources/Icons/EllipseTool.png");
	m_ButtonIcns.Polygon.loadFromFile("Resources/Icons/PolygonTool.png");
	m_ButtonIcns.CircleBrushShape.loadFromFile("Resources/Icons/CircleBrush.png");
	m_ButtonIcns.SquareBrushShape.loadFromFile("Resources/Icons/SquareBrush.png");
	m_ButtonIcns.Stamp.loadFromFile("Resources/Icons/Stamp.png");
}

GUI::~GUI()
{
	delete m_ColorVector;
}

const char* GUI::getStateStr()
{
	switch (Application::getAppState())
	{
	case AppState::Nothing:
		return "Nothing";
	case AppState::BrushIdle:
		return "Brush Idle";
	case AppState::BrushDraw:
		return "Brush Draw";
	case AppState::LineFrom:
		return "Line From";
	case AppState::LineTo:
		return "Line To";
	case AppState::RectFrom:
		return "Rect From";
	case AppState::RectTo:
		return "Rect To";
	case AppState::EllipseFrom:
		return "Ellipse From";
	case AppState::EllipseTo:
		return "Ellipse To";
	case AppState::DrawPolygon:
		return "Draw Polygon";
	default:
		Application::setAppState(AppState::Nothing);
		return "Nothing";
	}
}

void GUI::UpdateGUI(const sf::Vector2i _MousePosition)
{
	float MainMenuBarWidth{};
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		MainMenuBarWidth = ImGui::GetWindowHeight();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import"))
			{
				Application::LoadPNG(false);
			}

			if (ImGui::MenuItem("Export"))
			{
				Application::SaveAsPNG();
			}
			if (ImGui::MenuItem("Toggle Debug Menu"))
			{
				if (!m_bShouldDisplayDebug)
				{
					m_bShouldDisplayDebug = true;
				}
				else
				{
					m_bShouldDisplayDebug = false;
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo"))
			{
				// Implement Undo Function
			}
			if (ImGui::MenuItem("Redo"))
			{
				// Implement Redo Function
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	// Toolbox
	ImGui::SetNextWindowPos(ImVec2(0, MainMenuBarWidth));
	ImGui::SetNextWindowSize(ImVec2(250, 720), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTabBar("#Tools"))
		{
			if (ImGui::BeginTabItem("Draw Tools"))
			{
				// Paint Brush
				if (ImGui::ImageButton(m_ButtonIcns.BrushTool, ImVec2(32, 32)))
				{
					Application::setAppState(AppState::BrushIdle);
					Application::setBrushType(StrokeType::Round);
					std::cout << "Tool Selected: Brush" << std::endl;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Brush Tool");
				}
				ImGui::SameLine();

				// Line Tool
				if (ImGui::ImageButton(m_ButtonIcns.LineTool, ImVec2(32, 32)))
				{
					Application::setAppState(AppState::LineFrom);
					Application::setBrushType(StrokeType::Line);
					std::cout << "Tool Selected: Line Tool" << std::endl;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Line Tool");
				}
				ImGui::SameLine();

				// Rectangle Tool
				if (ImGui::ImageButton(m_ButtonIcns.RectangleTool, ImVec2(32, 32)))
				{
					Application::setAppState(AppState::RectFrom);
					Application::setBrushType(StrokeType::Rectangle);
					std::cout << "Tool Selected: Rectangle Tool" << std::endl;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Rectangle Tool");
				}
				ImGui::SameLine();

				// Ellipse Tool
				if (ImGui::ImageButton(m_ButtonIcns.EllipseTool, ImVec2(32, 32)))
				{
					Application::setAppState(AppState::EllipseFrom);
					Application::setBrushType(StrokeType::Ellipse);
					std::cout << "Tool Selected: Ellipse Tool" << std::endl;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Ellipse Tool");
				}
				ImGui::SameLine();

				// Polygon Tool
				if (ImGui::ImageButton(m_ButtonIcns.Polygon, ImVec2(32, 32)))
				{
					Application::setAppState(AppState::DrawPolygon);
					std::cout << "Tool Selected: Polygon Tool" << std::endl;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Polygon Tool");
				}

				if (ImGui::ImageButton(m_ButtonIcns.Stamp, ImVec2(32, 32)))
				{
					Application::setAppState(AppState::Stamp);
					Application::LoadPNG(true);
					std::cout << "Tool Selected: Stamp Tool" << std::endl;
				};
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Stamp Tool");
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();

		if (Application::getAppState() == AppState::BrushIdle || Application::getAppState() == AppState::BrushDraw || Application::getAppState() == AppState::Stamp)
		{
			if (Application::getAppState() != AppState::Stamp)
			{
				if (ImGui::CollapsingHeader("Brush Shape", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (ImGui::ImageButton(m_ButtonIcns.CircleBrushShape, ImVec2(24, 24)))
					{
						if (Application::getBrushType() != StrokeType::Round)
						{
							Application::setBrushType(StrokeType::Round);
							std::cout << "Circle Brush Selected" << std::endl;
						}
					}
					ImGui::SameLine();

					if (ImGui::ImageButton(m_ButtonIcns.SquareBrushShape, ImVec2(24, 24)))
					{
						if (m_BrushStroke != StrokeType::Square)
						{
							Application::setBrushType(StrokeType::Square);
							std::cout << "Square Brush Selected" << std::endl;
						}
					}
				}
			}
			ImGui::SliderFloat("Brush Size", &*Application::getBrushSize(), 1.f, 80.f);
		}

		if (ImGui::CollapsingHeader("Color", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorPicker3("Color", m_ColorVector);
		}

		if (m_bShouldDisplayDebug)
		{
			if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// Debug Menu
				ImGui::Text("Mouse Position:");
				ImGui::Text("x: %d, y: %d", _MousePosition.x, _MousePosition.y);
				ImGui::Text("App State : ");
				ImGui::Text("Brush State: %s", getStateStr());

			}
		}
	}
	ImGui::End();
}
