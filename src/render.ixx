module;

#include <windows.h>
#include <vector>

export module render;
import draw;

namespace render {

	MSG msg = {};
	std::vector<Triangle> triangles;
	RECT client_rect;
	size_t width;
	size_t height;
	uint32_t* frame_buffer;

	void update() {

		for (size_t x = 0; x < width; ++x) {
			double nx = x - width / 2.0;

			for (size_t y = 0; y < height; ++y) {
				double ny = height / 2.0 - y;

				Point R = { nx, ny, 1 };

				double min_dist = 10'000.0;
				Triangle* nearest = nullptr;

				for (auto& triangle : triangles) {
					double dist = triangle.distance_to_plane_eq(R);
					
					if (dist < 1'000.0) {

						if (min_dist > dist && triangle.in_triangle_area({ R.x, R.y, dist })) {
							min_dist = dist;
							nearest = &triangle;
						}
					}
				}

				if (min_dist < 1'000.0) {
					R.z = min_dist;

					const Point& norm = nearest->normal();
					
					// div to min_dist because R is not a vector from (0, 0, 0)
					double cos_NR = abs(scalar_product(R, norm)
						/ (min_dist * vector_length(norm)));

					frame_buffer[x + y * width] = RGB(cos_NR * 255, cos_NR * 50, cos_NR * 70);
				}
			}
		}
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_CREATE:
			SetTimer(hwnd, 1, 16, nullptr);
			return 0;

		case WM_TIMER:
			InvalidateRect(hwnd, nullptr, TRUE);
			return 0;
		case WM_PAINT: {
			int old_width = width, old_height = height;

			client_rect;
			GetClientRect(hwnd, &client_rect);
			width = client_rect.right - client_rect.left;
			height = client_rect.bottom - client_rect.top;

			if (old_width != width || old_height != height) {
				if (frame_buffer)
					delete[] frame_buffer;

				frame_buffer = new uint32_t[width * height + 1];
			}

			// __debugbreak();

			update();

			// __debugbreak();

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			
			BITMAPINFO bmi = {};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = (long long)height;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			StretchDIBits(
				hdc,
				0, 0, width, height,
				0, 0, width, height,
				frame_buffer,
				&bmi,
				DIB_RGB_COLORS,
				SRCCOPY
			);

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
		if (frame_buffer) {
			for (size_t i = 0; i < width * height; ++i)
				frame_buffer[i] = 0;
		}

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