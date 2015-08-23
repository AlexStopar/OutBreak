#ifndef VOIDBALL_H
#define VOIDBALL_H
#include "BallObject.h"

class VoidBall : public Ball
{
public:
	VoidBall();
	VoidBall(glm::vec2 pos, GLfloat radius,
		glm::vec2 velocity, Texture2D sprite);
	glm::vec2 Move(GLfloat dt, GLuint window_width);
	void Reset(glm::vec2 position, glm::vec2 velocity);
	void Draw(SpriteRenderer &renderer);
	GLboolean isIncreasing;
private:
};

#endif