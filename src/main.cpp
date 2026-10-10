#include <iostream>
#include <windows.h>

import draw;
import render;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR l, int nCmdShow) {
	render::init_windows(hInstance, h, l, nCmdShow);

	Triangle triangle({ -180, -180, 1 }, { 0, 30, 900 }, { 160, -130, 1 });

	while (render::alive()) {
		render::clear();
		render::draw_triangle(triangle);
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