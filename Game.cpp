#include "Game.h"
#include "BallObject.h"
#include "VoidBall.h"
#include "DeadWeight.h"
#include "SpriteRenderer.h"
#include "ResourceMaster.h"
#include "GameObject.h"
#include "ParticleGenerator.h"
#include "PostProcess.h"
#include "TextRenderer.h"
#include <irrKlang.h>
#include <sstream>
#include <map>
using namespace irrklang;

Ball* GameBall;
GameObject* Player;
GLint extendLimit = 3;
GLfloat extendRate = 10.0f;
GLint extensions = 0;
SpriteRenderer *Renderer;
ParticleGenerator *Particles;
PostProcessor *Effects;
GLfloat EffectTime = 0.0f;
GLfloat ShakeTime = 0.0f;
GLfloat RainbowTime = 0.0f;
GLboolean isInvincible = GL_FALSE;
GLboolean isWon = GL_FALSE;
const GLfloat EffectTimeDuration = 2.0f;
const GLfloat RainbowDuration = 6.0f;
std::vector<VoidBall> voids;
std::vector<DeadWeight> weights;
const GLfloat VOID_VELOCITY = 20.0f;
ISoundEngine *SoundEngine = createIrrKlangDevice();
TextRenderer  *Text;
glm::vec2 menuTitlePositions[8] = {
	glm::vec2(75.0f, 100.0f),
	glm::vec2(150.0f, 100.0f),
	glm::vec2(225.0f, 100.0f),
	glm::vec2(300.0f, 100.0f),
	glm::vec2(375.0f, 100.0f),
	glm::vec2(450.0f, 100.0f),
	glm::vec2(525.0f, 100.0f),
	glm::vec2(600.0f, 100.0f)
}; 
GLfloat winPositions[2];
const GLfloat WIN_VELOCITY = 220.0f;



Game::Game(GLuint width, GLuint height, GLuint lives)
: State(MENU), Keys(), Width(width), Height(height), Lives(lives)
{
	
}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete GameBall;
	voids.clear();
	weights.clear();
	delete Particles;
}

