#pragma once
#include "Geometry.h"
class BoxGeometry :
    public Geometry
{
public:
    BoxGeometry(Vector3D<float> pos, Vector3D<float> dir, float w, float h, float d);
    virtual ~BoxGeometry() = default;

    // --------- Métodos accesores ---------
    inline float getWidth() const noexcept { return m_Width; }
    inline float getHeight() const noexcept { return m_Height; }
    inline float getDepth() const noexcept { return depth; }

    // --------- Métodos mutadores ---------
    inline void setWidth(float w) noexcept { m_Width = w; }
    inline void setHeight(float h) noexcept { m_Height = h; }
    inline void setDepth(float d) noexcept { depth = d; }


    //Método para comprobar que tiene la parícula dentro de la gemotría
    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual bool inArea(Particle* p) override;

    //Método para comprobar que tiene la parícula dentro de la gemotría
    void containParticle(Particle* p, float restitution = 0.7f) override;

private:
    //Anchura
    float m_Width;
    //Altura
    float m_Height;
    //Altura de la geometría
    float depth;
};

