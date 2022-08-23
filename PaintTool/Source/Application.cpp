#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include "Application.h"

#include <corecrt_math_defines.h>
#include <Windows.h>

Application::~Application()
{
	delete m_ImportTexture;
	ImGui::SFML::Shutdown();
}


Application::Application(): m_GUI(nullptr), m_PrevPoint(nullptr), m_PolygonCount(0), m_bIsDrawingShape(false)
{
	m_StampTexture = new sf::Texture;
	m_ImportTexture = new sf::Texture;
	m_RenderTexture = new sf::RenderTexture;
	m_CurrentAppState = AppState::Nothing;
	m_BrushType = StrokeType::None;
	m_DrawVector = new std::vector<std::unique_ptr<sf::Shape>>;
	m_PolygonLineVector = new std::vector<std::unique_ptr<sf::Shape>>;

}

void Application::Init()
{
	constexpr int WindowWidth = 1280;
	constexpr int WindowHeight = 720;
	sf::ContextSettings ApplicationSettings;
	ApplicationSettings.antialiasingLevel = 16;
	m_AppWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(WindowWidth, WindowHeight), "Paint - William Clark", sf::Style::Default, ApplicationSettings);
	m_AppWindow->setFramerateLimit(120);
	m_AppWindow->setVerticalSyncEnabled(true);
	m_AppWindow->setMouseCursorVisible(false);
	m_RenderTexture->create(WindowWidth, WindowHeight);
	m_RenderTexture->clear(sf::Color::White);

	// Initialize ImGui SFML Bindings
	ImGui::SFML::Init(*m_AppWindow);
	// Remove ImGui.ini File
	ImGui::GetIO().IniFilename = nullptr;


	// Initialize GUI()
	m_GUI = new GUI();
	
	// Setup Brush Defaults
	// Initialize Brush as Black
	m_CurrentColor = sf::Color
	(0,0,0, 255);
	// Set Brush Size
	m_BrushSize = new float(10);
	// Set Brush Stroke Type
	m_BrushType = StrokeType::None;

	m_PolygonCount = 0;


	// Display Console Info
	std::cout << "\n Paint App is Starting" << std::endl;
}

