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
	std::vector<Drawable*> objects;

	constexpr double x_angle = PI / 8.0 * 2;
	constexpr double y_angle = PI / 8.0 * 2;

	double start_x_agnel = -PI / 8.0;
	double start_y_agnel = PI / 8.0;

	double start_x_agnel_sin = -PI / 8.0;
	double start_y_agnel_sin = PI / 8.0;

	double start_x_agnel_total = PI / 8.0 * 2;
	double start_y_agnel_total = PI / 8.0 * 2;

	double start_y_agnel_total_sin = PI / 8.0 * 2;
	double start_x_agnel_total_sin = PI / 8.0 * 2;

	export void camera_rotate_x_left(double _Angle) {
		start_x_agnel -= _Angle;

		sin(start_x_agnel_sin + x_angle);
		start_x_agnel_total_sin = sin(start_x_agnel + x_angle);
	}

	export void camera_rotate_x_right(double _Angle) {
		start_x_agnel += _Angle;
		start_x_agnel_sin = sin(start_x_agnel);
		start_x_agnel_total_sin = sin(start_x_agnel + x_angle);
	}

	export void camera_rotate_y_up(double _Angle) {
		start_y_agnel += _Angle;
		start_y_agnel_sin = sin(start_y_agnel);
		start_y_agnel_total_sin = sin(start_y_agnel + y_angle);
	}

	export void camera_rotate_y_down(double _Angle) {
		start_y_agnel -= _Angle;
		start_y_agnel_sin = sin(start_y_agnel);
		start_y_agnel_total_sin = sin(start_y_agnel + y_angle);
	}

	export void update() {
		double x_angle_sin = start_x_agnel_sin;
		double la_step = (start_x_agnel_total_sin - start_x_agnel_sin) / width;

		// __debugbreak();

		for (size_t x = 0; x < width; ++x, x_angle_sin += la_step) {
			double nx = x - width / 2.0;

			double y_angle_sin = start_y_agnel_sin;
			double ta_step = -(start_y_agnel_total_sin - start_y_agnel_sin) / height;

			for (size_t y = 0; y < height; ++y, y_angle_sin += ta_step) {
				double ny = height / 2.0 - y;

				Vec3 ray = { x_angle_sin, y_angle_sin, 1.0 };
				ray = ray * (1.0 / ray.len());

				Vec4 draw_norm = {};
				Drawable* draw_obj = nullptr;
				double near_dist = 100.0;

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

					frame_buffer[x + y * width] = (draw_obj->color() * 255.0 * cos_n_ay).get();
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

	export void draw(Drawable* _Object) {
		objects.push_back(_Object);
	}
}