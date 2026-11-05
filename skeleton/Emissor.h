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
	enum TYPE {
		PARTICLE,
		PROYECTILE
	};

	enum DENSITYFUNTION {
		UNIFORM,
		NORMAL
	};

protected:

	float uniformRandom(float min, float max);
	float normalRandom(float min, float max, float mean, float stdDeviation);

	float newFloat(float min, float max);

public:

	Emissor(ParticleSystem* ps, Vector3D<float> position, Vector3D<float> direction, Geometry* g, float size, float lifeTime, Vector4 color, float m_minVel, float m_maxVel, TYPE type, float mean, float stdD, float freq, bool active,DENSITYFUNTION func);
	virtual ~Emissor();
	
	inline void activate(bool activate) noexcept { m_active = activate; }
	inline bool isActive() const noexcept { return m_active; }

	inline void setDensityFunction(DENSITYFUNTION densityFunc) noexcept { m_densityFunc = densityFunc; }
	inline void setParticleType(TYPE type) noexcept { m_type = type; }

	inline Vector3D<float>& getPos() noexcept { return m_position; }
	inline Vector3D<float>& getDir() noexcept { return m_direction; }

	inline void setMean(float mean) noexcept { m_mean = mean; }
	inline void setStdDeviation(float stdDeviation) noexcept { m_stdDeviation = stdDeviation; }
	inline void setVel(float minVel, float maxVel) noexcept { m_minVel = minVel; m_maxVel = maxVel; }
	inline void setFreq(float f) noexcept { m_freq = f; }

	inline float getMean() const noexcept { return m_mean; }
	inline float getStdDeviation() const noexcept { return m_stdDeviation; }
	inline float getMinVel() const noexcept { return m_minVel; }
	inline float getMaxVel() const noexcept { return m_maxVel; }
	inline float getFreq() const noexcept { return m_freq; }
	inline float getSize() const noexcept { return m_size; }
	inline float getLifeTime() const noexcept { return m_lifeTime; }
	inline Vector4 getColor() const noexcept { return m_color; }

	virtual bool update(double t);

protected:

	TYPE m_type;
	Geometry* m_geom;
	DENSITYFUNTION m_densityFunc;

	Vector3D<float> m_position, m_direction;
	Vector4 m_color;
	float m_mean, m_stdDeviation, m_minVel, m_maxVel, m_freq, m_currT, m_size, m_lifeTime, m_mass;
	bool m_active;

	std::mt19937 m_engine;

	ParticleSystem* m_pS;  
};

