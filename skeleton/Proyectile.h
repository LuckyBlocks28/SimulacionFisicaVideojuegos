#pragma once
#include "Particle.h"
class Proyectile :
    public Particle
{
public:
    explicit Proyectile(Vector3D<float> p, Vector3D<float> v, float size, float mass, Vector4 color = Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    virtual ~Proyectile() = default;
    //Método para caluclar la masa simulada
    float calculateSimulatedMass(); 
    //Método para caluclar la gravedad simulada
    float calculateSimulateGravity();

private:
    //Factor de velocidad simulada
    const float velSFactor = 10.0f;
    //Gravedad real
    const float real_Gravity = -9.8f;
    //Velocidad real del objeto
    Vector3D<float> velR;

    //Masa simulada
    float simulated_Mass;
    //Gravedad simulada
    float simulated_Gravity;
    //Velocidad simulada
    float simulated_Velocity;
};

