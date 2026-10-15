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
	long width, height;
	uint32_t* frame_buffer;
	std::vector<Drawable*> objects;

	constexpr double z_cam = 900.0;

	double part_w, sin_a;
	double horizontal_angle_step;
	double part_h, sin_b;
	double vertical_angle_step;

	void update_screen_param(long new_width, long new_height) {
		part_w = new_width / 2.0;
		double l = sqrt(part_w * part_w + z_cam * z_cam);
		sin_a = -part_w / l;

		horizontal_angle_step = -2.0 * sin_a / new_width;

		part_h = new_height / 2.0;
		double t = sqrt(part_h * part_h + z_cam * z_cam);
		sin_b = part_h / t;

		vertical_angle_step = -2.0 * sin_b / new_height;
	}

	export void update() {
		double horizontal_angle = sin_a;
		for (size_t x = 0; x < width; ++x, horizontal_angle += horizontal_angle_step) {
			double nx = x - part_w;

			double vertical_angle = sin_b;
			for (size_t y = 0; y < height; ++y, vertical_angle += vertical_angle_step) {
				double ny = part_h - y;

				Vec3 ray = Vec3(horizontal_angle, vertical_angle, 1.0).norm();

				Vec3 draw_norm = {};
				Drawable* draw_obj = nullptr;
				double near_dist = 1000.0;

				for (const auto& obj : objects) {
					double dist = obj->distance(ray);

					if (dist >= 0.0 && near_dist > dist) {
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

					frame_buffer[x + y * width] = (draw_obj->color() * (255.0 * cos_n_ay)).get();
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

		case WM_SIZE: {
			int old_width = width, old_height = height;

			client_rect;
			GetClientRect(hwnd, &client_rect);
			width = client_rect.right - client_rect.left;
			height = client_rect.bottom - client_rect.top;

			if (old_width != width || old_height != height) {
				update_screen_param(width, height);

				if (frame_buffer)
					delete[] frame_buffer;

				frame_buffer = new uint32_t[width * height + 1];
			}

			break;
		}
		case WM_PAINT: {
			update();

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

	export void draw(Drawable* _Object) {
		objects.push_back(_Object);
	}
}