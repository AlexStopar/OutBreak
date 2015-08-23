#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "GameObject.h"
#include <irrKlang.h>
using namespace irrklang;

enum Speed { GREY, BLUE, GREEN, YELLOW, ORANGE, RED };
class Ball : public GameObject
{
public:
	// Ball state	
	GLfloat Radius;
	GLboolean IsStuck;
	GLboolean IsMagnetized;
	Texture2D veins;
	Texture2D iris;
	Texture2D pupil;
	GLint hitCounter = 0;
	const GLint HIT_LIMIT = 10;
	Speed speed;
	glm::vec2 pupilSize = glm::vec2(0.5f, 2.0f);

	Ball();
	Ball(glm::vec2 pos, GLfloat radius, 
		glm::vec2 velocity, Texture2D sprite);
	void InitSubTextures();
	glm::vec2 Move(GLfloat dt, GLuint window_width, GLuint window_height, 
		ISoundEngine& engine, GLboolean isInvincible = GL_FALSE, GLboolean isWon = GL_FALSE);
	void Reset(glm::vec2 position, glm::vec2 velocity);
	void Draw(SpriteRenderer &renderer, GLboolean isRainbow = GL_FALSE);
	void SwitchGears(GLboolean isBlock = GL_FALSE, GLboolean isSpeed = GL_TRUE);
};

#endif