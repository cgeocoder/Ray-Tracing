module;

#include <cmath>
#include <algorithm>

export module draw;
import matrix;

export constexpr double PI = 3.14159265358979323846;

export class Point {
public:
	double x, y, z;
	Point() : x(0.0), y(0.0), z(0.0) {}
	Point(double x, double y, double z) : x(x), y(y), z(z) {}

	Point operator*(double _Scalar) {
		return { x * _Scalar, y * _Scalar, z * _Scalar };
	}
};

export Point operator*(double _Scalar, const Point& _Point) {
	return { _Point.x * _Scalar, _Point.y * _Scalar, _Point.z * _Scalar };
}

export double det3(const Point& _A, const Point& _B, const Point& _C) {
	return _A.x * (_B.y * _C.z - _C.y * _B.z) - _B.x * (_A.y * _C.z - _C.y * _A.z) + _C.x * (_A.y * _B.z - _B.y * _A.z);
}

export double vector_length(const Point& _P) {
	return sqrt(_P.x * _P.x + _P.y * _P.y + _P.z * _P.z);
}

export double scalar_product(const Point& _P1, const Point& _P2) {
	return _P1.x * _P2.x + _P1.y * _P2.y + _P1.z * _P2.z;
}

export class Color {
public:
	unsigned char r, g, b, a;
	Color(unsigned char r, unsigned char g, unsigned char b) 
		: r(r), g(g), b(b), a(255) {}
};

export class Drawable {
protected:
	Point m_O;
	Color m_Color;

public:
	Drawable(const Point& _Origin, const Color& _Color)
		: m_O(_Origin), m_Color(_Color) {};

	virtual double distance(const Vec3&) const = 0;
	virtual Vec4 normal(const Vec3&, double) const = 0;

	const Color& color() const {
		return m_Color;
	}
};

export class Sphere : public Drawable {
public:
	double m_R;

	Sphere(double _Radius, const Point& _Origin, const Color& _Color) 
		: m_R(_Radius), Drawable(_Origin, _Color) {}

	virtual double distance(const Vec3& _Ray) const override {
		double a = _Ray.x * _Ray.x + _Ray.y * _Ray.y + _Ray.z * _Ray.z;
		double b = -2.0 * (_Ray.x * m_O.x + _Ray.y * m_O.y + _Ray.z * m_O.z);
		double c = m_O.x * m_O.x + m_O.y * m_O.y + m_O.z * m_O.z - m_R * m_R;

		double D = b * b - 4.0 * a * c;

		if (D < 0)
			return -1.0;

		double l1 = (-b + sqrt(D)) / (2.0 * a);
		double l2 = (-b - sqrt(D)) / (2.0 * a);

		return std::min(l1, l2);
	}

	virtual Vec4 normal(const Vec3& _Ray, double _Distance) const override {
		if (_Distance == -1.0)
			return { 0.0, 0.0, 0.0, 0.0 };

		Vec3 O = { m_O.x, m_O.y, m_O.z };
		Vec3 n = _Ray * _Distance - O;
		n = n * (1.0 / n.len());
		return { n.x, n.y, n.z, 1.0 };
	}
};

