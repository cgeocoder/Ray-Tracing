#include <iostream>
#include <windows.h>

import draw;
import render;
import matrix;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Sphere sphere2(1.0, { 0.0, 0.5, 8.0 }, Color(255, 155, 100));
	Sphere sphere3(1.0, { 0.5, -0.5, 9.0 }, Color(155, 255, 100));

	double d = 7.0;
	while (render::alive()) {
		Sphere sphere1(1.0, { -0.5, -0.5, d }, Color(255, 255, 100));
		d += 0.01;

		render::clear();
		render::draw(&sphere1);
		render::draw(&sphere2);
		render::draw(&sphere3);
		
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