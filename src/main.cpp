#include <iostream>
#include <windows.h>

import draw;
import render;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Point points[] = {
		{ -80, -80, 1 },
		{ -80, 80, 100 },
		{ 80, 80, 100 },
		{ 80, -80, 1 },
	};

	Triangle triangle1(points[0], points[1], points[2]);
	Triangle triangle2(points[2], points[3], points[0]);

	while (render::alive()) {

		render::clear();
		render::draw_triangle(triangle1);
		render::draw_triangle(triangle2);
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