#include "DeadWeight.h"

GLfloat DEATH_ACCELERATION = 100.0f;
GLfloat DEATH_ROTATION = 0.5f;

DeadWeight::DeadWeight()
: Ball() {}

DeadWeight::DeadWeight(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
: Ball(pos, radius, velocity, sprite){}

glm::vec2 DeadWeight::Move(GLfloat dt, GLuint window_height)
{
	if (this->Position.y > window_height + this->Size.y) return this->Position;
	this->Velocity.y += DEATH_ACCELERATION * dt;
	this->Position += glm::vec2(0, this->Velocity.y * dt);
	this->Rotation += DEATH_ROTATION;
	return this->Position;
}

void DeadWeight::Draw(SpriteRenderer &renderer, GLuint window_height)
{
	if (this->Position.y < window_height + this->Size.y) renderer.DrawSprite(this->Sprite, this->Position,
		this->Size, this->Rotation, this->Color);
}