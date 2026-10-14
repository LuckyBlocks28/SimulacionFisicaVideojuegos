#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "Emissor.h"

#include <algorithm>

class Scene3 :
    public Scene
{
public:
    Scene3(std::string name) : Scene(std::move(name)) {};
    void init() override;
    void update(double dt) override;
    void keyPress(unsigned char key, const physx::PxTransform& camera) override;
    void cleanup() override;
private:
    ParticleSystem* m_particleSystem = nullptr;
    Emissor* m_fountainEmitter = nullptr;
};

