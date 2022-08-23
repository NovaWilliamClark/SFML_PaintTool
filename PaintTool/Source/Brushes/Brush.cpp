#define _USE_MATH_DEFINES
#include <cmath>
#include "Brush.h"
Brush::Brush(const sf::Vector2f& _StartPoint, const sf::Vector2f& _EndPoint, const float _BrushWidth, const sf::Color _BrushColor, StrokeType _BrushType)
{
	m_StartPoint = _StartPoint;
	m_EndPoint = _EndPoint;
	m_BrushWidth = _BrushWidth;
	m_BrushColor = _BrushColor;
	m_BrushType = _BrushType;
	setPosition(m_StartPoint);
	setFillColor(m_BrushColor);
	update();
}

void Brush::setEndPoint(const sf::Vector2f& _EndPoint)
{
	m_EndPoint = _EndPoint;
	update();
}

std::size_t Brush::getPointCount() const
{
	if (m_BrushType == StrokeType::Round || m_BrushType == StrokeType::Ellipse)
	{
		return 30;
	}
	if (m_BrushType == StrokeType::Square || m_BrushType == StrokeType::Rectangle || m_BrushType == StrokeType::Line)
	{
		return 4;
	}
}

sf::Vector2f Brush::getPoint(std::size_t _Index) const
{
	if (m_BrushType == StrokeType::Round)
	{
		const sf::Vector2f Point1(1.f, 0.f);
		const sf::Vector2f Point2(m_EndPoint + sf::Vector2f(1.0, 0.0) - getPosition());
		sf::Vector2f DrawOffset;
		int FlipScalar;

		if (_Index < 15)
		{
			DrawOffset = Point2;
			FlipScalar = 1;
		}
		else
		{
			DrawOffset = Point1;
			FlipScalar = -1;
			_Index -= 15;
		}

		const float Start = (-atan2(Point1.y - Point2.y, Point2.x - Point1.x));
		const float Angle = _Index * (M_PI / 14) - (M_PI / 2) + Start;
		const float X = std::cos(Angle) * m_BrushWidth / 2;
		const float Y = std::sin(Angle) * m_BrushWidth / 2;
		return {DrawOffset.x + X * FlipScalar, DrawOffset.y + Y * FlipScalar};
	}

	if (m_BrushType == StrokeType::Square)
	{
		const sf::Vector2f Point1(1.f, 0.f);
		const sf::Vector2f Point2(m_EndPoint + sf::Vector2f(1.0, 0.0) - getPosition());
		sf::Vector2f DrawOffset;
		int FlipScalar;

		if (_Index < 2)
		{
			DrawOffset = Point2;
			FlipScalar = 1;
		}
		else
		{
			DrawOffset = Point1;
			FlipScalar = -1;
			_Index -= 2;
		}

		const float Start = (-atan2(Point1.y - Point2.y, Point2.x - Point1.x));
		const float Angle = _Index * (M_PI / 2) - (M_PI / 4) + Start;
		const float X = std::cos(Angle) * m_BrushWidth;
		const float Y = std::sin(Angle) * m_BrushWidth;
		return {DrawOffset.x + X * FlipScalar, DrawOffset.y + Y * FlipScalar};
	}

	if(m_BrushType == StrokeType::Line)
	{
		const sf::Vector2f Point1 = m_StartPoint;
		const sf::Vector2f Point2 = m_EndPoint;
		const auto opposite = (Point2.y - Point1.y);
		const auto adjacent = (Point2.x - Point1.x);
		const float hypotenuse = sqrt(pow(opposite, 2) + pow(adjacent, 2));
		float angle = (-atan2(adjacent, opposite) + 1.57) * (180 / M_PI);
		switch (_Index)
		{
			default:
			case 0: return {0, 0};
			case 1: return {hypotenuse, 0};
			case 2: return {hypotenuse, m_BrushWidth};
			case 3: return {0, m_BrushWidth};
		}
	}

	if(m_BrushType == StrokeType::Rectangle)
	{
		sf::Vector2f Size((m_EndPoint.x - m_StartPoint.x), (m_EndPoint.y - m_StartPoint.y));
		switch (_Index)
		{
		default:
		case 0: return {0, 0};
		case 1: return {Size.x, 0};
		case 2: return {Size.x, Size.y};
		case 3: return {0, Size.y};
		}
	}

	if(m_BrushType == StrokeType::Ellipse)
	{
		const sf::Vector2f Size(abs(m_EndPoint.x - m_StartPoint.x), abs(m_EndPoint.y - m_StartPoint.y));
		const float angle = _Index * 2 * M_PI / getPointCount() - M_PI / 2;
		const float X = std::cos(angle) * Size.x;
		const float Y = std::sin(angle) * Size.y;

		return {m_BrushWidth + X, m_BrushWidth + Y};
	}
}


void Brush::setBrushWidth(const float _BrushWidth)
{
	m_BrushWidth = _BrushWidth;
	update();
}
