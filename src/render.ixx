module;

#include <windows.h>
#include <vector>

export module render;
import draw;

namespace render {

	MSG msg = {};
	std::vector<Triangle> triangles;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_PAINT: {
			RECT client_rect;
			GetClientRect(hwnd, &client_rect);
			int width = client_rect.right - client_rect.left;
			int height = client_rect.bottom - client_rect.top;

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// for (int i = 0; i < 200; ++i) {
			// 	SetPixel(hdc, i, i, RGB(255, 0, 0));
			// }

			for (const auto& triangle : triangles) {

				for (int x = 0; x < width; ++x) {
					double nx = x - width / 2.0;

					for (int y = 0; y < height; ++y) {
						double ny = height / 2.0 - y;

						Point R = { nx, ny, 1 };

						double dist = triangle.distance_to_plane_eq(R);

						if (dist <= 1000.0) {
							R.z = dist;

							if (triangle.in_triangle_area(R)) {
								double cos_NR = abs(1 - scalar_product(R, triangle.normal())
									/ (vector_length(R) * vector_length(triangle.normal())));

								SetPixel(hdc, x, y, RGB(cos_NR * 255, cos_NR * 255, cos_NR * 255));
							}
							else {
								SetPixel(hdc, x, y, RGB(0, 0, 0));
							}
						}
						else {
							SetPixel(hdc, x, y, RGB(0, 0, 0));
						}
					}
				}
			}

			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};

	export void init_windows(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
		const wchar_t CLASS_NAME[] = L"Ray Tracing";
		WNDCLASS wc = {
			.lpfnWndProc = WndProc,
			.hInstance = hInstance,
			.lpszClassName = CLASS_NAME
		};
		RegisterClass(&wc);

		HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Ray Tracing",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 700,
			nullptr, nullptr, hInstance, nullptr);
		ShowWindow(hwnd, nCmdShow);
	}

	export void clear() {
		triangles.clear();
	}

	export bool alive() {
		msg = {};
		return GetMessage(&msg, nullptr, 0, 0);
	}

	export void render() {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	export void draw_triangle(const Triangle& _Object) {
		triangles.push_back(_Object);
	}

}