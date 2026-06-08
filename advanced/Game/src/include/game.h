#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <game_level.h>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);


class Game
{
public:
	// game state
	GameState State;
	bool Keys[1024];
	unsigned int Width, Height;
	std::vector<GameLevel> levels;
	unsigned int Level;
	// constructor / destructor
	Game(unsigned int width, unsigned int height);
	~Game();
	//initialize game state (load all shaders/textures/levels)
	void init();
	//game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void DoCollisions();
};

#endif