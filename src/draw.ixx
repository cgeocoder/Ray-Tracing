module;

#include <cmath>
#include <algorithm>
#include <windows.h>

export module draw;
import matrix;

export constexpr double PI = 3.14159265358979323846;

export Vec3 operator*(double _Scalar, const Vec3& _Vec3) {
	return { _Vec3.x * _Scalar, _Vec3.y * _Scalar, _Vec3.z * _Scalar };
}

export double det3(const Vec3& _A, const Vec3& _B, const Vec3& _C) {
	return _A.x * (_B.y * _C.z - _C.y * _B.z) - _B.x * (_A.y * _C.z - _C.y * _A.z) + _C.x * (_A.y * _B.z - _B.y * _A.z);
}

export double vector_length(const Vec3& _P) {
	return sqrt(_P.x * _P.x + _P.y * _P.y + _P.z * _P.z);
}

export double scalar_product(const Vec3& _P1, const Vec3& _P2) {
	return _P1.x * _P2.x + _P1.y * _P2.y + _P1.z * _P2.z;
}

export class Color {
public:
	double r, g, b, a;
	Color(double r, double g, double b)
		: r(r), g(g), b(b), a(1.0) {}

	Color operator*(double _Scalar) const {
		return {r * _Scalar, g * _Scalar, b * _Scalar };
	}

	inline auto get() const {
		return RGB(r, g, b);
	}
};

export class Drawable {
protected:
	Vec3 m_Origin;
	Color m_Color;

public:
	Drawable(const Vec3& _Origin, const Color& _Color)
		: m_Origin(_Origin), m_Color(_Color) {};

	virtual double distance(const Vec3&) const = 0;
	virtual Vec4 normal(const Vec3&, double) const = 0;

	const Color& color() const {
		return m_Color;
	}

	const Vec3& origin() const {
		return m_Origin;
	}
};

export class Sphere : public Drawable {
private:
	double c;

public:
	double m_R;

	Sphere(double _Radius, const Vec3& _Origin, const Color& _Color) 
		: m_R(_Radius), Drawable(_Origin, _Color) {
		c = dot(m_Origin, m_Origin) - m_R * m_R;
	}

	virtual double distance(const Vec3& _Ray) const override {
		double a2 = 2.0 * dot(_Ray, _Ray);
		double b = -2.0 * dot(_Ray, m_Origin);

		double D = b * b - 2.0 * a2 * c;

		if (D < 0)
			return -1.0;

		double Dsrt = sqrt(D);
		double l1 = (-b + Dsrt) / a2;
		double l2 = (-b - Dsrt) / a2;

		return min(l1, l2);
	}

	virtual Vec4 normal(const Vec3& _Ray, double _Distance) const override {
		if (_Distance == -1.0)
			return { 0.0, 0.0, 0.0, 0.0 };

		return (_Ray * _Distance - m_Origin).norm();
	}
};

export class Plane : public Drawable {
private:
	double m_A, m_B, m_C, m_D;
	Vec3 m_Norm;

public:
	Plane(const Vec3& _P1, const Vec3& _P2, const Vec3& _P3, const Vec3& _Origin, const Color& _Color)
		: Drawable(_Origin, _Color) {

		m_A = (_P2.y - _P1.y) * (_P3.z - _P1.z) - (_P2.z - _P1.z) * (_P3.y - _P1.y);
		m_B = (_P2.z - _P1.z) * (_P3.x - _P1.x) - (_P2.x - _P1.x) * (_P3.z - _P1.z);
		m_C = (_P2.x - _P1.x) * (_P3.y - _P1.y) - (_P2.y - _P1.y) * (_P3.x - _P1.x);
		m_D = -(_P1.x * m_A + _P1.y * m_B + _P1.z * m_C);

		m_Norm = Vec3(m_A, m_B, m_C).norm();
	}

	virtual double distance(const Vec3& _Ray) const override {
		double sprod = (m_A * _Ray.x + m_B * _Ray.y + m_C);

		if (sprod != 0.0) {
			return -m_D / sprod;
		}

		return -1.0;
	}

	virtual Vec4 normal(const Vec3& _Ray, double _Distance) const override {
		if (_Distance == -1.0)
			return { 0.0, 0.0, 0.0, 0.0 };

		return (Vec4)m_Norm;
	}
};