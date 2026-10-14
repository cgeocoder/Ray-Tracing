#include <iostream>
#include <windows.h>

import draw;
import render;
import matrix;

Vec3 rotate_around_x(const Vec3& _Src, const Vec3& _Origin, double _Angle) {
	return rotate_x(_Angle) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_xy(const Vec3& _Src, const Vec3& _Origin, double _AngleX, double _AngleY) {
	return rotate_y(_AngleY) * rotate_x(_AngleX) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_xz(const Vec3& _Src, const Vec3& _Origin, double _AngleX, double _AngleZ) {
	return rotate_z(_AngleZ) * rotate_x(_AngleX) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_xyz(const Vec3& _Src, const Vec3& _Origin, double _AngleX, double _AngleY, double _AngleZ) {
	return rotate_z(_AngleZ) * rotate_y(_AngleY) * rotate_x(_AngleX) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_y(const Vec3& _Src, const Vec3& _Origin, double _Angle) {
	return rotate_y(_Angle) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_yz(const Vec3& _Src, const Vec3& _Origin, double _AngleY, double _AngleZ) {
	return rotate_z(_AngleZ) * rotate_y(_AngleY) * (_Src - _Origin) + _Origin;
}

Vec3 rotate_around_z(const Vec3& _Src, const Vec3& _Origin, double _Angle) {
	return rotate_z(_Angle) * (_Src - _Origin) + _Origin;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Vec3 sun_pos = { 0.0, 0.0, 20.0 };
	Vec3 planet1_pos = { 0.0, 0.0, 25.0 };
	Vec3 planet2_pos = { 0.0, 0.0, 23.0 };
	
	Vec3 moon_pos = { planet1_pos.x, planet1_pos.y, planet1_pos.z + 1.5 };

	render::camera_rotate_x_right(0);
	render::camera_rotate_y_down(0.05);
	
	Plane plane({ -1, -10, -1 }, { 0, -9.4, 1 }, { 1, -10, -1 }, {0, 0, 0}, Color(1, 0, 1));
	
	while (render::alive()) {
		Vec3 last_planet1_pos = planet1_pos;
		Vec3 last_sun_pos = sun_pos;
		
		sun_pos = rotate_around_x(sun_pos, { 0.0, 0.0, 19.5 }, -PI / 500.0);
		planet1_pos = rotate_around_xy(planet1_pos, sun_pos, PI / 1000.0, PI / 600.0 * PI) + (sun_pos - last_sun_pos);
		planet2_pos = rotate_around_y(planet2_pos, sun_pos, -PI / 500.0) + (sun_pos - last_sun_pos);
		
		// rotate + offset
		moon_pos = rotate_around_xy(moon_pos, planet1_pos, -PI / 1000.0, PI / 70.0) + (planet1_pos - last_planet1_pos);
		
		Sphere sun(2.0, sun_pos, Color(0, 1, 1));
		Sphere sphere1(1.0, planet1_pos, Color(1, 1, 0));
		Sphere sphere2(0.6, planet2_pos, Color(0.2, 1, 0));
		Sphere moon(0.2, moon_pos, Color(1, 1, 1));

		render::clear();
		render::draw(&plane);
		render::draw(&sphere1);
		render::draw(&sphere2);
		render::draw(&sun);
		render::draw(&moon);
		
		render::render();
	}

	return 0;
}  
