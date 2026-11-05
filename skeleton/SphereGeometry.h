#pragma once
#include "Geometry.h"
class SphereGeometry :
    public Geometry
{
public:
    SphereGeometry(Vector3D<float> pos, Vector3D<float> dir, float l);
    virtual ~SphereGeometry() = default;

    inline float getLenght() const noexcept { return m_lenght; }
    inline void setLenght(float l) noexcept { m_lenght = l; }

    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;

    virtual bool inArea(Particle* p) override;
private:
    float m_lenght;
    float r, theta;
    
};

