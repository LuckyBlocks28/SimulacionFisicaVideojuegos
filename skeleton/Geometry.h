#pragma once
#include "Vector3D.h"
#include "Particle.h"

#include <cmath>
#include <limits>
#include <numbers>
#include <algorithm>
#include <random>

class Geometry
{
public:
	Geometry(Vector3D<float> pos, Vector3D<float> dir): m_position(pos),m_direction(dir) {};
	virtual ~Geometry() = default;

    virtual Vector3D<float> calculateDirection(float factorRadius) = 0;
    virtual Vector3D<float> calculateNewPosition(float factorRadius) = 0;

    virtual bool inArea(Particle* p) = 0;

	inline Vector3D<float>& getPos() noexcept { return m_position; }
	inline Vector3D<float>& getDir() noexcept { return m_direction; }

protected:
    const float TWO_PI = 2.0f * std::numbers::pi_v<float>; 
	
    Vector3D<float> m_position, m_direction;
    
    float x, y, z;
    Vector3D<float> offset = Vector3D<float>(0.0f, 0.0f, 0.0f);

	void rotatePoint(Vector3D<float>& position, const Vector3D<float>& u, const Vector3D<float>& v, const Vector3D<float>& center);
};

inline void Geometry::rotatePoint(Vector3D<float>& position, const Vector3D<float>& u, const Vector3D<float>& v, const Vector3D<float>& center) {
	Vector3D<float> uNorm = u;
	Vector3D<float> vNorm = v;
	float uMag = uNorm.magnitud();
	float vMag = vNorm.magnitud();
	if (uMag < 0.0001f || vMag < 0.0001f) return;

	uNorm = uNorm * (1.0f / uMag);
	vNorm = vNorm * (1.0f / vMag);

	float dot = std::clamp(uNorm.dot(vNorm), -1.0f, 1.0f);
	Vector3D<float> p = position - center;

	// Vectores prácticamente idénticos
	if (dot > 0.9999f) {
		position = p + center;
		return;
	}

	// Vectores opuestos (180 grados)
	if (dot < -0.9999f) {
		Vector3D<float> axis(1.0f, 0.0f, 0.0f);
		if (std::abs(uNorm.getY()) > 0.8f) {
			axis = Vector3D<float>(0.0f, 0.0f, 1.0f);
		}
		Vector3D<float> k(
			uNorm.getY() * axis.getZ() - uNorm.getZ() * axis.getY(),
			uNorm.getZ() * axis.getX() - uNorm.getX() * axis.getZ(),
			uNorm.getX() * axis.getY() - uNorm.getY() * axis.getX()
		);
		k.normalize();
		Vector3D<float> pRot = k * (2.0f * k.dot(p)) - p;
		position = pRot + center;
		return;
	}

	// Caso general usando rotación de Rodrigues (Eje k = u x v)
	Vector3D<float> k(
		uNorm.getY() * vNorm.getZ() - uNorm.getZ() * vNorm.getY(),
		uNorm.getZ() * vNorm.getX() - uNorm.getX() * vNorm.getZ(),
		uNorm.getX() * vNorm.getY() - uNorm.getY() * vNorm.getX()
	);
	k.normalize();

	float angle = std::acos(dot);
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	Vector3D<float> kCrossP(
		k.getY() * p.getZ() - k.getZ() * p.getY(),
		k.getZ() * p.getX() - k.getX() * p.getZ(),
		k.getX() * p.getY() - k.getY() * p.getX()
	);

	Vector3D<float> pRot = (p * cosA) + (kCrossP * sinA) + (k * (k.dot(p) * (1.0f - cosA)));
	position = pRot + center;
}