void Game::InitGame()
{
	winPositions[0] = (GLfloat)this->Width;
	winPositions[1] = (GLfloat)this->Width;
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("QuickJerk/DUNEBUG.ttf", 20);
	SoundEngine->play2D("audio/windy.mp3", GL_TRUE);
	ResourceMaster::LoadShader("SpriteShader.vert", "SpriteShader.frag", nullptr, "sprite");
	ResourceMaster::LoadShader("ParticleShader.vert", "ParticleShader.frag", nullptr, "particle");
	ResourceMaster::LoadShader("PostProcess.vert", "PostProcess.frag", nullptr, "postproc");
	glm::mat4 projection = glm::ortho(0.0f,
		static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceMaster::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceMaster::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceMaster::GetShader("particle").Use().SetInteger("image", 0);
	ResourceMaster::GetShader("particle").SetMatrix4("projection", projection);
	Renderer = new SpriteRenderer(ResourceMaster::GetShader("sprite"));
	ResourceMaster::LoadTexture("Sprites/particle.png", GL_TRUE, "particle");
	ResourceMaster::LoadTexture("Sprites/SquidPaddle.png", GL_TRUE, "squidPaddle", 5);
	ResourceMaster::LoadTexture("Sprites/DeadSquid.png", GL_TRUE, "deadSquid", 12);
	ResourceMaster::LoadTexture("Sprites/background.png", GL_TRUE, "background");
	ResourceMaster::LoadTexture("Sprites/Blocks/plain.png", GL_FALSE, "plain");
	ResourceMaster::LoadTexture("Sprites/Blocks/indestructible.png", GL_FALSE, "indestructible");
	ResourceMaster::LoadTexture("Sprites/Blocks/chaos.png", GL_FALSE, "chaos");
	ResourceMaster::LoadTexture("Sprites/Blocks/dizzy.png", GL_FALSE, "dizzy");
	ResourceMaster::LoadTexture("Sprites/Blocks/invert.png", GL_FALSE, "invert");
	ResourceMaster::LoadTexture("Sprites/Blocks/magnet.png", GL_FALSE, "magnet");
	ResourceMaster::LoadTexture("Sprites/EyeBraun/globe.png", GL_TRUE, "globe");
	ResourceMaster::LoadTexture("Sprites/EyeBraun/veins.png", GL_TRUE, "veins");
	ResourceMaster::LoadTexture("Sprites/EyeBraun/iris.png", GL_TRUE, "iris");
	ResourceMaster::LoadTexture("Sprites/EyeBraun/pupil.png", GL_TRUE, "pupil");
	ResourceMaster::LoadTexture("Sprites/Blocks/speed.png", GL_TRUE, "speed");
	ResourceMaster::LoadTexture("Sprites/Blocks/slow.png", GL_TRUE, "slow");
	ResourceMaster::LoadTexture("Sprites/Blocks/extend.png", GL_TRUE, "extend");
	ResourceMaster::LoadTexture("Sprites/Blocks/baby.png", GL_TRUE, "baby", 9);
	ResourceMaster::LoadTexture("Sprites/Blocks/death.png", GL_TRUE, "death");
	Particles = new ParticleGenerator(ResourceMaster::GetShader("particle"), 
		ResourceMaster::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceMaster::GetShader("postproc"), this->Width, this->Height);
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
		);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceMaster::GetTexture("squidPaddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	GameBall = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceMaster::GetTexture("globe"));
	GameLevel one;
	one.Load("Levels/LevelOne.txt", this->Width, (GLuint)(this->Height * 0.5f));
	this->Levels.push_back(one);
	this->Level = 0;
}

void Game::Update(GLfloat dt)
{
	if (this->State == GameState::MENU)
	{
		for (int i = 0; i < 8; i++)
		{
			menuTitlePositions[i] += glm::vec2((GLfloat)((rand() % 10) - 5), (GLfloat)((rand() % 10) - 4));
		}
	}
	else
	{
		if(GameBall->Position.y > -BALL_RADIUS * 2.0f)
			GameBall->Move(dt, this->Width, this->Height, *SoundEngine, isInvincible, isWon);
		this->DoCollisions();
		if (Player->Size.x < PLAYER_SIZE.x + ((GLfloat)extensions * 25.0f))
			Player->Size.x += (extendRate * dt);

		Particles->Update(dt, *GameBall, 2, glm::vec2(GameBall->Radius / 2));
		if (EffectTime > 0.0f)
		{
			EffectTime -= dt;

			if (EffectTime <= 0.0f)
			{
				Effects->IsChaos = GL_FALSE;
				Effects->IsConfused = GL_FALSE;
				if (ShakeTime <= 0.0f) Effects->IsShaken = GL_FALSE;
			}

		}
		if (ShakeTime > 0.0f)
		{
			ShakeTime -= dt;
			if (ShakeTime <= 0.0f && EffectTime <= 0.0f) Effects->IsShaken = GL_FALSE;
		}
		if (RainbowTime > 0.0f)
		{
			RainbowTime -= dt;
			if (RainbowTime <= 0.0f)
			{
				SoundEngine->removeSoundSource("audio/windy.mp3");
				SoundEngine->play2D("audio/wormsign.mp3", GL_TRUE);
				if(!isWon) isInvincible = GL_FALSE;
			}
		}
		if (GameBall->IsMagnetized) GameBall->Velocity = glm::length(GameBall->Velocity) *
			glm::normalize(Player->Position - GameBall->Position);

		std::vector<VoidBall>::iterator it;
		int i = 0;
		for (it = voids.begin(); it < voids.end(); it++, i++) {
			it->Move(dt, 0);
		}
		std::vector<DeadWeight>::iterator itD;
		i = 0;
		for (itD = weights.begin(); itD < weights.end(); itD++, i++) {
			itD->Move(dt, this->Height);
		}
		if (GameBall->Position.y >= this->Height && !this->isDead && !isInvincible)
		{
			SoundEngine->play2D("audio/fallDeath.wav");
			this->isDead = true;
			Player->Sprite = ResourceMaster::GetTexture("deadSquid");
			Renderer->isIncreasing = true;
		}
		if (this->isDead && Player->Sprite.isDead && !isWon)
		{
			this->Lives--;
			if (Lives <= 0)
			{
				ResetLevel();
				this->State = GameState::MENU;
				SoundEngine->removeAllSoundSources();
				SoundEngine->play2D("audio/windy.mp3", GL_TRUE);
			}
			this->ResetPlayer();
		}
		if (this->State == GameState::WIN)
		{
			if (winPositions[0] > 250.0f) winPositions[0] -= WIN_VELOCITY * dt;
			if (winPositions[1] > 130.0f) winPositions[1] -= WIN_VELOCITY * dt;
			GameBall->IsStuck = GL_FALSE;
		}
		if (this->Levels[this->Level].IsCompleted() && this->State != GameState::WIN)
		{
			isInvincible = GL_TRUE;
			SoundEngine->play2D("audio/YEAH.wav");
			this->State = GameState::WIN;
			isWon = GL_TRUE;
		}
	}
	
}
GLboolean Game::FindLastUsedWeight(GameObject &obj)
{
	std::vector<DeadWeight>::iterator it;
	int i = 0;
	for (it = weights.begin(); it < weights.end(); it++, i++) {
		if (it->Position.y > this->Height + it->Size.y)
		{
			it->Position = obj.Position;
			it->Velocity = glm::vec2(0.0f, 0.0f);
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}
GLboolean Game::FindLastUsedVoid(GameObject &obj)
{
	std::vector<VoidBall>::iterator it;
	int i = 0;
	for (it = voids.begin(); it < voids.end(); it++, i++) {
		if (it->Radius <= 1.0f)
		{
			it->Radius = BALL_RADIUS;
			it->Position = obj.Position;
			it->isIncreasing = GL_TRUE;
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}

void Game::SettleDestructibleEffect(GameObject &obj)
{
	if (obj.Type == BrickType::MAGNET)
	{
		SoundEngine->play2D("audio/magnet.wav");
		GameBall->IsMagnetized = GL_TRUE;
	}
	else if (obj.Type == BrickType::VOID)
	{
		SoundEngine->play2D("audio/void.wav");
		if(!FindLastUsedVoid(obj)) voids.push_back(VoidBall(obj.Position, BALL_RADIUS,
			glm::vec2(VOID_VELOCITY, VOID_VELOCITY), ResourceMaster::GetTexture("globe")));
	}
	else if (obj.Type == BrickType::DEATH)
	{
		SoundEngine->play2D("audio/dieSound.wav");
		if (!FindLastUsedWeight(obj)) weights.push_back(DeadWeight(obj.Position, BALL_RADIUS * 2.0f,
			glm::vec2(0.0f, 0.0f), ResourceMaster::GetTexture("death")));
	}
	else if (obj.Type == BrickType::SPEEDY)
	{
		SoundEngine->play2D("audio/speed.wav");
		GameBall->SwitchGears(GL_TRUE, GL_TRUE);
	}
	else if (obj.Type == BrickType::SLOW)
	{
		SoundEngine->play2D("audio/slow.wav");
		GameBall->SwitchGears(GL_TRUE, GL_FALSE);
	}
	else if (obj.Type == BrickType::INVINCIBLE)
	{
		if (!isInvincible)
		{
			SoundEngine->removeSoundSource("audio/wormsign.mp3");
			SoundEngine->play2D("audio/windy.mp3");
		}
		RainbowTime = RainbowDuration;
		isInvincible = GL_TRUE;
	}
	else if (obj.Type == BrickType::EXTEND && extensions < extendLimit)
	{
		SoundEngine->play2D("audio/extend.mp3");
		extensions++;
	}
	else if (obj.Type == BrickType::BABY)
	{
		SoundEngine->play2D("audio/baby.wav");
		for (GameObject &box : this->Levels[this->Level].Bricks)
		{
			if (glm::distance(box.Position, obj.Position) 
				<= 2.0f * obj.Size.x && box.Type != BrickType::BABY)
			{
				box.Destroyed = GL_FALSE;
			}
		}
	}
	else if (obj.Type != BrickType::PLAIN)
	{
		Effects->IsChaos = GL_FALSE;
		Effects->IsConfused = GL_FALSE;
		Effects->IsShaken = GL_FALSE;
		EffectTime = EffectTimeDuration;
	}
	else SoundEngine->play2D("audio/hit.wav");
	
	switch (obj.Type)
	{
		case BrickType::CHAOS:
			SoundEngine->play2D("audio/chaos.mp3");
			Effects->IsChaos = GL_TRUE;
			break;
		case BrickType::DIZZY:
			SoundEngine->play2D("audio/blur.wav");
			Effects->IsShaken = GL_TRUE;
			break;
		case BrickType::INVERT:
			SoundEngine->play2D("audio/invert.mp3");
			Effects->IsConfused = GL_TRUE;
			break;
		default:
			break;
	}
}

void Game::DoCollisions()
{
	for (GameObject &box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*GameBall, box);
			if (std::get<0>(collision)) // If collision is true
			{
				GameBall->hitCounter++;
				if (GameBall->hitCounter >= GameBall->HIT_LIMIT) GameBall->SwitchGears();
				// Destroy block if not solid
				if (!box.IsSolid)
				{
					SettleDestructibleEffect(box);
					box.Destroyed = GL_TRUE;
				}	
				else
				{   // if block is solid, enable shake effect
					SoundEngine->play2D("audio/hit.wav");
					ShakeTime = 0.05f;
					Effects->IsShaken = GL_TRUE;
				}
				// Collision resolution
				Direction::Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == Direction::LEFT || dir == Direction::RIGHT) // Horizontal collision
				{
					GameBall->Velocity.x = -GameBall->Velocity.x; // Reverse horizontal velocity
					// Relocate
					GLfloat penetration = GameBall->Radius - std::abs(diff_vector.x);
					if (dir == Direction::LEFT)
						GameBall->Position.x += penetration; // Move ball to right
					else
						GameBall->Position.x -= penetration; // Move ball to left;
				}
				else // Vertical collision
				{
					GameBall->Velocity.y = -GameBall->Velocity.y; // Reverse vertical velocity
					// Relocate
					GLfloat penetration = GameBall->Radius - std::abs(diff_vector.y);
					if (dir == Direction::UP)
						GameBall->Position.y -= penetration; // Move ball bback up
					else
						GameBall->Position.y += penetration; // Move ball back down
				}
			}
			
			std::vector<VoidBall>::iterator it;
			int i = 0;
			for (it = voids.begin(); it < voids.end(); it++, i++) {

				collision = CheckCollision(*it, box);
				if (std::get<0>(collision) && !box.IsSolid) box.Destroyed = GL_TRUE; // If collision is true
			}
		}
	}

	Collision result = CheckCollision(*GameBall, *Player);
	if (!GameBall->IsStuck && std::get<0>(result) && !this->isDead)
	{
		SoundEngine->play2D("audio/paddleSquish.wav");
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat distance = (GameBall->Position.x + GameBall->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2);
		// Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = GameBall->Velocity;
		GameBall->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		GameBall->Velocity = glm::normalize(GameBall->Velocity) * glm::length(oldVelocity); 
		// Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
		// Fix sticky paddle
		GameBall->Velocity.y = -1 * abs(GameBall->Velocity.y);

		if (GameBall->IsMagnetized) GameBall->IsStuck = GameBall->IsMagnetized;
		GameBall->IsMagnetized = GL_FALSE;
	}
	std::vector<DeadWeight>::iterator itD;
	int i = 0;
	for (itD = weights.begin(); itD < weights.end(); itD++, i++) {

		result = CheckCollision(*itD, *Player);
		if (std::get<0>(result) && !this->isDead)
		{
			SoundEngine->play2D("audio/weightDeath.wav");
			GameBall->IsStuck = GL_FALSE;
			this->isDead = true; // If collision is true
			Player->Sprite = ResourceMaster::GetTexture("deadSquid");
			Renderer->isIncreasing = true;
		}
	}
}

Direction::Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction::Direction)best_match;
}

