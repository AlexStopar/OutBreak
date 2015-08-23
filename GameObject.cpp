#include "GameObject.h"


GameObject::GameObject()
: Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, BrickType type, glm::vec3 color, glm::vec2 velocity)
: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), Type(type) { }

void GameObject::Draw(SpriteRenderer &renderer, GLboolean isRainbow)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color, 0, isRainbow);
}