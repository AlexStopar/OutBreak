#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "SpriteRenderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
enum BrickType { NONE, SOLID, PLAIN, CHAOS, DIZZY, INVERT, MAGNET, 
	VOID, SPEEDY, SLOW, INVINCIBLE, EXTEND, BABY, DEATH, TOTAL };

class GameObject
{
public:
	// Object state
	glm::vec2   Position, Size, Velocity;
	GLfloat Width;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	BrickType Type;
	GLboolean   Destroyed;
	// Render state
	Texture2D   Sprite;
	// Constructor(s)
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, BrickType type = BrickType::NONE,
		glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f)
		);
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer, GLboolean isRainbow = GL_FALSE);
};

#endif