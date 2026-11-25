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

    physx::PxTransform m_pinballAnchorPose;
    RenderItem* m_pinballAnchorRenderItem = nullptr;

    Particle* m_plungerHead = nullptr; // Partícula que hace de tope del muelle
    Particle* m_pinballBall = nullptr; // Partícula nueva que saldrá disparada
    AnchoredSpringForceEmissor* m_plungerSpring = nullptr;

    bool m_isCharging = false;
    float m_timeSinceKey = 0.0f;
    const float m_pinballY = -20.0f;     // Altura separada de los otros muelles
    const float m_restX = 15.0f;         // Posición X de reposo del tope (ancla en X = 0)
    const float m_maxPullbackX = 3.0f;   // Máximo retroceso hacia la izquierda
    const float m_plungerRadius = 2.0f;
    const float m_ballRadius = 2.0f;

};

