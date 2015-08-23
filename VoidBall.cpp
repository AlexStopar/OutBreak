#include "VoidBall.h"

const GLfloat VOID_RADIUS_LIMIT = 40.0f;
VoidBall::VoidBall()
: Ball(), isIncreasing(GL_TRUE) {
	this->Color = glm::vec3(0.0f);
	
}

VoidBall::VoidBall(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
: Ball(pos, radius, velocity, sprite), isIncreasing(GL_TRUE) {
	this->Color = glm::vec3(0.0f);
}

glm::vec2 VoidBall::Move(GLfloat dt, GLuint window_width)
{
	float oldRadius = this->Radius;
	if(isIncreasing) this->Radius += this->Velocity.x * dt;
	else this->Radius -= this->Velocity.x * dt;
	this->Size = glm::vec2(this->Radius * 2, this->Radius * 2);
	this->Position = this->Position - glm::vec2(Radius - oldRadius, Radius - oldRadius);
	if (this->Radius >= VOID_RADIUS_LIMIT) isIncreasing = GL_FALSE;
	return this->Position;
}

void VoidBall::Draw(SpriteRenderer &renderer)
{
	if(this->Radius > 1.0f) renderer.DrawSprite(this->Sprite, this->Position,
		this->Size, this->Rotation, this->Color);
}