Collision Game::CheckCollision(Ball &one, GameObject &two) // AABB - Circle collision
{
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;

	if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, Direction::UP, glm::vec2(0, 0));
}



void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GameState::ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// Move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (GameBall->IsStuck)
					GameBall->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_W])
		{
			for (GameObject &tile : this->Levels[this->Level].Bricks)
			{
				tile.Destroyed = GL_TRUE;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (GameBall->IsStuck)
					GameBall->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			GameBall->IsStuck = false;
	}
	else if (this->Keys[GLFW_KEY_SPACE] && this->State == GameState::MENU)
	{
		this->State = GameState::ACTIVE;
		SoundEngine->removeAllSoundSources();
		SoundEngine->play2D("audio/wormsign.mp3", GL_TRUE);
	}
	else if (this->State == GameState::WIN)
	{
		if (this->Keys[GLFW_KEY_SPACE])
		{
			this->ResetLevel();
			this->ResetPlayer();
			this->State = GameState::ACTIVE;
			SoundEngine->removeAllSoundSources();
			SoundEngine->play2D("audio/wormsign.mp3", GL_TRUE);
		}
	}

}

void Game::ProcessMouseInput(GLfloat xPos)
{
	if (xPos >= 0.0f && xPos <= this->Width - Player->Size.x) Player->Position.x = xPos;
	else if (xPos < 0.0f) Player->Position.x = 0.0f;
	else if (xPos > this->Width - Player->Size.x) Player->Position.x = this->Width - Player->Size.x;
	if (GameBall->IsStuck) GameBall->Position.x = Player->Position.x + Player->Size.x * 0.4f;
}

