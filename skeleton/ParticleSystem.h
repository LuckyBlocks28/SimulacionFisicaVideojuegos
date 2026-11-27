#pragma once
#include "Particle.h"
#include "Proyectile.h"
#include "Emissor.h"

#include <vector>

class ParticleSystem
{
private:
	// Manejo por punteros para evitar duplicación/liberación prematura de RenderItem
	std::vector<Particle*> m_particles;
	// Gestión y liberación de memoria de emisores
	std::vector<Emissor*> m_emitters;   
	
	//Gravedad de las partículas
	Vector3D<float> m_gravity; 

	// ------- Espacio de acción (Bounding Box) -------

	Vector3D<float> m_minBounds;
	Vector3D<float> m_maxBounds;
	bool m_useBounds;

	// ------- Valores por defecto para partículas generadas -------

	float m_defaultParticleSize;
	Vector4 m_defaultParticleColor;
	float m_defaultLifeTime;

public:
	/// <summary>
	/// Constructora por defecto de la clase
	/// </summary>
	ParticleSystem();
	/// <summary>
	/// Construcotra de la clase
	/// </summary>
	/// <param name="gravity">Gravedad que se le van aaplicar a las particulas</param>
	/// <param name="minBounds"> Límites minimos donde pueden estar las particulas</param>
	/// <param name="maxBounds">Límites máximos donde pueden estar las particulas</param>
	/// <param name="typeP"></param>
	ParticleSystem(Vector3D<float> gravity, Vector3D<float> minBounds, Vector3D<float> maxBounds, Particle::TYPE typeP);
	~ParticleSystem();

	// ---------- Gestión de emisores ----------

	void addEmissor(Emissor* emitter);
	void removeEmissor(Emissor* emitter);

	// ---------- Creación e inserción de partículas----------
	
	/// <summary>
	/// Método para añadir una sola particula
	/// </summary>
	/// <param name="particle"> Particula a añadir</param>
	void addParticle(Particle* particle);

	/// <summary>
	/// Método para crear una partícula y añadirla a las listas
	/// </summary>
	/// <param name="pos">Posición</param>
	/// <param name="vel">Velocidad</param>
	/// <param name="typeP">Tipo de partícula</param>
	/// <param name="mass">Masa</param>
	/// <param name="size">Tamaño</param>
	/// <param name="lifeTime">tiempo de vida</param>
	/// <param name="color">Color</param>
	/// <returns></returns>
	Particle* addParticle(Vector3D<float> pos, Vector3D<float> vel, Particle::TYPE typeP, float mass, float size = 0.5f, float lifeTime = 4.0f, 
		Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));


	// Bucle principal
	void update(double dt);

	// ---------- Métodos accesores ----------

	inline Vector3D<float> getGravity() const noexcept { return m_gravity; }
	inline const std::vector<Particle*>& getParticles() const noexcept { return m_particles; }
	inline size_t getParticleCount() const noexcept { return m_particles.size(); }
	inline Particle::TYPE getParticleType() const noexcept { return m_type; }

	// ---------- Métodos mutables ----------
	inline void setGravity(const Vector3D<float>& gravity) noexcept { m_gravity = gravity; }
	inline void setBounds(const Vector3D<float>& minB, const Vector3D<float>& maxB) noexcept {
		m_minBounds = minB;
		m_maxBounds = maxB;
		m_useBounds = true;
	}

	inline void setDefaultParticleProperties(float size, Vector4 color, float lifeTime) noexcept {
		m_defaultParticleSize = size;
		m_defaultParticleColor = color;
		m_defaultLifeTime = lifeTime;
	}

	//---------- Métodos para activar emissores ----------

	inline void onOffAllEmission() {
		for (auto& a : m_emitters) {
			a->activate(!a->getActive());
		}
	}
	inline void onOffEmissor(int i) {
		if (i >= 0 && i < m_emitters.size()) {
			m_emitters[i]->activate(!m_emitters[i]->getActive());
		}
	}
	

private:
	//Método para comprobar si la partícula se sale de las límites
	bool isOutOfBounds(const physx::PxVec3& pos) const;
	//Limpiar todas las partículas del sistema
	void clearDeadParticles();
	//Tipo de particula del sistema
	Particle::TYPE m_type;
};

