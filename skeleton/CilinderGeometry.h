#pragma once
#include "Geometry.h"


class CilinderGeometry :
    public Geometry
{
public:
    CilinderGeometry(Vector3D<float> pos, Vector3D<float> dir, float l, float d, float L);
    virtual ~CilinderGeometry() = default;

    inline float getDLenght() const noexcept { return m_lenght; }
    inline float getDepth() const noexcept { return depth; }
    inline float getULenght() const noexcept { return m_lenght_U; }

    inline void setDLenght(float l) noexcept { m_lenght = l; }
    inline void setDepth(float d) noexcept { depth = d; }
    inline void setULenght(float L) noexcept { m_lenght_U = L; }

    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;

    virtual bool inArea(Particle* p) override;
private:
    float m_lenght;
    float depth;
    float m_lenght_U;
    float r, theta;
};

