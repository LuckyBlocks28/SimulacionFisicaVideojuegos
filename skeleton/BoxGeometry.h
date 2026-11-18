#pragma once
#include "Geometry.h"
class BoxGeometry :
    public Geometry
{
public:
    BoxGeometry(Vector3D<float> pos, Vector3D<float> dir, float w, float h, float d);
    virtual ~BoxGeometry() = default;

    inline float getWidth() const noexcept { return m_Width; }
    inline float getHeight() const noexcept { return m_Height; }
    inline float getDepth() const noexcept { return depth; }


    inline void setWidth(float w) noexcept { m_Width = w; }
    inline void setHeight(float h) noexcept { m_Height = h; }
    inline void setDepth(float d) noexcept { depth = d; }


    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;
    void containParticle(Particle* p, float restitution = 0.7f) override;

    virtual bool inArea(Particle* p) override;
private:
    float m_Width, m_Height;
    float depth;
};

