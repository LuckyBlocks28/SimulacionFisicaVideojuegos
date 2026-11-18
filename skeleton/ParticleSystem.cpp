#include "ParticleSystem.h"
#include <algorithm>
#include "ForceSystem.h"

ParticleSystem::ParticleSystem()
	: m_gravity(0.0f, -9.8f, 0.0f), m_minBounds(-100.0f, -100.0f, -100.0f),
	m_maxBounds(100.0f, 100.0f, 100.0f), m_useBounds(true),
	m_defaultParticleSize(0.5f), m_defaultParticleColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	m_defaultLifeTime(4.0f), m_type(Particle::TYPE::WITHOUT_GRAVITY)
{
}

ParticleSystem::ParticleSystem(Vector3D<float> gravity, Vector3D<float> minBounds, Vector3D<float> maxBounds, Particle::TYPE typeP)
	: m_gravity(gravity), m_minBounds(minBounds),
	m_maxBounds(maxBounds), m_useBounds(true),
	m_defaultParticleSize(0.5f), m_defaultParticleColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	m_defaultLifeTime(4.0f), m_type(typeP)
{
}

ParticleSystem::~ParticleSystem()
{
	// Liberar emisores
	for (Emissor* emitter : m_emitters) {
		delete emitter;
	}
	m_emitters.clear();

	// Liberar partículas activas e inactivas
	for (Particle* p : m_particles) {
		if (p) {
			p->deactivate(); // Libera el RenderItem de PhysX
			delete p;
		}
	}
	m_particles.clear();
}

void ParticleSystem::addEmissor(Emissor* emitter)
{
	if (emitter != nullptr) {
		m_emitters.push_back(emitter);
	}
}

void ParticleSystem::removeEmissor(Emissor* emitter)
{
	auto it = std::find(m_emitters.begin(), m_emitters.end(), emitter);
	if (it != m_emitters.end()) {
		for (Particle* p : m_particles) {
			if (p && p->getEmissor() == *it) {
				p->setEmissor(nullptr);
			}
		}
		delete* it;
		m_emitters.erase(it);
	}
}

void ParticleSystem::addParticle(Particle* particle)
{
	if (particle != nullptr) {
		m_particles.push_back(particle);
	}
}

Particle* ParticleSystem::addParticle(Vector3D<float> pos, Vector3D<float> vel, Particle::TYPE typeP, float mass, float size, float lifeTime,Vector4 color)
{
	Particle* p = new Particle(pos, vel, size, mass, typeP, color);
	p->setAcceleration(m_gravity);
	p->setLifeTime(lifeTime);
	m_particles.push_back(p);
	
	return p;
}

bool ParticleSystem::isOutOfBounds(const physx::PxVec3& pos) const
{
	if (!m_useBounds) return false;

	return (pos.x < m_minBounds.getX() || pos.x > m_maxBounds.getX() ||
		pos.y < m_minBounds.getY() || pos.y > m_maxBounds.getY() ||
		pos.z < m_minBounds.getZ() || pos.z > m_maxBounds.getZ());
}

void ParticleSystem::update(double dt)
{
	// Actualizar emisores para generar nuevas partículas
	for (Emissor* emitter : m_emitters) {
		if (emitter && emitter->isActive()) {
			emitter->update(dt);
		}
	}

	// Actualizar cada partícula activa
	for (Particle* p : m_particles) {
		if (!p || !p->isActive()) continue;

		// 1. Actualizar integración física y restar tiempo de vida
		p->update(dt);
		if (!p->isActive()) continue;

		// 2. Mantener dentro de la geometría de su propio emisor si está activo
		Emissor* owner = p->getEmissor();
		if (owner && owner->isKeepingInside() && owner->getGeometry()) {
			owner->getGeometry()->containParticle(p);
		}

		// 3. Comprobar límites globales del sistema
		if (isOutOfBounds(p->getPos())) {
			p->deactivate();
		}
	}

	// Eliminar partículas desactivadas para liberar memoria
	clearDeadParticles();
}

void ParticleSystem::clearDeadParticles()
{
	auto it = m_particles.begin();
	while (it != m_particles.end()) {
		Particle* p = *it;
		if (p == nullptr || !p->isActive()) {
			if (p) {
				delete p;
			}
			it = m_particles.erase(it);
		}
		else {
			++it;
		}
	}
}