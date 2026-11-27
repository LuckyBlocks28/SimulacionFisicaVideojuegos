#pragma once

#include "Vector3D.h"
#include "Particle.h"
#include "Geometry.h" 

#include <vector>
#include <functional>
#include <random>
#include <cmath>
#include <limits>
#include <numbers>
#include <algorithm>

class ParticleSystem;
class Emissor
{
public:
	//Tipo de distribución a la hora de generar las partículass
	enum DENSITYFUNTION {
		UNIFORM,
		NORMAL
	};

protected:
	/// <summary>
	/// Generar un numero aleatorio con distribución uniforme
	/// </summary>
	/// <param name="min"> Valor mínimo </param>
	/// <param name="max"> Valor máximo </param>
	/// <returns></returns>
	float uniformRandom(float min, float max);
	/// <summary>
	/// Generar un numero aleatorio con distribución normal
	/// </summary>
	/// <param name="min"> Valor mínimo </param>
	/// <param name="max"> Valor máximo </param>
	/// <param name="mean"> Media</param>
	/// <param name="stdDeviation"> Desviación típica</param>
	/// <returns></returns>
	float normalRandom(float min, float max, float mean, float stdDeviation);

	/// <summary>
	/// Dar un numero aleatorio en función de la distribución del emissor
	/// </summary>
	/// <param name="min"> Valor mínimo </param>
	/// <param name="max"> Valor máximo </param>
	/// <returns></returns>
	float newFloat(float min, float max);

public:
	/// <summary>
	/// Constructora de la clase emissor
	/// </summary>
	/// <param name="ps"> Referencia al sistema de partículas</param>
	/// <param name="position"> Positción del emissor</param>
	/// <param name="direction"> Dirreción de las partículas</param>
	/// <param name="g"> Tipo de geometría del emissor</param>
	/// <param name="size"> Tamaño de la partícula </param>
	/// <param name="lifeTime"> Tiempo de vida de la partícula</param>
	/// <param name="color"> Color de la partícula</param>
	/// <param name="m_minVel"> Valor minimo de velocidad de las partículas</param>
	/// <param name="m_maxVel"> Valor máximo de velocidad de las partículas</param>
	/// <param name="mean"> Media de dispersión de las partículas</param>
	/// <param name="stdD"> Desviación típica</param>
	/// <param name="freq"> Frecuencia de generación de las partículas</param>
	/// <param name="active"> Activación inicial</param>
	/// <param name="func"> Tipo de distribución</param>
	Emissor(ParticleSystem* ps, Vector3D<float> position, Vector3D<float> direction, 
		Geometry* g, float size, float lifeTime, Vector4 color, float m_minVel, 
		float m_maxVel, float mean, float stdD, float freq, bool active, DENSITYFUNTION func);
	virtual ~Emissor();
	// Accesor a la activación del emissor
	inline void activate(bool activate) noexcept { m_active = activate; }
	// Activación/Desactivación del emissor
	inline bool isActive() const noexcept { return m_active; }

	//--------- Métodos mutadores de variables de emissor ---------

	inline void setMean(float mean) noexcept { m_mean = mean; }
	inline void setStdDeviation(float stdDeviation) noexcept { m_stdDeviation = stdDeviation; }
	inline void setVel(float minVel, float maxVel) noexcept { m_minVel = minVel; m_maxVel = maxVel; }
	inline void setFreq(float f) noexcept { m_freq = f; }
	inline void setDensityFunction(DENSITYFUNTION densityFunc) noexcept { m_densityFunc = densityFunc; }

	//--------- Métodos accesores de variables de emissor ---------

	inline Vector3D<float>& getPos() noexcept { return m_position; }
	inline Vector3D<float>& getDir() noexcept { return m_direction; }
	inline float getMean() const noexcept { return m_mean; }
	inline float getStdDeviation() const noexcept { return m_stdDeviation; }
	inline float getMinVel() const noexcept { return m_minVel; }
	inline float getMaxVel() const noexcept { return m_maxVel; }
	inline float getFreq() const noexcept { return m_freq; }
	inline float getSize() const noexcept { return m_size; }
	inline float getLifeTime() const noexcept { return m_lifeTime; }
	inline Vector4 getColor() const noexcept { return m_color; }
	inline bool getActive() const noexcept { return m_active; }

	//--------- Métodos acerca de le geometría ---------
	
	inline void setKeepInside(bool keep) noexcept { m_keepInside = keep; }
	inline bool isKeepingInside() const noexcept { return m_keepInside; }
	inline Geometry* getGeometry() const noexcept { return m_geom; }
	
	//Método para actualizar el objeto
	virtual bool update(double t);

protected:
	//Geometría del objeto
	Geometry* m_geom;
	//Tipo de distribución
	DENSITYFUNTION m_densityFunc;

	//Posición del emissor
	Vector3D<float> m_position;
	//Dirección de la partícula
	Vector3D<float>	m_direction;
	//Color de las partículas
	Vector4 m_color;
	
	//Media
	float m_mean;
	//Cantidad de desviación típca
	float m_stdDeviation;
	//Velocidad mínima de las partículas
	float m_minVel;
	//Velocidad máxima de las partículas
	float m_maxVel;
	//Frecuencia
	float m_freq;
	//Tiempo actual
	float m_currT;
	//Tamaño de la partícula
	float m_size;
	//Tiempo de vida de la partícual
	float m_lifeTime;
	//Masa de la partícula
	float m_mass;
	//Bandera de activación de la partícula
	bool m_active;
	
	//Bandera para manterner a las particulas dentro de la geometría
	bool m_keepInside = false;
	
	//Semilla para generar aleatoriamente los valores
	std::mt19937 m_engine;
	
	//Refernecia al objeto tipo sistema de partículas
	ParticleSystem* m_pS;  
};

