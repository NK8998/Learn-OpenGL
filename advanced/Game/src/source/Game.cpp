#include <game.h>
#include <resource_manager.h>
#include <sprite_renderer.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GameObject* Player;
SpriteRenderer* Renderer;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	
}

Game::~Game()
{
	delete Renderer;
	delete Player;
}

void Game::init()
{
	ResourceManager::LoadShader("../../shaders/game_shaders/sprite_transform.vert", "../../shaders/game_shaders/sprite_transform.frag", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	
	ResourceManager::LoadTexture("../../resources/images/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("../../resources/images/background.jpg", false, "background");
	ResourceManager::LoadTexture("../../resources/images/block.png", false, "block");
	ResourceManager::LoadTexture("../../resources/images/block_solid.png", false, "block_solid");

	GameLevel one; one.Load("../../resources/levels/one.lvl", this->Width, this->Height / 2);
	GameLevel two; two.Load("../../resources/levels/two.lvl", this->Width, this->Height / 2);
	GameLevel three; three.Load("../../resources/levels/three.lvl", this->Width, this->Height / 2);
	GameLevel four; four.Load("../../resources/levels/four.lvl", this->Width, this->Height / 2);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->Level = 0;

	glm::vec2 playerPos = glm::vec2(
		this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->Height - PLAYER_SIZE.y
	);

	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{

}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

		this->levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
	}

}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f)
				Player->Position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
				Player->Position.x += velocity;
		}
	}
}