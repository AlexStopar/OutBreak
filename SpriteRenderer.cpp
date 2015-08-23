#include "SpriteRenderer.h"

float rainbowOffset = 0.0f;
float rainbowColorAlpha = 0.0f;
bool isColorIncreasing = true;
float rainbowSpeed = 0.0002f;
float scrollTime = 0.0f;
float scrollSpeed = 0.00001f;
bool isAnimating = false;
bool isScrolling = false;



SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::IncrementDelay(float dt)
{
	
}

void SpriteRenderer::initRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	
	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
		4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position,
	glm::vec2 size, GLfloat rotate, glm::vec3 color, GLint eyeColor, GLboolean isRainbow, glm::vec2 alternateScale)
{
	this->shader.Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	if (texture.Name == "background") isScrolling = true;
	else isScrolling = false;
	if (texture.Name == "deadSquid" || texture.Name == "squidPaddle" || texture.Name == "baby") isAnimating = true;
	else isAnimating = false;
	
	this->shader.SetMatrix4("model", model);
	this->shader.SetInteger("isAnimating", (GLint)isAnimating);
	this->shader.SetInteger("isRainbow", (GLint)isRainbow);
	this->shader.SetInteger("isScrolling", (GLint)isScrolling);
	this->shader.SetInteger("eyeColor", eyeColor);
	this->shader.SetFloat("scrollTime", scrollTime);
	this->shader.SetFloat("frameNumberMax", (GLfloat)texture.frameNumber);
	this->shader.SetFloat("currentFrameNumber", (GLfloat)(texture.currentFrameNumber));
	this->shader.SetFloat("rainbowOffset", rainbowOffset);
	this->shader.SetFloat("rainbowColorAlpha", rainbowColorAlpha);

	rainbowOffset += rainbowSpeed;
	if (rainbowOffset > 1.0f) rainbowOffset = 0.0f;
	if (isColorIncreasing) rainbowColorAlpha += rainbowSpeed;
	else rainbowColorAlpha -= rainbowSpeed;
	if (rainbowColorAlpha < 0.0f) isColorIncreasing = true;
	else if (rainbowColorAlpha > 1.0f) isColorIncreasing = false;

	scrollTime += scrollSpeed;
	if (scrollTime > 1.0f) scrollTime = 0.0f;
	texture.delay += 0.5f;
	if (texture.delay > texture.delayMax && isAnimating)
	{
		if (texture.isIncreasing) texture.currentFrameNumber++;
		else texture.currentFrameNumber--;
		if (texture.currentFrameNumber == 0) texture.isIncreasing = true;
		else if (texture.currentFrameNumber == texture.frameNumber - 1 && texture.Name != "baby")
		{
 			texture.isIncreasing = false;
			texture.isDead = true;
		}
		else if (texture.currentFrameNumber == texture.frameNumber - 1) texture.currentFrameNumber = 0;
		texture.delay = 0.0f;
	}
	// Render textured quad
	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}