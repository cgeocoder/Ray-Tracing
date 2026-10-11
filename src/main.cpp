#include <iostream>
#include <windows.h>

import draw;
import render;
import matrix;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Vec3 p = { 0.0, 0.5, 8.0 };
	Vec3 rotate_around_origin = { 0.0, 0.0, 9.0 };

	Sphere sphere2(1.0, { 0.5, -0.5, 9.0 }, Color(155, 255, 100));

	double angle = 0.0;
	while (render::alive()) {
		Vec3 new_coord = rotate_x(angle += PI / 50.0) * (p - rotate_around_origin) + rotate_around_origin;

		Sphere sphere1(1.0, Point(new_coord.x, new_coord.y, new_coord.z), Color(255, 155, 100));

		render::clear();
		render::draw(std::make_shared<Sphere>(sphere1));
		render::draw(std::make_shared<Sphere>(sphere2));
		
		render::render();
	}

	return 0;
}  

/*int main(int argc, char** argv) {
	Triangle t({ -3, -3, 5 }, { 0, 3, 7 }, { 3, -3, 5 });

	Point R = { 0, 0, 1 };

	double dist = t.distance_to_plane_eq(R);

	R.z = dist;

	bool res = t.in_triangle_area(R);

	if (res) {
		double cos_NR = scalar_product(R, t.normal()) / (vector_length(R) * vector_length(t.normal()));

		__debugbreak();
	}

	__debugbreak();
}*/