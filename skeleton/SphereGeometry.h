#pragma once
#include "Geometry.h"
class SphereGeometry :
    public Geometry
{
public:
    /// <summary>
    /// Constructor de la clase
    /// </summary>
    /// <param name="pos"> Posición de la geometría</param>
    /// <param name="dir"> Dirección de la geometría</param>
    /// <param name="l"> Radio de la esfera</param>
    SphereGeometry(Vector3D<float> pos, Vector3D<float> dir, float l);
    virtual ~SphereGeometry() = default;

    // --------- Métodos accesores y mutadores ---------

    inline float getLenght() const noexcept { return m_lenght; }
    inline void setLenght(float l) noexcept { m_lenght = l; }

    //Método para comprobar que tiene la parícula dentro de la gemotría
    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual bool inArea(Particle* p) override;
    //Método para comprobar que tiene la parícula dentro de la gemotría
    void containParticle(Particle* p, float restitution = 0.7f) override;

private:
    //Radio de la esfera
    float m_lenght;
    //Radio
    float r;
    //Angulo para calcular la circunferencia
    float theta;
};

