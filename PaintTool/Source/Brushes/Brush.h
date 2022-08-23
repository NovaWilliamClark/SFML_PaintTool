#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics.hpp>
enum class StrokeType
{
	None,
	Round,
	Square,
	Line,
	Rectangle,
	Ellipse,
};

class Brush : public sf::Shape
{

public:
	Brush(const sf::Vector2f& _StartPoint, const sf::Vector2f& _EndPoint, const float _BrushWidth, const sf::Color _BrushColor, StrokeType _BrushType);

	void setEndPoint(const sf::Vector2f& _EndPoint);
	virtual std::size_t getPointCount() const;
	virtual sf::Vector2f getPoint(std::size_t _Index) const;
	void setBrushWidth(const float _BrushWidth);

private:
	StrokeType m_BrushType;
	sf::Vector2f m_StartPoint;
	sf::Vector2f m_EndPoint;
	sf::Color m_BrushColor;
	float m_BrushWidth;
};

