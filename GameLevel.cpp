#include "GameLevel.h"
#include "ResourceMaster.h"
#include <fstream>
#include <sstream>



void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	this->Bricks.clear();
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
	for (GameObject &tile : this->Bricks)
	if (!tile.Destroyed)
	{
		if(tile.Type == BrickType::INVINCIBLE) tile.Draw(renderer, GL_TRUE);
		else tile.Draw(renderer);
	}
		
}

GLboolean GameLevel::IsCompleted()
{
	for (GameObject &tile : this->Bricks)
	if (!tile.IsSolid && !tile.Destroyed)
		return GL_FALSE;
	return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint lvlWidth, GLuint lvlHeight)
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = lvlWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = lvlHeight / static_cast<GLfloat>(height);

	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			if (tileData[y][x] == (GLint) BrickType::SOLID)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size,
					ResourceMaster::GetTexture("indestructible"), BrickType::SOLID,
					glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = GL_TRUE;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > BrickType::SOLID)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				glm::vec3 color = glm::vec3(1.0f); // original: white
				float probIncrement = 100.0f / (float)((int)BrickType::TOTAL - 2);
				float probability = (float)(rand() % 100);
				if (probability > (float)(((GLint)BrickType::PLAIN - 2) * probIncrement) && 
					probability < (float)(((GLint)BrickType::PLAIN - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("plain"), BrickType::PLAIN, 
					glm::vec3(0.823529, 0.705882, 0.54902)));
				else if (probability >(float)(((GLint)BrickType::CHAOS - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::CHAOS - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("chaos"), BrickType::CHAOS, 
					color));
				else if (probability >(float)(((GLint)BrickType::DIZZY - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::DIZZY - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("dizzy"), BrickType::DIZZY, 
					glm::vec3(0.27451, 0.509804, 0.705882)));
				else if (probability >(float)(((GLint)BrickType::INVERT - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::INVERT - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("invert"), BrickType::INVERT, color)
					);
				else if (probability >(float)(((GLint)BrickType::MAGNET - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::MAGNET - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("magnet"), BrickType::MAGNET, color)
					);
				else if (probability >(float)(((GLint)BrickType::VOID - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::VOID - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("plain"), BrickType::VOID, glm::vec3(0.05f))
					);
				else if (probability >(float)(((GLint)BrickType::SPEEDY - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::SPEEDY - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("speed"), BrickType::SPEEDY, color)
					);
				else if (probability >(float)(((GLint)BrickType::SLOW - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::SLOW - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("slow"), BrickType::SLOW, color)
					);
				else if (probability >(float)(((GLint)BrickType::INVINCIBLE - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::INVINCIBLE - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("dizzy"), BrickType::INVINCIBLE, color)
					);
				else if (probability >(float)(((GLint)BrickType::EXTEND - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::EXTEND - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("extend"), BrickType::EXTEND, color)
					);
				else if (probability >(float)(((GLint)BrickType::BABY - 2) * probIncrement) &&
					probability < (float)(((GLint)BrickType::BABY - 1) * probIncrement))
					this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("baby"), BrickType::BABY, color)
					);
				else this->Bricks.push_back(
					GameObject(pos, size, ResourceMaster::GetTexture("death"), BrickType::DEATH, color)
					);
				
				
				
			}
		}
	}
}