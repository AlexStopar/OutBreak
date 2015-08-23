#include <GL/glew.h>
#include <iostream>
#include <SOIL.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H 

struct Character {
	GLuint TextureID;
	glm::ivec2 GlyphSize;
	glm::ivec2 GlyphBearing;
	GLuint GlyphAdvance;
};

std::map<GLchar, Character> Characters;