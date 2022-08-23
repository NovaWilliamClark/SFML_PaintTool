#pragma once
// Include SFML Libs
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
// C++ STL
#include <queue>
#include <memory>
// Project Files
#include "GUI.h"
#include "Brushes/Brush.h"

// Application State determines functionality of the paint tools
enum class AppState
{
	Nothing,
	BrushIdle,
	BrushDraw,
	LineFrom,
	LineTo,
	RectFrom,
	RectTo,
	EllipseFrom,
	EllipseTo,
	DrawPolygon,
	PolygonNextPoint,
	Stamp
};
	static AppState m_CurrentAppState;
	static StrokeType m_BrushType;
	static float* m_BrushSize{};
	static sf::RenderTexture* m_RenderTexture;
	//App Window
	static std::unique_ptr<sf::RenderWindow> m_AppWindow;
	static std::vector<std::unique_ptr<sf::Shape>>* m_DrawVector;
	static sf::Texture* m_ImportTexture;
	static sf::Texture* m_StampTexture;

class GUI;
class Application
{
public:
	// Constructor & Destructor
	Application();
	~Application();

	void Init();
	void Update();
	void Loop();

	static AppState getAppState();

	static void setAppState(AppState _NewAppState);

	static sf::RenderWindow* getApplicationWindow()
	{
		return m_AppWindow.get();
	}

	static StrokeType getBrushType()
	{
		return m_BrushType;
	}

	static void setBrushType(const StrokeType _NewBrushType);
	static float* getBrushSize();
	static void SaveAsPNG();
	static void LoadPNG(bool _BStamp);
	static void PushToDrawStack(std::unique_ptr<sf::RectangleShape> _PushShape);

private:
	std::vector<std::unique_ptr<sf::Shape>>* m_PolygonLineVector;
	GUI* m_GUI;

	// Store the vector into SFMLs color class
	sf::Color m_CurrentColor;

	// Store the previous position of the brush so the brush can use later
	sf::Vector2f* m_PrevPoint;

	sf::Vector2f m_PolygonPrevPoint;

	int m_PolygonCount;
	// Store the current position of the brush
	sf::Vector2i m_MousePosition;

	bool m_bIsDrawingShape;

	sf::Vector2f m_ShapeStartPos;
	std::unique_ptr<Brush> m_DrawShape;
	std::unique_ptr<sf::ConvexShape> m_Polygon;

	std::vector<sf::Vector2f> m_PolygonBuffer;
};

