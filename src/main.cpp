#include <iostream>
#include <windows.h>

import draw;
import render;
import matrix;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Vec3 sun_pos = { 0.0, 0.0, 20.0 };
	Sphere sun(2.0, { sun_pos.x, sun_pos.y, sun_pos.z }, Color(0, 255, 255));

	Vec3 planet1_pos = { 0.0, 0.0, 25.0 };
	Matrix3x3 planet1_rotation = rotate_y(PI / 250.0);

	Vec3 moon_pos = { planet1_pos.x, planet1_pos.y, planet1_pos.z + 1.4 };
	Matrix3x3 moon_rotation = rotate_y(-PI / 50.0);

	while (render::alive()) {
		Vec3 last_planet1_pos = planet1_pos;
		planet1_pos = planet1_rotation * (planet1_pos - sun_pos) + sun_pos;

		moon_pos = moon_rotation * (moon_pos - planet1_pos) + planet1_pos + (planet1_pos - last_planet1_pos);

		Sphere sphere1(1.0, Point(planet1_pos.x, planet1_pos.y, planet1_pos.z), Color(255, 255, 0));
		Sphere moon(0.5, Point(moon_pos.x, moon_pos.y, moon_pos.z), Color(255, 255, 255));

		render::clear();
		render::draw(std::make_shared<Sphere>(sphere1));
		render::draw(std::make_shared<Sphere>(sun));
		render::draw(std::make_shared<Sphere>(moon));
		
		render::render();
	}

	return 0;
}  
