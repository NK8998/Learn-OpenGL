#include <game.h>
#include <resource_manager.h>
#include <sprite_renderer.h>
#include <ball_object.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const float BALL_RADIUS = 12.5F;

GameObject* Player;
SpriteRenderer* Renderer;

BallObject* Ball;                   

bool CheckCollision(GameObject& one, GameObject& two);

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
	ResourceManager::LoadTexture("../../resources/images/ball.png", true, "ball");

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
	

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));

}

void Game::Update(float dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

		this->levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Ball->Draw(*Renderer);
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
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

void Game::DoCollisions()
{
	for (GameObject& box : this->levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			if (CheckCollision(*Ball, box))
			{
				if (!box.IsSolid)
					box.Destroyed = true;
			}
		}
	}
}

bool CheckCollision(GameObject& one, GameObject& two)
{
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;

	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;

	return collisionX && collisionY;
}

bool CheckCollision(BallObject &one, GameObject &two)
{
	//get center point circle first
	glm::vec2 center(one.Position + one.Radius);

	//calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);

	//get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	//add clamped value to AAB_center aand we get value closesv to circle
	glm::vec2 closest = aabb_center + clamped;
	//retrieve vecvor between center circle and closesv poinv AABB and check again if length < radius
	difference = closest - center;
	return glm::length(difference) < one.Radius;
}