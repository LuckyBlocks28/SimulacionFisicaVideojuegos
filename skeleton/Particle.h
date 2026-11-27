#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
class Emissor;
class Particle
{
public:
	// Tipo de particula a crear
	// - STATIC: particula estática sin movimiennto y sin que le afecte ninugna fuerza.
	// - MOVING: particula que se mueve y le afecta todo tipo de fuerzas.
	// - WITHOUT_GRAVITY: particula que no le afecta la gravedad pero si el resto de particulas.
	// - WITHOUT_FORCE: particula que no le afecta ninguna fuerza pero si la acceleración de la gravedad.
	enum TYPE {
		STATIC,
		MOVING,
		WITHOUT_GRAVITY,
		WITHOUT_FORCE
	};
	Particle();

	/// <summary>
	/// Constructora de la clase
	/// </summary>
	/// <param name="p"> Posición de la partícula</param>
	/// <param name="v"> Velocidad de la partíucla</param>
	/// <param name="size"> Tamaño de la partícula</param>
	/// <param name="mass"> Masa de la partícula</param>
	/// <param name="type"> Tipo de partícula</param>
	/// <param name="color"> Color de la partícula</param>
	Particle(Vector3D<float> p, Vector3D<float> v, float size, float mass,TYPE type, Vector4 color = Vector4(0.0, 1.0, 0.0, 1.0));

	virtual ~Particle();
	
	//Método integrador en euler
	void integrateEuler(float t);
	//Método integrador en euler-semi explícito
	void integrateEulerSemi(float t);
	//Método integrador en verlet
	void integrateVerlet(float t);
	
	//Método que crea laparticula por parte visual
	void createRenderItem(float size, Vector4 color);
	//Método para 
	void setOriginalPosition();

	//------- Métodos accesores a diferentes variables de la particula -------

	inline Vector3D<float> getPos() const noexcept { return pose.p; }
	inline Vector3D<float> getVel() const noexcept { return vel; }
	inline float getMass() const noexcept { return real_Mass; }
	inline Emissor* getEmissor() const noexcept { return m_emissor; }
	inline TYPE getType() const noexcept { return m_type; }

	//------- Métodos transmutables de variables de la particula -------

	inline void setPosition(Vector3D<float> p) noexcept { ogPos = p; pose = physx::PxTransform(p); };
	inline void setVelocity(Vector3D<float> v) noexcept { vel = v; };
	inline void setLifeTime(float lifetime) noexcept { remainingLife = lifetime; };
	inline void setAcceleration(Vector3D<float> a) noexcept { acc = a; };
	inline void setDamping(float d) noexcept { damping = d; };
	inline void setActive(bool state) noexcept { active = state; }
	void setMass(float mass) noexcept { real_Mass = mass; };
	inline void setEmissor(Emissor* e) noexcept { m_emissor = e; }
	inline void setType(TYPE t) noexcept { m_type = t; }
	
	//Métodos para añadir fuerza a la particula
	inline void addForce(Vector3D<float> f) { summatoryForce += f; }
	
	//Métodos para limpiar fuerzas a la particula
	inline void clearForce() { summatoryForce = Vector3D<float>(0.0f,0.0f,0.0f); }

	//Método para actualiar el movimiento de la partícula
	virtual void update(float dt);

	//------- Método para activar o desactivar la partícula -------

	inline bool isActive() const { return active; }
	void deactivate();

protected:
	//Velocidad de la partícula
	Vector3D<float> vel;
	//Aceleración de la particula
	Vector3D<float> acc = { 0.0f, -9.8f, 0.0f };
	//Damping de la partícula
	float damping;

	//Posición de la particula en escena
	physx::PxTransform pose;
	//Posición anterior de la particula en escena
	physx::PxTransform antPos;
	//Objeto para renderizar la particula
	RenderItem* renderItem;

	//Posición orignal de creación de la partícula
	Vector3D<float> ogPos;
	bool firstMove;

	//Bandera para activar y desactivar
	bool active = false;
	//Tiempo d evida de la partícula
	float remainingLife = 0.0f;

	//Acumulador de fuerzas
	Vector3D<float> summatoryForce;
	//Masa de la particula
	float real_Mass;

	//Tipo de particula
	TYPE m_type;
	//Emissor del que sale la partícula
	Emissor* m_emissor = nullptr;
};