void Application::Update()
{
	// Update Our Canvas
	sf::Event InputEvent{};

	while (m_AppWindow->pollEvent(InputEvent))
	{
		m_MousePosition = sf::Mouse::getPosition(*m_AppWindow);
		ImGui::SFML::ProcessEvent(InputEvent);

		// If the window is closed using the 'X' button close application and end program
		if (InputEvent.type == sf::Event::Closed)
		{
			m_AppWindow->close();
		}
		if (InputEvent.type == sf::Event::LostFocus)
		{
			break;
		}

		// catch the resize events
		if (InputEvent.type == sf::Event::Resized)
		{
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0.f, 0.f, InputEvent.size.width, InputEvent.size.height);

			m_AppWindow->setView(sf::View(visibleArea));
		}
		 
		if (InputEvent.type == sf::Event::MouseButtonReleased)
		{
			if(m_CurrentAppState == AppState::BrushDraw)
			{
				m_CurrentAppState = AppState::BrushIdle;
			}
			if(m_CurrentAppState == AppState::LineTo)
			{
				m_CurrentAppState = AppState::LineFrom;
				m_DrawVector->push_back(std::move(m_DrawShape));
			}
			if (m_CurrentAppState == AppState::EllipseTo)
			{
				m_CurrentAppState = AppState::EllipseFrom;
				m_DrawVector->push_back(std::move(m_DrawShape));
			}
			if (m_CurrentAppState == AppState::RectTo)
			{
				m_CurrentAppState = AppState::RectFrom;
				m_DrawVector->push_back(std::move(m_DrawShape));
			}
			m_PrevPoint = nullptr;
			m_bIsDrawingShape = false;
		}
		// Check if mouse is within windows bounds
		if ((m_MousePosition.x >= 250 && m_MousePosition.x <= 250) + InputEvent.size.width && m_MousePosition.y >= 0 && m_MousePosition.y <= 0 + InputEvent.size.height)
		{
			// When the left click is pressed we must determine what the functionality is using AppState
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// Paint Brush is active
				if ((m_CurrentAppState == AppState::BrushIdle || m_CurrentAppState == AppState::BrushDraw))
				{
					// If We are on brush but don't have a Stroke Type, the stroke type to round
					if (m_BrushType == StrokeType::None)
					{
						m_BrushType = StrokeType::Round;
					}

					// Check if we have already started drawing
					if (m_PrevPoint != nullptr && m_CurrentAppState == AppState::BrushDraw && (m_BrushType == StrokeType::Round || m_BrushType == StrokeType::Square))
					{
						std::unique_ptr<Brush> BrushStroke(new Brush(*m_PrevPoint, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType));
						m_PrevPoint = new sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
						m_DrawVector->push_back(std::move(BrushStroke));
					}
					// We haven't started painting so start painting
					else if (m_BrushType == StrokeType::Round || m_BrushType == StrokeType::Square)
					{
						m_CurrentAppState = AppState::BrushDraw;
						std::unique_ptr<Brush> BrushStroke(new Brush(sf::Vector2f(m_MousePosition.x, m_MousePosition.y), sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType));
						m_PrevPoint = new sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
						m_DrawVector->push_back(std::move(BrushStroke));
					}
				}


				if (m_CurrentAppState == AppState::LineFrom && !m_bIsDrawingShape && m_BrushType == StrokeType::Line)
				{
					m_ShapeStartPos = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
					m_CurrentAppState = AppState::LineTo;
					m_bIsDrawingShape = true;
					m_DrawShape = std::make_unique<Brush>(m_ShapeStartPos, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType);
				}

				if (m_CurrentAppState == AppState::LineTo)
				{
					sf::Vector2f Point1 = m_ShapeStartPos;
					sf::Vector2f Point2 = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);

					float opposite = (Point2.y - Point1.y);
					float adjacent = (Point2.x - Point1.x);

					float theta = -atan2(adjacent, opposite) + 1.57;
					float degrees = theta * (180 / M_PI);

					m_DrawShape->setRotation(degrees);
					m_DrawShape->setEndPoint(sf::Vector2f(m_MousePosition.x, m_MousePosition.y));
					m_AppWindow->draw(*m_DrawShape);
				}

				if (m_CurrentAppState == AppState::EllipseFrom && !m_bIsDrawingShape && m_BrushType == StrokeType::Ellipse)
				{
					m_ShapeStartPos = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
					m_CurrentAppState = AppState::EllipseTo;
					m_bIsDrawingShape = true;
					m_DrawShape = std::make_unique<Brush>(m_ShapeStartPos, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType);
				}

				if (m_CurrentAppState == AppState::EllipseTo)
				{
					m_DrawShape->setEndPoint(sf::Vector2f(m_MousePosition.x, m_MousePosition.y));
				}

				if (m_CurrentAppState == AppState::EllipseFrom && !m_bIsDrawingShape && m_BrushType == StrokeType::Ellipse)
				{
					m_ShapeStartPos = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
					m_CurrentAppState = AppState::EllipseTo;
					m_bIsDrawingShape = true;
					m_DrawShape = std::make_unique<Brush>(m_ShapeStartPos, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType);
				}

				if (m_CurrentAppState == AppState::EllipseTo)
				{
					m_DrawShape->setEndPoint(sf::Vector2f(m_MousePosition.x, m_MousePosition.y));
				}

				if (m_CurrentAppState == AppState::RectFrom && !m_bIsDrawingShape && m_BrushType == StrokeType::Rectangle)
				{
					m_ShapeStartPos = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
					m_CurrentAppState = AppState::RectTo;
					m_bIsDrawingShape = true;
					m_DrawShape = std::make_unique<Brush>(m_ShapeStartPos, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType);
				}

				if (m_CurrentAppState == AppState::RectTo)
				{
					m_DrawShape->setEndPoint(sf::Vector2f(m_MousePosition.x, m_MousePosition.y));
				}

				if (m_CurrentAppState == AppState::RectFrom && !m_bIsDrawingShape && m_BrushType == StrokeType::Rectangle)
				{
					m_ShapeStartPos = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
					m_CurrentAppState = AppState::RectTo;
					m_bIsDrawingShape = true;
					m_DrawShape = std::make_unique<Brush>(m_ShapeStartPos, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), *m_BrushSize * 2, m_CurrentColor, m_BrushType);
				}

				if (m_CurrentAppState == AppState::RectTo)
				{
					m_DrawShape->setEndPoint(sf::Vector2f(m_MousePosition.x, m_MousePosition.y));
				}

				if (m_CurrentAppState == AppState::DrawPolygon || m_CurrentAppState == AppState::PolygonNextPoint && !m_bIsDrawingShape)
				{
					m_bIsDrawingShape = true;
					if (m_CurrentAppState == AppState::PolygonNextPoint)
					{
						m_PolygonLineVector->push_back(std::move(m_DrawShape));
					}
					m_CurrentAppState = AppState::PolygonNextPoint;
					m_PolygonCount += 1;
					m_PolygonPrevPoint = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);
					m_PolygonBuffer.emplace_back(m_MousePosition);
					m_DrawShape = std::make_unique<Brush>(m_PolygonPrevPoint, sf::Vector2f(m_MousePosition.x, m_MousePosition.y), 2, m_CurrentColor, StrokeType::Line);
				}

				// Stamp Tool
				if (m_CurrentAppState == AppState::Stamp && !m_bIsDrawingShape)
				{
					m_bIsDrawingShape = true;
					std::unique_ptr<sf::RectangleShape> BrushStroke(new sf::RectangleShape(sf::Vector2f(m_StampTexture->getSize().x * *m_BrushSize/30, m_StampTexture->getSize().y * *m_BrushSize/30)));
					BrushStroke->setPosition(sf::Vector2f(m_MousePosition));
					BrushStroke->setTexture(m_StampTexture);
					m_DrawVector->push_back(std::move(BrushStroke));
				}
			}
		}

		if (m_CurrentAppState == AppState::PolygonNextPoint)
		{
			sf::Vector2f Point1 = m_PolygonPrevPoint;
			sf::Vector2f Point2 = sf::Vector2f(m_MousePosition.x, m_MousePosition.y);

			float opposite = (Point2.y - Point1.y);
			float adjacent = (Point2.x - Point1.x);

			float theta = -atan2(adjacent, opposite) + 1.57;
			float degrees = theta * (180 / M_PI);

			m_DrawShape->setRotation(degrees);
			m_DrawShape->setEndPoint(sf::Vector2f(m_MousePosition.x, m_MousePosition.y));
			m_AppWindow->draw(*m_DrawShape);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				m_Polygon = std::make_unique<sf::ConvexShape>(m_PolygonBuffer.size());
				m_Polygon->setFillColor(m_CurrentColor);
				for (int i = 0; i < m_PolygonBuffer.size(); i++)
				{
					m_Polygon->setPoint(i, m_PolygonBuffer[i]);
				}
				m_DrawVector->push_back(std::move(m_Polygon));
				m_PolygonLineVector->clear();
				m_PolygonBuffer.clear();
				m_DrawShape = nullptr;
				m_CurrentAppState = AppState::DrawPolygon;
			}
		}
	}
	if (m_DrawShape != nullptr)
	{
		m_AppWindow->draw(*m_DrawShape);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		m_AppWindow->close();
		exit(EXIT_SUCCESS);
	}
}

