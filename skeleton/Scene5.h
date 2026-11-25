#pragma once
#include "Scene.h"
#include "Particle.h"
#include "ForceSystem.h"
#include "ForceEmmiter.h"
#include "SpringForceEmissor.h"
#include "AnchoredSpringForceEmissor.h"

#include <vector>

class Scene5 :
    public Scene
{
public:
    Scene5(std::string name) : Scene(std::move(name)) {};
    void init() override;
    void update(double dt) override;
    void keyPress(unsigned char key, const physx::PxTransform& camera) override;
    void cleanup() override;
private:
    ForceSystem* m_ForceSystem;
    std::vector<Particle*> m_particles;

    physx::PxTransform m_anchorPose;
    RenderItem* m_anchorRenderItem = nullptr;

};

