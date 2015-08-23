#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <glfw3.h>
#include <tuple>
#include <vector>
#include "GameLevel.h"
#include "BallObject.h"



enum GameState{
	ACTIVE,
	MENU,
	WIN
};

namespace Direction
{
	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
}


typedef std::tuple<GLboolean, Direction::Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(80, 40);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;
const GLint MAX_LIFE_COUNT = 3;

class Game
{
public:
	std::vector<GameLevel> Levels;
	GLuint Level;
	GLuint Lives;
	GameState State;
	GLboolean Keys[1024];
	bool isDead = false;
	GLuint Width, Height;
	Game(GLuint width, GLuint height, GLuint lives = MAX_LIFE_COUNT);
	~Game();
	void InitGame();
	void DoCollisions();
	GLboolean FindLastUsedVoid(GameObject &obj);
	GLboolean FindLastUsedWeight(GameObject &obj);
	Direction::Direction VectorDirection(glm::vec2 target);
	Collision CheckCollision(Ball &one, GameObject &two);
	void ProcessInput(GLfloat deltaTime);
	void ProcessMouseInput(GLfloat xPos);
	void Update(GLfloat deltaTime);
	void Render();
	void SettleDestructibleEffect(GameObject &obj);
	void ResetLevel();
	void ResetPlayer();
	void RenderMenu();
};

#endif