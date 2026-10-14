#pragma once

#include "Vector3D.h"
#include "Particle.h"

#include <vector>
#include <functional>
#include <random>

class ParticleSystem;
class Emissor
{
protected:
	union DATA {
		struct rectangle {
			float w;
			float h;

			float W;
			float H;

			float depth;
		} m_rectangle;

		struct square {
			float l;
			float L;

			float depth;
		} m_square;

		struct circle {
			float l;
			float L;

			float depth;
		} m_circle;

		struct cube {
			float size;
		} m_cube;

		struct box {
			float w;
			float h;

			float depth;
		} m_box;

		struct sphere {
			float radius;
		} m_sphere;
	};
	enum GEOMETRY {
		RECTANGLE,
		SQUARE,
		CIRCLE,
		CUBE,
		BOX,
		SPHERE
	};
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
	Emissor(Vector3D<float> position, Vector3D<float> direction, float m_minVel, float m_maxVel, TYPE type, float mean, float stdD, float freq, bool active, GEOMETRY geometry);

	Emissor(Vector3D<float> position, Vector3D<float> direction, float m_minVel, float m_maxVel, TYPE type, DATA::circle data,
		float mean = 0.f, float stdD = 1.f, float freq = 1.f, bool active = false);

	Emissor(Vector3D<float> position, Vector3D<float> direction, float m_minVel, float m_maxVel, TYPE type, DATA::square data,
		float mean = 0.f, float stdD = 1.f, float freq = 1.0f, bool active = false);

	Emissor(Vector3D<float> position, Vector3D<float> direction, float m_minVel, float m_maxVel, TYPE type, DATA::rectangle data,
		float mean = 0.f, float stdD = 1.f, float freq = 1.0f, bool active = false);

	Emissor(Vector3D<float> position,Vector3D<float> direction,float m_minVel, float m_maxVel,TYPE type,DATA::cube data,
		float mean = 0.f, float stdD = 1.f,float freq = 1.0f,bool active = false);

	Emissor(Vector3D<float> position, Vector3D<float> direction, float m_minVel, float m_maxVel, TYPE type, DATA::box data,
		float mean = 0.f, float stdD = 1.f, float freq = 1.0f, bool active = false);

	Emissor(Vector3D<float> position, Vector3D<float> direction, float m_minVel, float m_maxVel, TYPE type, DATA::sphere data,
		float mean = 0.f, float stdD = 1.f, float freq = 1.0f, bool active = false);

	float uniformRandom(float min, float max);
	float normalRandom(float min, float max, float mean, float stdDeviation);

	float newFloat(float min, float max);
	Vector3D<float> newPosition();
	Vector3D<float> newDirection(const Vector3D<float> pos);

public:

	static Emissor* createRectangleGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float depth, float groundWidth, float groundHeight, float airWidth, float airHeight, float m_minVel, float m_maxVel,
		TYPE particleType, float mean = 0.f, float stdD = 1.f, float freq = 1.f, bool active = false);

	static Emissor* createSquareGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float depth, float groundSize, float airSize, float m_minVel, float m_maxVel, TYPE particleType,
		float mean = 0.f, float stdD = 1.f, float freq = 1.f, bool active = false);

	static Emissor* createCircleGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float depth, float groundRadius, float airRadius, float m_minVel, float m_maxVel, TYPE particleType,
		float mean = 0.f, float stdD = 1.f, float freq = 1.f, bool active = false);

	static Emissor* createCubeGenerator(ParticleSystem* pS, Vector3D<float> position,Vector3D<float> direction,float size,float m_minVel, float m_maxVel,TYPE particleType,
		float mean = 0.f, float stdD = 1.f,float freq = 1.f,bool active = false);

	static Emissor* createBoxGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float width, float height, float depth, float m_minVel, float m_maxVel, TYPE particleType,
		float mean = 0.f, float stdD = 1.f, float freq = 1.f, bool active = false);

	static Emissor* createSphereGenerator(ParticleSystem* pS, Vector3D<float> position, Vector3D<float> direction, float radius, float m_minVel, float m_maxVel, TYPE particleType,
		float mean = 0.f, float stdD = 1.f, float freq = 1.f, bool active = false);

	virtual ~Emissor() {};
	
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

	virtual bool update(double t);

protected:
	DATA m_data;
	TYPE m_type;
	GEOMETRY m_geometry;
	DENSITYFUNTION m_densityFunc;

	Vector3D<float> m_position, m_direction;
	float m_mean, m_stdDeviation, m_minVel, m_maxVel, m_freq, m_currT;
	bool m_active;

	std::mt19937 m_engine;

	ParticleSystem* m_pS;  
};