void Game::Render()
{
	if (this->State == GameState::ACTIVE || this->State == GameState::WIN)
	{
		Effects->BeginRender();
		if (isInvincible)Renderer->DrawSprite(ResourceMaster::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), 0, GL_TRUE);
		else Renderer->DrawSprite(ResourceMaster::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
			);
		// Draw level
		if(GameBall->Position.y >= 0.0f) Particles->Draw();
		this->Levels[this->Level].Draw(*Renderer);
		if(!Player->Sprite.isDead || !this->isDead) Player->Draw(*Renderer, isInvincible);
		GameBall->Draw(*Renderer, isInvincible);
		std::vector<VoidBall>::iterator it;
		int i = 0;
		for (it = voids.begin(); it < voids.end(); it++, i++) {
			it->Draw(*Renderer);
		}
		std::vector<DeadWeight>::iterator itD;
		i = 0;
		for (itD = weights.begin(); itD < weights.end(); itD++, i++) {
			itD->Draw(*Renderer, Height);
		}
		if (this->State == GameState::WIN)
		{
			Text->RenderText(
				"EyeBraun has launched to victory!!!", winPositions[0], Height / 2 - 50.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)
				);
			Text->RenderText(
				"Press Space to turn back time or Esc to die", winPositions[1], Height / 2, 2.0f, glm::vec3(0.0f, 0.0f, 1.0f)
				);
		}
		std::stringstream ss; 
		ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, this->Height - 100.0f, 4.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		Effects->EndRender();
		Effects->Render((GLfloat)glfwGetTime());
	}
	else if (this->State == GameState::MENU) RenderMenu();
}

