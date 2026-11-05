#pragma once
#include "Vector3D.h"
#include "Geometry.h"
#include "Particle.h"

class ForceSystem;

class ForceEmmiter
{
public:
	ForceEmmiter(ForceSystem* fS, Vector3D<float> pos, Vector3D<float> dir, Geometry* g, float force, bool active = true)
		: m_FS(fS), m_position(pos), m_direction(dir), m_geom(g), m_force(force), m_active(active) {
	}

	virtual ~ForceEmmiter() = default;

	// Método para actualizar el estado dinámico del emisor (tiempo, radio, etc.)
	virtual void update(double dt) {}

	// Evalúa y aplica la fuerza a la partícula si está dentro del área
	virtual void addForce(Particle* p, double dt) = 0;

	inline bool isActive() const noexcept { return m_active; }
	inline void setActive(bool a) noexcept { m_active = a; }

	inline Vector3D<float>& getPos() noexcept { return m_position; }
	inline Vector3D<float>& getDir() noexcept { return m_direction; }

protected:
	ForceSystem* m_FS;
	Geometry* m_geom;
	Vector3D<float> m_position, m_direction;
	float m_force;
	bool m_active;
};