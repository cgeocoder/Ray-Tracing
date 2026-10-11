#include <iostream>
#include <windows.h>

import draw;
import render;
import matrix;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Sphere sphere1(1.0, {0.0, 0.0, 7.0}, Color(255, 255, 100));
	Sphere sphere2(0.3, {0.1, -0.1, 4.0}, Color(255, 155, 100));

	while (render::alive()) {
		render::clear();
		render::draw(&sphere1);
		render::draw(&sphere2);
		
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