void Game::RenderMenu()
{
	Renderer->DrawSprite(ResourceMaster::GetTexture("background"),
		glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f), 0, GL_TRUE);
	Text->RenderText("O", menuTitlePositions[0].x, menuTitlePositions[0].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("u", menuTitlePositions[1].x, menuTitlePositions[1].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("t", menuTitlePositions[2].x, menuTitlePositions[2].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("B", menuTitlePositions[3].x, menuTitlePositions[3].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("r", menuTitlePositions[4].x, menuTitlePositions[4].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("e", menuTitlePositions[5].x, menuTitlePositions[5].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("a", menuTitlePositions[6].x, menuTitlePositions[6].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("k", menuTitlePositions[7].x, menuTitlePositions[7].y, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Text->RenderText("EyeBraun needs your help to escape the hellish atmosphere of your screwed up world!", 50.0f, 300.0f, 1.2f, glm::vec3(1.0f));
	Text->RenderText("Use the Mouse or the A and D keys to move your squishy body.", 50.0f, 350.0f, 1.2f, glm::vec3(1.0f));
	Text->RenderText("Press Space to start the game and release EyeBraun.", 50.0f, 400.0f, 1.2f, glm::vec3(1.0f));
	Text->RenderText("Keep EyeBraun alive and use the chaos of the planetary defense system to win!", 50.0f, 450.0f, 1.2f, glm::vec3(1.0f));
	Text->RenderText("Press Esc at any time to destroy the universe.", 50.0f, 500.0f, 1.2f, glm::vec3(1.0f));
	
}


void Game::ResetLevel()
{
	menuTitlePositions[0] = glm::vec2(75.0f, 100.0f);
	menuTitlePositions[1] = glm::vec2(150.0f, 100.0f);
	menuTitlePositions[2] = glm::vec2(225.0f, 100.0f);
	menuTitlePositions[3] = glm::vec2(300.0f, 100.0f);
	menuTitlePositions[4] = glm::vec2(375.0f, 100.0f);
	menuTitlePositions[5] = glm::vec2(450.0f, 100.0f);
	menuTitlePositions[6] = glm::vec2(525.0f, 100.0f);
	menuTitlePositions[7] = glm::vec2(600.0f, 100.0f);
	winPositions[0] = (GLfloat)this->Width;
	winPositions[1] = (GLfloat)this->Width;
	this->Levels[0].Load("Levels/LevelOne.txt", this->Width, GLuint(this->Height * 0.5f));
	this->Lives = MAX_LIFE_COUNT;
}

void Game::ResetPlayer()
{
	// Reset player/ball stats
	this->isDead = false;
	voids.clear();
	weights.clear();
	if (isInvincible)
	{
		SoundEngine->removeSoundSource("audio/windy.mp3");
		SoundEngine->play2D("audio/wormsign.mp3", GL_TRUE);
	}
	isInvincible = GL_FALSE;
	isWon = GL_FALSE;
	Effects->IsChaos = GL_FALSE;
	Effects->IsConfused = GL_FALSE;
	Effects->IsShaken = GL_FALSE;
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player->Sprite.isDead = false;
	Player->Sprite = ResourceMaster::GetTexture("squidPaddle");
	GameBall->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}