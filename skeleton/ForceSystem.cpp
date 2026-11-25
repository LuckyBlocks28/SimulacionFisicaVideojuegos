#include "ForceSystem.h"

ForceSystem::ForceSystem(Particle* p)
{
	if (p) m_singleParticles.push_back(p);
}

ForceSystem::ForceSystem(ParticleSystem* pS)
{
	if (pS) m_particleSystems.push_back(pS);
}

ForceSystem::ForceSystem(std::vector<Particle*> p): m_singleParticles(p)
{
}
ForceSystem::ForceSystem(std::vector<ParticleSystem*> pS) : m_particleSystems(pS)
{
}

ForceSystem::ForceSystem(std::vector<Particle*> p, std::vector<ParticleSystem*> pS): m_singleParticles(p), m_particleSystems(pS)
{
}

ForceSystem::~ForceSystem()
{
	// No eliminar los punteros de m_particles / m_particleSystems ya que pertenecen a ParticleSystem o a la escena.
	m_singleParticles.clear();
	m_particleSystems.clear();

	// Solo liberar los emisores de fuerza propios asignados a este sistema
	for (auto* emitter : m_emitters) {
		if (emitter) {
			delete emitter;
		}
	}
	m_emitters.clear();
}

void ForceSystem::addParticle(Particle* p) 
{
	if (p) m_singleParticles.push_back(p);
}
void ForceSystem::addParticleSystem(ParticleSystem* pS)
{
	if (pS) m_particleSystems.push_back(pS);
}
void ForceSystem::addForceEmmiter(ForceEmmiter* fE)
{
	if (fE) m_emitters.push_back(fE);
}

void ForceSystem::update(double dt)
{
	for (auto* emitter : m_emitters)
	{
		if (!emitter) continue;

		// 1. Actualizar el estado interno del emisor (p. ej. tiempo t de la explosión)
		emitter->update(dt);

		if (!emitter->isActive()) continue;

		// 2. Aplicar fuerzas a partículas individuales
		for (auto* p : m_singleParticles) {
			if (p && p->isActive()) {
				emitter->addForce(p);
			}
		}

		// 3. Aplicar fuerzas a partículas pertenecientes a ParticleSystems
		for (auto* ps : m_particleSystems) {
			if (!ps) continue;
			for (auto* p : ps->getParticles()) {
				if (p && p->isActive()) {
					emitter->addForce(p);
				}
			}
		}
	}
}