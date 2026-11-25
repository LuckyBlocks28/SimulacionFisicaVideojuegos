#pragma once
#include "ForceEmmiter.h"


class GravityForceEmissor : public ForceEmmiter
{
public:
	enum class GravityType { 
		Directional, // Aplica gravedad en la dirección m_direction
		Point        // Atrae la partícula hacia m_position (Centro de gravedad)
	}; 

	GravityForceEmissor(Vector3D<float> pos, Vector3D<float> dir, Geometry* g, float gravityMagnitude,bool active, GravityType type = GravityType::Directional);
	virtual ~GravityForceEmissor() = default;

	virtual void addForce(Particle* p) override;

	inline void setGravityType(GravityType type) noexcept { m_type = type; }

private:
	GravityType m_type;
};

