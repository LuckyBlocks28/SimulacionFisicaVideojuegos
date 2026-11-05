#pragma once
#include "Particle.h"
#include "ParticleSystem.h"
#include "ForceEmmiter.h"
#include <vector>
#include <iostream>

class ForceSystem
{
public:
	ForceSystem() = default;
	ForceSystem(Particle* p);
	ForceSystem(ParticleSystem* p);
	ForceSystem(std::vector<Particle*> p, std::vector<ParticleSystem*> pS);

	~ForceSystem();
	void addParticle(Particle* p); 
	void addParticleSystem(ParticleSystem* pS); 
	void addForceEmmiter(ForceEmmiter* fE);

	void update(double dt);
	
protected:
	std::vector<Particle*> m_singleParticles;
	std::vector<ParticleSystem*> m_particleSystems;
	std::vector<ForceEmmiter*> m_emitters;
};