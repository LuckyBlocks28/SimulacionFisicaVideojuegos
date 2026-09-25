#pragma once
#include "PxPhysicsAPI.h"

template<typename T = float>
class Vector3D {
private:
	float _x;
	float _y;
	float _z;
public:
	inline Vector3D() : _x(0), _y(0), _z(0) {};
	inline Vector3D(T x, T y, T z) : _x(x), _y(y), _z(z) {};
	inline Vector3D(const physx::PxVec3& v) : _x(v.x), _y(v.y), _z(v.z) {};

	inline float magnitud() const {
		return std::sqrtf(std::powf(_x, 2) + std::powf(_y, 2) + std::powf(_z, 2));
	};
	inline Vector3D normlize() const {
		return *this / magnitud();
	};
	
	T dot(const Vector3D& v) const {
		return (x * _v.x + _y * v._y + _z * v._z);
	};
	Vector3D cross(const Vector3D& v) const {
		return { _y * v._z - z,_z * v._x - _x,_x * v._y - _y };
	};

	inline Vector3D& operator=(const Vector3D& v) noexcept{
		_x = v._x;
		_y = v._y;
		_z = v._z;
		return *this;
	}
	
	inline Vector3D operator+(const Vector3D& v) {
		return Vector3D(_x + v._x, _y + v._y, _z + v._z);
	}
	inline Vector3D operator-(const Vector3D& v) {
		return Vector3D(_x - v._x, _y - v._y, _z - v._z);
	}
	inline Vector3D operator*(float d) {
		return Vector3D(_x * d, _y * d, _z * d);
	}
	inline Vector3D operator/(float d) const {
		return Vector3D(_x / d, _y / d, _z / d);
	}
	inline Vector3D& operator+=(const Vector3D& v) {
		_x += v._x;
		_y += v._y;
		_z += v._z;
		return *this;
	}
	inline Vector3D& operator*=(float d) {
		_x *= d;
		_y *= d;
		_z *= d;
		return *this;
	}
	template<typename U>
	inline operator Vector3D<U>() const
	{
		return { U(_x),U(_y),U(_z) };
	}
	operator physx::PxVec3() const {
		return physx::PxVec3(static_cast<float>(_x), static_cast<float>(_y), static_cast<float>(_z));
	}
};