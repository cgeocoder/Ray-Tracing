module;

#include <corecrt_math.h>

export module matrix;

export class Vec4 {
public:
	double x, y, z, w;

	inline Vec4() {}
	inline Vec4(const Vec4& _Copy) {
		x = _Copy.x;
		y = _Copy.y;
		z = _Copy.z;
		w = _Copy.w;
	}

	inline Vec4(double X, double Y, double Z, double W) {
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
};

export class Vec3 {
public:
	double x, y, z;

	inline Vec3() {}

	inline Vec3(double X, double Y, double Z) {
		x = X;
		y = Y;
		z = Z;
	}

	inline Vec3(const Vec4& _V4) {
		x = _V4.x;
		y = _V4.y;
		z = _V4.z;
	}

	inline Vec3 operator-(const Vec3& _Right) const {
		return { x - _Right.x, y - _Right.y, z - _Right.z };
	}

	inline Vec3 operator+(const Vec3& _Right) const {
		return { x + _Right.x, y + _Right.y, z + _Right.z };
	}

	inline Vec3 operator*(double _Right) {
		return { x * _Right, y * _Right, z * _Right };
	}

	inline Vec3 operator*(double _Right) const {
		return { x * _Right, y * _Right, z * _Right };
	}

	inline double len() const {
		return sqrt(x * x + y * y + z * z);
	}

	inline Vec3 norm() const {
		return (*this) * (1.0 / len());
	}

	operator Vec4() const {
		return Vec4(x, y, z, 1.0);
	}
};

export inline Vec3 cross(const Vec3& _V1, const Vec3& _V2) {
	return { _V1.y * _V2.z - _V1.z * _V2.y, -_V1.x * _V2.z + _V1.z * _V2.x, _V1.x * _V2.y - _V1.y * _V2.x };
}

export inline double dot(const Vec3& _V1, const Vec3& _V2) {
	return _V1.x * _V2.x + _V1.y * _V2.y + _V1.z * _V2.z;
}

export class Matrix3x3 {
private:
	double m_Data[3][3] = { 0 };

public:
	Matrix3x3() {};
	Matrix3x3(double _Scalar) {
		m_Data[0][0] = _Scalar;
		m_Data[1][1] = _Scalar;
		m_Data[2][2] = _Scalar;
	}

	// no validation!!!
	inline double& operator()(int x, int y) {
		return m_Data[x][y];
	}

	Matrix3x3 operator*(const Matrix3x3& _Right) {
		Matrix3x3 res;

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				res(j, i) = m_Data[j][0] * _Right.m_Data[0][i]
					+ m_Data[j][1] * _Right.m_Data[1][i]
					+ m_Data[j][2] * _Right.m_Data[2][i];
			}
		}

		return res;
	}

	Vec3 operator*(const Vec3& _Right) {
		Vec3 res;

		res.x = m_Data[0][0] * _Right.x
			+ m_Data[0][1] * _Right.y
			+ m_Data[0][2] * _Right.z;

		res.y = m_Data[1][0] * _Right.x
			+ m_Data[1][1] * _Right.y
			+ m_Data[1][2] * _Right.z;

		res.z = m_Data[2][0] * _Right.x
			+ m_Data[2][1] * _Right.y
			+ m_Data[2][2] * _Right.z;

		return res;
	}
};

export Matrix3x3 translate(const Matrix3x3& _Mat, const Vec3& _Vec) {
	Matrix3x3 res = _Mat;
	res(0, 3) += _Vec.x;
	res(1, 3) += _Vec.y;
	res(2, 3) += _Vec.z;

	return res;
}

export Matrix3x3 scale(const Matrix3x3& _Mat, const Vec3& _Vec) {
	Matrix3x3 res = _Mat;
	res(0, 0) *= _Vec.x;
	res(1, 1) *= _Vec.y;
	res(2, 2) *= _Vec.z;

	return res;
}

export Matrix3x3 rotate_x(double _Angle) {
	Matrix3x3 res(1.0);
	res(1, 1) = res(2, 2) = cos(_Angle);
	res(1, 2) = -(res(2, 1) = sin(_Angle));
	return res;
}

export Matrix3x3 rotate_y(double _Angle) {
	Matrix3x3 res(1.0);
	res(0, 0) = res(2, 2) = cos(_Angle);
	res(2, 0) = -(res(0, 2) = sin(_Angle));
	return res;
}

export Matrix3x3 rotate_z(double _Angle) {
	Matrix3x3 res(1.0);
	res(0, 0) = res(1, 1) = cos(_Angle);
	res(1, 0) = -(res(0, 1) = sin(_Angle));
	return res;
}