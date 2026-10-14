#include "ParticleSystem.h"
#include <algorithm>

ParticleSystem::ParticleSystem(): m_gravity(0.0f, -9.8f, 0.0f), m_minBounds(-100.0f, -100.0f, -100.0f), m_maxBounds(100.0f, 100.0f, 100.0f), m_useBounds(true), m_defaultParticleSize(0.5f),
    m_defaultParticleColor(Vector4(1.0f, 0.5f, 0.0f, 1.0f)), m_defaultLifeTime(4.0f)
{
}

ParticleSystem::ParticleSystem(Vector3D<float> gravity, Vector3D<float> minBounds, Vector3D<float> maxBounds): m_gravity(gravity), m_minBounds(minBounds), m_maxBounds(maxBounds), m_useBounds(true),
	m_defaultParticleSize(0.5f), m_defaultParticleColor(Vector4(1.0f, 0.5f, 0.0f, 1.0f)), m_defaultLifeTime(4.0f)
{
}

ParticleSystem::~ParticleSystem()
{
	// Liberar la memoria de todas las partículas activas/inactivas
	for (Particle* p : m_particles) {
		if (p) {
			p->deactivate();
			delete p;
		}
	}
	m_particles.clear();

	// Liberar la memoria de los emisores registrados
	for (Emissor* emitter : m_emitters) {
		delete emitter;
	}
	m_emitters.clear();
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

void ParticleSystem::addParticle(Vector3D<float> pos, Vector3D<float> vel, float lifeTime, float size, Vector4 color)
{
	Particle* p = new Particle(pos, vel, size, color);
	p->setAcceleration(m_gravity); // Aplica la gravedad (-10 en Y)
	p->setLifeTime(lifeTime);
	m_particles.push_back(p);
}

void ParticleSystem::addParticle(Vector3D<float> pos, Vector3D<float> vel, Emissor::TYPE type, float lifeTime, float size, Vector4 color)
{
	Particle* p = nullptr;

	if (type == Emissor::PROYECTILE) {
		// Instancia la clase derivada Proyectile
		float defaultMass = 1.0f;
		p = new Proyectile(pos, vel, size, defaultMass, color);
	}
	else {
		// Instancia la clase base Particle
		p = new Particle(pos, vel, size, color);
		p->setAcceleration(m_gravity); // Aplica gravedad global si es partícula estándar
	}

	if (p != nullptr) {
		p->setLifeTime(lifeTime);
		m_particles.push_back(p);
	}
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
	//Actualizar emisores para generar nuevas partículas
	for (Emissor* emitter : m_emitters) {
		if (emitter && emitter->isActive()) {
			emitter->update(dt);
		}
	}

	//Actualizar cada partícula activa
	for (Particle* p : m_particles) {
		if (!p || !p->isActive()) continue;

		// Actualizar integración física y tiempo de vida
		p->update(dt);

		// Comprobar condición 2: Abandona el espacio de acción
		if (isOutOfBounds(p->getPos())) {
			p->deactivate();
		}
	}

	//Eliminar partículas desactivadas para liberar memoria
	clearDeadParticles();
}

void ParticleSystem::clearDeadParticles()
{
	auto it = m_particles.begin();
	while (it != m_particles.end()) {
		Particle* p = *it;
		if (p == nullptr || !p->isActive()) {
			if (p) {
				delete p; // Llama a ~Particle() liberando el RenderItem
			}
			it = m_particles.erase(it);
		}
		else {
			++it;
		}
	}
}
