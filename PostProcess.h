#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "SpriteRenderer.h"
#include "Shader.h"

class PostProcessor
{
public:
	Shader PostProcShader;
	Texture2D Texture;
	GLuint Width, Height;
	GLboolean IsConfused, IsChaos, IsShaken;

	PostProcessor(Shader shader, GLuint width, GLuint height);
	void BeginRender();
	void EndRender();
	void Render(GLfloat time);

private:
	GLuint MSFBO, FBO;
	GLuint RBO;
	GLuint VAO;
	void initRenderData();
};

#endif