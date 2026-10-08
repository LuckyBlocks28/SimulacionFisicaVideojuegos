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

    void setMass(float mass);
    float getMass() const { return real_Mass; }

    void setAcceleration(Vector3D<float> acceleration);

private:
    const float velSFactor = 15.0f;

    Vector3D<float> velR;
    float real_Mass;
    float real_Gravity;
    
    float simulated_Mass;
    float simulated_Gravity;
    float simulated_Velocity;

};

