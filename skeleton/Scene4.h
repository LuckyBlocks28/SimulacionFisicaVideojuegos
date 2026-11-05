#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "Emissor.h"

#include "CilinderGeometry.h"
#include "SphereGeometry.h"
#include "BoxGeometry.h"
#include "PrismGeometry.h"

#include "ForceSystem.h"
#include "GravityForceEmissor.h"
#include "AirForceEmissor.h"
#include "TyphoonForceEmissor.h"
#include "ExplosionForceEmissor.h"

#include <random>

class Scene4 :
    public Scene
{
public:
    Scene4(std::string name) : Scene(std::move(name)) {};
    void init() override;
    void update(double dt) override;
    void keyPress(unsigned char key, const physx::PxTransform& camera) override;
    void cleanup() override;
private:
    ParticleSystem* m_particleSystem = nullptr;
    ForceSystem* m_ForceSystem = nullptr;
    ExplosionForceEmissor* m_explosionEmissor = nullptr;
};

