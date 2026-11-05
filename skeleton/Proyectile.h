#pragma once
#include "Particle.h"
class Proyectile :
    public Particle
{
public:
    explicit Proyectile(Vector3D<float> p, Vector3D<float> v, float size, float mass, Vector4 color = Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    virtual ~Proyectile() = default;
    float calculateSimulatedMass();
    float calculateSimulateGravity();

private:
    const float velSFactor = 10.0f;
    const float real_Gravity = -9.8f;

    Vector3D<float> velR;

    
    float simulated_Mass;
    float simulated_Gravity;
    float simulated_Velocity;

};

