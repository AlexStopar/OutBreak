#include "BallObject.h"
#include "ResourceMaster.h"

GLfloat invincibleSpeedMultiplier = 2.0f;

Ball::Ball()
: GameObject(), Radius(12.5f), IsStuck(true), IsMagnetized(false), speed(Speed::BLUE)  {
	InitSubTextures();
	
}

Ball::Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, BrickType::NONE, glm::vec3(1.0f), velocity), 
Radius(radius), IsStuck(true), IsMagnetized(false), speed(Speed::BLUE) {
	InitSubTextures();
}

void Ball::SwitchGears(GLboolean isBlock, GLboolean isSpeed)
{
	this->hitCounter = 0;
	if (!isBlock)
	{
		if (this->speed != Speed::RED) this->speed = static_cast<Speed>(((GLint) this->speed) + 1);
		else this->speed = Speed::BLUE;
	}
	else
	{
		if (isSpeed && this->speed != Speed::RED) this->speed = static_cast<Speed>(((GLint) this->speed) + 1);
		else if (!isSpeed && this->speed != Speed::BLUE) this->speed = static_cast<Speed>(((GLint) this->speed) - 1);
	}
}


void Ball::InitSubTextures()
{
	veins = ResourceMaster::GetTexture("veins");
	iris = ResourceMaster::GetTexture("iris");
	pupil = ResourceMaster::GetTexture("pupil");
}
glm::vec2 Ball::Move(GLfloat dt, GLuint window_width, GLuint window_height, 
	ISoundEngine& engine, GLboolean isInvincible, GLboolean isWon)
{
	glm::vec2 moveVelocity = this->Velocity * (1.0f + (((GLfloat) this->speed)/5.0f));
	if (isInvincible) moveVelocity *= invincibleSpeedMultiplier;
	if (!this->IsStuck)
	{
		this->Position += moveVelocity * dt;
		// Check if outside window bounds; if so, reverse velocity and restore at correct position
		if (this->Position.x <= 0.0f)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = 0.0f;
			engine.play2D("audio/hit.wav");
		}
		else if (this->Position.x + this->Size.x >= window_width)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = window_width - this->Size.x;
			engine.play2D("audio/hit.wav");
		}
		if (this->Position.y <= 0.0f && !isWon)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = 0.0f;
			engine.play2D("audio/hit.wav");
		}
		else if (this->Position.y + this->Size.y >= window_height && isInvincible)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = window_height - this->Size.y;
			engine.play2D("audio/hit.wav");
		}

	}
	return this->Position;
}

void Ball::Draw(SpriteRenderer &renderer, GLboolean isRainbow)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	renderer.DrawSprite(veins, this->Position, this->Size, this->Rotation, this->Color);
	renderer.DrawSprite(iris, this->Position, this->Size, this->Rotation, this->Color, (GLint) this->speed, isRainbow);
	renderer.DrawSprite(pupil, this->Position, 
		this->Size, this->Rotation, this->Color);
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->IsStuck = true;
	speed = Speed::BLUE;
}