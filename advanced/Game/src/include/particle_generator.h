#pragma once
#ifndef PARTICLE_GENERATOR_H

#include <glm/glm.hpp>
#include <vector>

#include <game_object.h>

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float     Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
	//update all particles
	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// render all particles
	void Draw();

private:
	std::vector<Particle> particles;
	unsigned int amount;

	Shader shader;
	Texture2D texture;
	unsigned int VAO;
	//initialize buffer and vertex attributes
	void init();
	//returnes first particle index that's currently unused Life <= 0.0f or 0 if none is currently inactive
	unsigned int firstUnusedParticle();
	//respawns particle
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

};


#endif // !PARTICLE_GENERATOR_H
