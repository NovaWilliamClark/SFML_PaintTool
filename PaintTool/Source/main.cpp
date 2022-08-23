/**
 * File: Main.cpp
 * Purpose: Application Entry Point
 * Author: William Clark
 * Date: 15/08/2022
 **/

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Application.h"

int main()
{
	const auto PaintApp = new Application();
	PaintApp->Loop();
	return 0;
}