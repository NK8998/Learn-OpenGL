#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <tuple>

#include <game.h>
#include <resource_manager.h>
#include <sprite_renderer.h>
#include <ball_object.h>
#include <particle_generator.h>


enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const float BALL_RADIUS = 12.5F;

GameObject* Player;
SpriteRenderer* Renderer;

BallObject* Ball;                   

bool CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);

Direction VectorDirection(glm::vec2 target);

ParticleGenerator* Particles;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	
}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
}

void Game::init()
{
	ResourceManager::LoadShader("../../shaders/game_shaders/sprite_transform.vert", "../../shaders/game_shaders/sprite_transform.frag", nullptr, "sprite");
	ResourceManager::LoadShader("../../shaders/game_shaders/particle_gen.vert", "../../shaders/game_shaders/particle_gen.frag", nullptr, "particle");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);


	ResourceManager::LoadTexture("../../resources/images/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("../../resources/images/background.jpg", false, "background");
	ResourceManager::LoadTexture("../../resources/images/block.png", false, "block");
	ResourceManager::LoadTexture("../../resources/images/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("../../resources/images/ball.png", true, "ball");
	ResourceManager::LoadTexture("../../resources/images/particle.png", true, "particle");

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	
	GameLevel one; one.Load("../../resources/levels/one.lvl", this->Width, this->Height / 2);
	GameLevel two; two.Load("../../resources/levels/two.lvl", this->Width, this->Height / 2);
	GameLevel three; three.Load("../../resources/levels/three.lvl", this->Width, this->Height / 2);
	GameLevel four; four.Load("../../resources/levels/four.lvl", this->Width, this->Height / 2);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->Level = 0;

	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));	
}

void Game::Update(float dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));

	if (Ball->Position.y >= this->Height)
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

		this->levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Particles->Draw();
		Ball->Draw(*Renderer);
	}

}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->levels[0].Load("../../resources/levels/one.lvl", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->levels[1].Load("../../resources/levels/two.lvl", this->Width, this->Height / 2);
	else if (this->Level == 2)
		this->levels[2].Load("../../resources/levels/three.lvl", this->Width, this->Height / 2);
	else if (this->Level == 3)
		this->levels[3].Load("../../resources/levels/four.lvl", this->Width, this->Height / 2);

}

void Game::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
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
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision))
			{
				//destroy box if not solid
				if (!box.IsSolid)
					box.Destroyed = true;
				//Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) //horizontal collision
				{
					Ball->Velocity.x = -Ball->Velocity.x; //reverse horizontal velocity

					float penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration; //move ball to the right
					else
						Ball->Position.x -= penetration; //move ball to left
				}
				else
				{
					Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity

					float penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration;
					else
						Ball->Position.y += penetration;
				}
			}
		}
	}

	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		//check where it hit the board and change velocity
		float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
		float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		float percentage = distance / (Player->Size.x / 2.0f);
		// move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
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

Collision CheckCollision(BallObject &one, GameObject &two)
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
	
	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Direction VectorDirection(glm::vec2 target) 
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,  1.0f),
		glm::vec2(1.0f,  0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product; 
			best_match = i;
		}
	}
	return (Direction)best_match;
}