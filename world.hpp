#pragma once

#include <array>
#include <memory>
#include <vector>

#include "rendertarget.hpp"
#include "resources.hpp"
#include "resourceholder.hpp"
#include "font.hpp"
#include "texture.hpp"
#include "statestack.hpp"

#define INPUT_UP    0x01
#define INPUT_DOWN  0x02
#define INPUT_LEFT  0x04
#define INPUT_RIGHT 0x08
#define INPUT_SPACE 0x10

struct Frame
{
	glm::vec2 size;
	glm::vec2 uvPos;
	glm::vec2 uvSize;
};

struct EnemyBullet
{
	glm::vec2 pos;
	glm::vec2 vel;
	int frameIndex;
	float delay;
};

enum class EnemyType
{
	Eagle,
	Raptor,
	Avenger,
};

struct Enemy
{
	EnemyType type;
	int state;
	glm::vec2 pos;
	glm::vec2 vel;
	float xCenter;
	float yCenter;
	int frameIndex;
};

enum class PlayerBulletType
{
	Gun,
	Cannon,
	Missile,
};

struct PlayerBullet
{
	PlayerBulletType type;
	glm::vec2 pos;
	glm::vec2 vel;
	int frameIndex;
	float delay;
};

enum class PlayerState
{
	Flying,
	Firing,
	Dead,
};

struct Player
{
	PlayerState state;
	glm::vec2 pos;
	int frameIndex;
	PlayerBulletType bulletType;
	unsigned maxBulletCount;
	float delay;
};

struct World
{
	std::vector<Enemy> enemies;
	std::vector<EnemyBullet> enemyBullets;
	std::vector<PlayerBullet> playerBullets;

	unsigned inputStatus;
	unsigned inputChange;

	Player player;

	std::unique_ptr<Window> window;
	TextureHolder textures;
	FontHolder fonts;
	StateStack states;
};

extern World world;
