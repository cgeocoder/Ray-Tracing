#include <iostream>
#include <windows.h>

import draw;
import render;
import matrix;

Vec3 rotate_around_x(const Vec3& _Src, const Vec3& _Origin, double _Angle) {
	return rotate_x(_Angle) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_y(const Vec3& _Src, const Vec3& _Origin, double _Angle) {
	return rotate_y(_Angle) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_z(const Vec3& _Src, const Vec3& _Origin, double _Angle) {
	return rotate_z(_Angle) * (_Src - _Origin) + _Origin;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Vec3 sun_pos = { 0.0, 0.0, 20.0 };
	Vec3 planet1_pos = { 0.0, 0.0, 25.0 };
	Vec3 moon_pos = { planet1_pos.x, planet1_pos.y, planet1_pos.z + 1.5 };

	Sphere sun(2.0, { sun_pos.x, sun_pos.y, sun_pos.z }, Color(0, 255, 255));

	while (render::alive()) {
		Vec3 last_planet1_pos = planet1_pos;

		planet1_pos = rotate_around_y(planet1_pos, sun_pos, PI / 250.0);

		// rotate + offset
		moon_pos = rotate_around_y(moon_pos, planet1_pos, -PI / 50.0) + (planet1_pos - last_planet1_pos);

		Sphere sphere1(1.0, Point(planet1_pos.x, planet1_pos.y, planet1_pos.z), Color(255, 255, 0));
		Sphere moon(0.2, Point(moon_pos.x, moon_pos.y, moon_pos.z), Color(255, 255, 255));

		render::clear();
		render::draw(std::make_shared<Sphere>(sphere1));
		render::draw(std::make_shared<Sphere>(sun));
		render::draw(std::make_shared<Sphere>(moon));
		
		render::render();
	}

	return 0;
}  
