#pragma once

#include <glm/glm.hpp>

#include <textures.h>
#include <game_object.h>

#ifndef BALL_OBJECT_H

class BallObject : public GameObject
{
public:
	float Radius;
	bool Stuck;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 Move(float dt, unsigned int window_width);
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif // !BALL_OBJECT_H
