#pragma once
#include "Geometry.h"
class PrismGeometry :
    public Geometry
{
public:
    PrismGeometry(Vector3D<float> pos, Vector3D<float> dir, float w, float h, float d, float W, float H);
    virtual ~PrismGeometry() = default;

    inline float getDWidth() const noexcept{ return m_Width; }
    inline float getDHeight() const noexcept { return m_Height; }
    inline float getDepth() const noexcept { return depth; }

    inline float getUWidth() const noexcept { return m_Width_U; }
    inline float getUHeight() const noexcept { return m_Height_U; }

    inline void setDWidth(float w) noexcept { m_Width = w; }
    inline void setDHeight(float h) noexcept { m_Height = h; }
    inline void setDepth(float d) noexcept { depth = d; }

    inline void setUWidth(float W) noexcept { m_Width_U = W; }
    inline void setUHeight(float H)  noexcept { m_Height_U = H; }

    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;
    
    virtual bool inArea(Particle* p) override;
private:
    float m_Width, m_Height;
    float depth;
    float m_Width_U, m_Height_U;
};