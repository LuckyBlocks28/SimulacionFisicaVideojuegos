#pragma once
#include "Geometry.h"


class CilinderGeometry :
    public Geometry
{
public:
    /// <summary>
    /// Constructora de la clase, genera un cilindro si los valores l y L son iguales; en caso contrario, creará un cono truncado
    /// </summary>
    /// <param name="pos">Posición de la geometría</param>
    /// <param name="dir"> Dirección de la geometría</param>
    /// <param name="l">Tamaño del radio de la circungferencia inferior</param>
    /// <param name="d">Altura de la geometría</param>
    /// <param name="L">Tamaño del radio de la circunferencia superior</param>
    CilinderGeometry(Vector3D<float> pos, Vector3D<float> dir, float l, float d, float L);
    virtual ~CilinderGeometry() = default;

    // --------- Métodos accesores ---------

    inline float getDLenght() const noexcept { return m_lenght; }
    inline float getDepth() const noexcept { return depth; }
    inline float getULenght() const noexcept { return m_lenght_U; }

    // --------- Métodos mutadores ---------
    inline void setDLenght(float l) noexcept { m_lenght = l; }
    inline void setDepth(float d) noexcept { depth = d; }
    inline void setULenght(float L) noexcept { m_lenght_U = L; }

    //Método para comprobar que tiene la parícula dentro de la gemotría
    virtual Vector3D<float> calculateDirection(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual Vector3D<float> calculateNewPosition(float factorRadius) override;
    //Método para comprobar que se encuentra dentro de la geometría la partícula
    virtual bool inArea(Particle* p) override;
private:
    //Longitud radio inferior
    float m_lenght;
    //Altura de la circunferencia
    float depth;
    //Longitud radio superior
    float m_lenght_U;
    //Radio
    float r;
    //Angulo para calcular la circunferencia
    float theta;
};

