#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"

class SpriteRenderer

{
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();
	void DrawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f), GLint eyeColor = 0, GLboolean isRainbow = GL_FALSE, glm::vec2 alternateScale = glm::vec2(1.0f, 1.0f));
	void IncrementDelay(float dt);
	bool isIncreasing = true;
private:	
	Shader shader;
	GLuint quadVAO;
	void initRenderData();
};

#endif