void Application::Loop()
{
	Init();

	sf::Clock DeltaClock;

	m_AppWindow->clear(sf::Color::White);

	while (m_AppWindow->isOpen())
	{
		m_CurrentColor = sf::Color (255 * m_GUI->getColorVector()[0], 255 * m_GUI->getColorVector()[1], 255 * m_GUI->getColorVector()[2]);
		m_AppWindow->clear(sf::Color::White);
		Update();
		ImGui::SFML::Update(*m_AppWindow, DeltaClock.restart());
		m_GUI->UpdateGUI(m_MousePosition);

		for (auto& DrawShape : *m_DrawVector)
		{
			m_RenderTexture->draw(*DrawShape);
			m_AppWindow->draw(*DrawShape);
		}

		for (auto& PolygonLine : *m_PolygonLineVector)
		{
			m_AppWindow->draw(*PolygonLine);
		}

		Update();

		ImGui::SFML::Render(*m_AppWindow);

		m_AppWindow->display();
	}
}

AppState Application::getAppState()
{
	return m_CurrentAppState;
}

void Application::setAppState(AppState _NewAppState)
{
	m_CurrentAppState = _NewAppState;
}

void Application::setBrushType(const StrokeType _NewBrushType)
{
	m_BrushType = _NewBrushType;
}

float* Application::getBrushSize()
{
	return m_BrushSize;
}

void Application::SaveAsPNG()
{
	OPENFILENAME OFN = { 0 };
	wchar_t szFile[260] = L"Untitled.png";

	// init
	OFN.lStructSize = sizeof(OFN);
	OFN.hwndOwner = m_AppWindow->getSystemHandle();
	OFN.lpstrFile = szFile;
	OFN.nMaxFile = sizeof(szFile);
	OFN.lpstrFilter = L"png(.png)\0*.png\0bmp(.bmp)\0*.bmp\0";
	OFN.nFilterIndex = 1;
	OFN.lpstrFileTitle = nullptr;
	OFN.nMaxFileTitle = 0;
	OFN.lpstrInitialDir = nullptr;
	OFN.Flags = 0x00000800 | 0x00001000;

	std::string rString = "Hello";

	if (GetSaveFileNameW(&OFN))
	{
		std::wstring fileName = OFN.lpstrFile;
		rString = std::string(fileName.begin(), fileName.end());
	}

	std::cout << rString << std::endl;
	m_RenderTexture->getTexture().copyToImage().saveToFile(rString);
}

void Application::LoadPNG(bool _bStamp)
{
	OPENFILENAME ofn = { 0 };
	wchar_t szFile[260] = { 0 };

	// init
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_AppWindow->getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"png(.png)\0*.png\0bmp(.bmp)\0*.bmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = 0x00000800 | 0x00001000;

	std::string rString = "Hello";

	if (GetOpenFileNameW(&ofn))
	{
		std::wstring fileName = ofn.lpstrFile;
		rString = std::string(fileName.begin(), fileName.end());
	}

	if(_bStamp)
	{
		m_StampTexture->create(m_AppWindow->getSize().x, m_AppWindow->getSize().y);
		m_StampTexture->loadFromFile(rString);
	}

	if(!_bStamp)
	{
		m_ImportTexture->create(m_AppWindow->getSize().x, m_AppWindow->getSize().y);
		m_ImportTexture->loadFromFile(rString);
		const auto Temp = new sf::RectangleShape(sf::Vector2f(m_AppWindow->getSize().x, m_AppWindow->getSize().y));
		Temp->setTexture(m_ImportTexture);
		m_DrawVector->push_back(std::unique_ptr<sf::Shape>(Temp));
	}

	std::cout << rString << std::endl;
}


