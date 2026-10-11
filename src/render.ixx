module;

#include <windows.h>
#include <vector>
#include <memory>

export module render;
import draw;
import matrix;

namespace render {
	MSG msg = {};
	RECT client_rect;
	size_t width;
	size_t height;
	uint32_t* frame_buffer;
	std::vector<std::shared_ptr<Drawable>> objects;

	export void update() {
		double fi = -PI / 8.0;
		double fi_step = 2.0 * abs(fi) / width;

		for (size_t x = 0; x < width; ++x, fi += fi_step) {
			double psi = PI / 10.0;
			double psi_step = -2.0 * abs(psi) / height;

			double nx = x - width / 2.0;

			for (size_t y = 0; y < height; ++y, psi += psi_step) {
				double ny = height / 2.0 - y;

				Vec3 ray = { fi, psi, 1.0 };
				ray = ray * (1.0 / ray.len());

				Vec4 draw_norm = {};
				std::shared_ptr<Drawable> draw_obj = nullptr;
				double near_dist = 10'000.0;

				for (const auto& obj : objects) {
					double dist = obj->distance(ray);

					if (near_dist > dist) {
						Vec4 normal = obj->normal(ray, dist);

						if (normal.w == 1.0) {
							draw_norm = normal;
							near_dist = dist;
							draw_obj = obj;
						}
					}
				}

				if (draw_obj) {
					double cos_n_ay = abs(dot(ray, draw_norm));

					frame_buffer[x + y * width] = RGB(
						draw_obj->color().r * cos_n_ay,
						draw_obj->color().g * cos_n_ay,
						draw_obj->color().b * cos_n_ay
					);
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
			bmi.bmiHeader.biHeight = -(long long)height;
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
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
			nullptr, nullptr, hInstance, nullptr);
		ShowWindow(hwnd, nCmdShow);
	}

	export void clear() {
		if (frame_buffer) {
			for (size_t i = 0; i < width * height; ++i)
				frame_buffer[i] = 0;
		}

		objects.clear();
	}

	export bool alive() {
		msg = {};
		return GetMessage(&msg, nullptr, 0, 0);
	}

	export void render() {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	export void draw(std::shared_ptr<Drawable> _Object) {
		objects.push_back(_Object);
	}
}