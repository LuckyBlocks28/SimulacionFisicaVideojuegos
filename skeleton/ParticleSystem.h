#pragma once
#include "Particle.h"
#include "Proyectile.h"
#include "Emissor.h"

#include <vector>

class ParticleSystem
{
private:
	std::vector<Particle*> m_particles; // Manejo por punteros para evitar duplicación/liberación prematura de RenderItem
	std::vector<Emissor*> m_emitters;   // Gestión y liberación de memoria de emisores

	Vector3D<float> m_gravity;

	// Espacio de acción (Bounding Box)
	Vector3D<float> m_minBounds;
	Vector3D<float> m_maxBounds;
	bool m_useBounds;

	// Valores por defecto para partículas generadas
	float m_defaultParticleSize;
	Vector4 m_defaultParticleColor;
	float m_defaultLifeTime;

public:
	ParticleSystem();
	ParticleSystem(Vector3D<float> gravity, Vector3D<float> minBounds, Vector3D<float> maxBounds);
	~ParticleSystem();

	// Gestión de emisores
	void addEmissor(Emissor* emitter);
	void removeEmissor(Emissor* emitter);

	// Creación e inserción de partículas
	void addParticle(Particle* particle);
	void addParticle(Vector3D<float> pos, Vector3D<float> vel, float lifeTime = 5.0f, float size = 1.0f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	void addParticle(Vector3D<float> pos, Vector3D<float> vel, Emissor::TYPE type = Emissor::PARTICLE, float lifeTime = 4.0f, float size = 0.5f, Vector4 color = Vector4(1.0f, 0.5f, 0.0f, 1.0f));

	// Bucle principal
	void update(double dt);

	// Getters y Setters
	inline void setGravity(const Vector3D<float>& gravity) noexcept { m_gravity = gravity; }
	inline Vector3D<float> getGravity() const noexcept { return m_gravity; }

	inline void setBounds(const Vector3D<float>& minB, const Vector3D<float>& maxB) noexcept {
		m_minBounds = minB;
		m_maxBounds = maxB;
		m_useBounds = true;
	}

	inline void setDefaultParticleProperties(float size, Vector4 color, float lifeTime) noexcept {
		m_defaultParticleSize = size;
		m_defaultParticleColor = color;
		m_defaultLifeTime = lifeTime;
	}

	inline const std::vector<Particle*>& getParticles() const noexcept { return m_particles; }
	inline size_t getParticleCount() const noexcept { return m_particles.size(); }

private:
	bool isOutOfBounds(const physx::PxVec3& pos) const;
	void clearDeadParticles();
};

