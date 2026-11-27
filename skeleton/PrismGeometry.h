#pragma once
#include "Geometry.h"
class PrismGeometry :
    public Geometry
{
public:
    /// <summary>
    /// Constructora de la clase, genera un prisma si los valores w == W y h == H son iguales; en caso contrario, creará una pirámide truncado
    /// </summary>
    /// <param name="pos">Posición</param>
    /// <param name="dir">Dirección</param>
    /// <param name="w"> Anchura de la geometría inferior</param>
    /// <param name="h"> Altura de la geometría inferior</param>
    /// <param name="d"> Altura de la geometría</param>
    /// <param name="W"> Anchura de la geometría superior</param>
    /// <param name="H"> Altura de la geometría superior</param>
    PrismGeometry(Vector3D<float> pos, Vector3D<float> dir, float w, float h, float d, float W, float H);
    virtual ~PrismGeometry() = default;

    // --------- Métodos accesores ---------

    inline float getDWidth() const noexcept{ return m_Width; }
    inline float getDHeight() const noexcept { return m_Height; }
    inline float getDepth() const noexcept { return depth; }

    inline float getUWidth() const noexcept { return m_Width_U; }
    inline float getUHeight() const noexcept { return m_Height_U; }

    // --------- Métodos mutadores ---------

    inline void setDWidth(float w) noexcept { m_Width = w; }
    inline void setDHeight(float h) noexcept { m_Height = h; }
    inline void setDepth(float d) noexcept { depth = d; }

    inline void setUWidth(float W) noexcept { m_Width_U = W; }
    inline void setUHeight(float H)  noexcept { m_Height_U = H; }

    //Método para comprobar que tiene la parícula dentro de la gemotría
    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual bool inArea(Particle* p) override;

private:
    //Anchura de la geometria inferior
    float m_Width;
    //Altura de la geometria inferior
    float m_Height;
    //Altura del prisma
    float depth;
    //Anchura de la geometria superor
    float m_Width_U;
    //Altura de la geometria superor
    float m_Height_U;
};