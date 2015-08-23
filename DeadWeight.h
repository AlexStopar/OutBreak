#ifndef DEADWEIGHT_H
#define DEADWEIGHT_H
#include "BallObject.h"

class DeadWeight : public Ball
{
public:
	DeadWeight();
	DeadWeight(glm::vec2 pos, GLfloat radius,
		glm::vec2 velocity, Texture2D sprite);
	glm::vec2 Move(GLfloat dt, GLuint window_height);
	void Reset(glm::vec2 position, glm::vec2 velocity);
	void Draw(SpriteRenderer &renderer, GLuint window_height);
private:
};

#endif