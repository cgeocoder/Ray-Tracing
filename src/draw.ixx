module;

#include <cmath>

export module draw;

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

export class Drawable {
public:

};

export class Triangle : public Drawable {
private:
	double A, B, C, D, m_Delta;
	Point m_pA, m_pB, m_pC, m_vecN;

public:
	Triangle() {};

	Triangle(const Point& _A, const Point& _B, const Point& _C) : m_pA(_A), m_pB(_B), m_pC(_C) {
		A = (m_pB.y - m_pA.y) * (m_pC.z - m_pA.z) - (m_pB.z - m_pA.z) * (m_pC.y - m_pA.y);
		B = (m_pB.z - m_pA.z) * (m_pC.x - m_pA.x) - (m_pB.x - m_pA.x) * (m_pC.z - m_pA.z);
		C = (m_pB.x - m_pA.x) * (m_pC.y - m_pA.y) - (m_pB.y - m_pA.y) * (m_pC.x - m_pA.x);
		D = -m_pA.x * A - m_pA.y * B - m_pA.z * C;

		m_vecN = { A, B, C };
		m_Delta = det3(m_pA, m_pB, m_pC);
	}

	double distance_to_plane_eq(const Point& _Dir) const {
		if (C == 0.0)
			return -1.0;

		// Ax + By + Cz + D = 0
		// z = -(Ax + By + D) / C

		return -(D + A * _Dir.x + B * _Dir.y) / C;
	}

	bool in_triangle_area(const Point& _P) const {
		double delta1 = det3(_P, m_pB, m_pC);
		double delta2 = det3(m_pA, _P, m_pC);
		double delta3 = det3(m_pA, m_pB, _P);

		double alpha = delta1 / m_Delta;
		double beta = delta2 / m_Delta;
		double gamma = delta3 / m_Delta;

		// double sum = alpha + beta + gamma;
		return (alpha >= 0) && (beta >= 0) && (gamma >= 0); // && (0.8 <= sum && sum <= 1);
	}

	const Point& normal() const {
		return m_vecN;
	}
};
