#include <array>

#include <GLFW/glfw3.h>

#include "window.hpp"
#include "gamestate.hpp"

namespace
{

enum
{
	FRAME_PLAYERCENTER,
	FRAME_PLAYERRIGHT,
	FRAME_PLAYERLEFT,
	FRAME_ENEMY1,
	FRAME_ENEMY2,
	FRAME_HEALTHPOW,
	FRAME_MISSILEPOW,
	FRAME_DOUBLEGUNPOW,
	FRAME_FASTERGUNPOW,
	FRAME_MISSILE,
	FRAME_PLAYERBULLET,
	FRAME_ENEMYBULLET,
};

constexpr std::array frames = {
	Frame{
		{ 48.f, 64.f },
		{  0.f / 288.f,  0.f / 104.f },
		{ 48.f / 288.f, 64.f / 104.f },
	},
	Frame{
		{ 48.f, 64.f },
		{ 48.f / 288.f,  0.f / 104.f },
		{ 48.f / 288.f, 64.f / 104.f },
	},
	Frame{
		{ 48.f, 64.f },
		{ 96.f / 288.f,  0.f / 104.f },
		{ 48.f / 288.f, 64.f / 104.f },
	},
	Frame{
		{ 84.f, 64.f },
		{ 144.f / 288.f,  0.f / 104.f },
		{  84.f / 288.f, 64.f / 104.f },
	},
	Frame{
		{ 60.f, 64.f },
		{ 228.f / 288.f,  0.f / 104.f },
		{  60.f / 288.f, 64.f / 104.f },
	},
	Frame{
		{ 40.f, 40.f },
		{  0.f / 288.f, 64.f / 104.f },
		{ 40.f / 288.f, 40.f / 104.f },
	},
	Frame{
		{ 40.f, 40.f },
		{ 40.f / 288.f, 64.f / 104.f },
		{ 40.f / 288.f, 40.f / 104.f },
	},
	Frame{
		{ 40.f, 40.f },
		{ 80.f / 288.f, 64.f / 104.f },
		{ 40.f / 288.f, 40.f / 104.f },
	},
	Frame{
		{ 40.f, 40.f },
		{ 120.f / 288.f, 64.f / 104.f },
		{  40.f / 288.f, 40.f / 104.f },
	},
	Frame{
		{ 15.f, 32.f },
		{ 160.f / 288.f, 64.f / 104.f },
		{  15.f / 288.f, 32.f / 104.f },
	},
	Frame{
		{ 3.f, 14.f },
		{ 175.f / 288.f, 64.f / 104.f },
		{   3.f / 288.f, 14.f / 104.f },
	},
	Frame{
		{ 3.f, 14.f },
		{ 178.f / 288.f, 64.f / 104.f },
		{   3.f / 288.f, 14.f / 104.f },
	}
};

constexpr std::array expFrames = {
	Frame{
		{ 96.f, 96.f },
		{   0.f / 480.f,   0.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{  96.f / 480.f,   0.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 192.f / 480.f,   0.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 288.f / 480.f,   0.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 384.f / 480.f,   0.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{   0.f / 480.f,  96.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{  96.f / 480.f,  96.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 192.f / 480.f,  96.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 288.f / 480.f,  96.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 384.f / 480.f,  96.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{   0.f / 480.f, 192.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{  96.f / 480.f, 192.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 192.f / 480.f, 192.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 288.f / 480.f, 192.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 384.f / 480.f, 192.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{   0.f / 480.f, 288.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{  96.f / 480.f, 288.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 192.f / 480.f, 288.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 288.f / 480.f, 288.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
	Frame{
		{ 96.f, 96.f },
		{ 384.f / 480.f, 288.f / 384.f },
		{  96.f / 480.f,  96.f / 384.f }
	},
};

constexpr std::array enemyWaves = {
	EnemyWave{ EnemyType::Eagle,  100.f,  20.f, 5.f, 2 },
	EnemyWave{ EnemyType::Raptor, 100.f,  60.f, 1.f, 6 },
	EnemyWave{ EnemyType::Eagle,  500.f, 100.f, 0.f, 1 },
};
}

GameState::GameState()
{
	world.player.pos = (glm::vec2(640.f, 480.f) - glm::vec2(48.f, 64.f))
		* glm::vec2(0.5f, 0.8f);
	world.player.maxBulletCount = 3;

	world.mapPosition = 0.f;
	world.nextWave = 0;
}

bool
GameState::update(float dt)
{
	updateMap(dt);
	updateWaves(dt);
	updateEnemies(dt);
	updateBullets(dt);
	updatePlayer(world.player, dt);
	updateExplosions(dt);
	collideBulletsEnemies();
	return true;
}

void
GameState::updateMap(float dt)
{
	// scroll the map
	world.mapPosition += 4.f * dt;
	if (world.mapPosition >= 480.f * 9.f)
	{
		world.mapPosition = 480.f * 9.f;
	}
}

void
GameState::updateWaves(float dt)
{
	// span new enemies
	while (world.nextWave < enemyWaves.size()
	       && world.mapPosition >= enemyWaves[world.nextWave].spawnY)
	{
		world.activeWaves.push_back(enemyWaves[world.nextWave]);
		world.nextWave++;
	}
	auto wit = world.activeWaves.begin();
	while (wit != world.activeWaves.end())
	{
		wit->spawnElapsed -= dt;
		if (wit->spawnElapsed < 0.f)
		{
			wit->spawnElapsed += wit->spawnDelay;
			createEnemy(*wit);
			wit->enemyCount--;
			if (wit->enemyCount == 0)
			{
				wit = world.activeWaves.erase(wit);
				continue;
			}
		}
		++wit;
	}
}

void
GameState::updateEnemies(float dt)
{
	// update the enemies
	auto e = world.enemies.begin();
	while (e != world.enemies.end())
	{
		updateEnemy(*e, dt);

		// check if the enemy is outside the screen
		const auto &frame = frames[e->frameIndex];
		if (e->pos.x < 0.f
		    || e->pos.x + frame.size.x > 640.f
		    || e->pos.y > 480.f)
		{
			e = world.enemies.erase(e);
		}
		else
		{
			++e;
		}
	}
}

void
GameState::updateBullets(float dt)
{
	// update the bullets
	auto b = world.playerBullets.begin();
	while (b != world.playerBullets.end())
	{
		b->pos += b->vel * dt;
		if (b->pos.y < 0.f)
		{
			b = world.playerBullets.erase(b);
		}
		else
		{
			++b;
		}
	}
}

void
GameState::updatePlayer(Player &player, float dt)
{
	switch (player.state)
	{
	case PlayerState::Flying:
		updatePlayerPosition(player, dt);
		if ((world.inputChange & INPUT_SPACE)
		    && world.playerBullets.size() < player.maxBulletCount)
		{
			player.state = PlayerState::Firing;
			player.delay = 0.5f;
			firePlayerBullet(player);
		}
		break;
	case PlayerState::Firing:
		updatePlayerPosition(player, dt);
		world.player.delay -= dt;
		if (world.player.delay > 0)
		{
			world.player.state = PlayerState::Flying;
		}
		break;
	case PlayerState::Dead:
		break;
	}
}

void
GameState::updatePlayerPosition(Player &player, float dt)
{
	glm::vec2 pvel(0.f);
	if (world.inputStatus & INPUT_UP)
	{
		pvel.y -= 180.f;
	}
	if (world.inputStatus & INPUT_DOWN)
	{
		pvel.y += 180.f;
	}
	if (world.inputStatus & INPUT_LEFT)
	{
		pvel.x -= 180.f;
	}
	if (world.inputStatus & INPUT_RIGHT)
	{
		pvel.x += 180.f;
	}
	// normalize the velocity
	if (pvel.x != 0.f && pvel.y != 0.f)
	{
		pvel *= 1.f / std::sqrtf(2.f);
	}
	// update the player frame
	if (pvel.x < 0.f)
	{
		player.frameIndex = FRAME_PLAYERRIGHT;
	}
	else if (pvel.x > 0.f)
	{
		player.frameIndex = FRAME_PLAYERLEFT;
	}
	else
	{
		world.player.frameIndex = FRAME_PLAYERCENTER;
	}
	// update the player position
	pvel = player.pos + pvel * dt;
	player.pos.x = glm::clamp(pvel.x, 10.f, 640.f - 48.f - 10.f);
	player.pos.y = glm::clamp(pvel.y, 10.f, 480.f - 64.f - 10.f);
}

void
GameState::firePlayerBullet(Player &player)
{
	PlayerBullet pb;
	pb.type = player.bulletType;
	pb.pos = player.pos + glm::vec2(24.f - 2.f, -14.f);
	pb.vel = glm::vec2(0, -300.f);
	pb.frameIndex = FRAME_PLAYERBULLET;
	world.playerBullets.push_back(pb);
}

void
GameState::createEnemy(const EnemyWave &w)
{
	Enemy e{};
	switch (w.type)
	{
	case EnemyType::Eagle:
		e.type = w.type;
		e.pos.x = w.spawnX;
		e.pos.y = 0.f;
		e.vel = glm::vec2(0.f, 60.f);
		e.state = 0;
		break;
	case EnemyType::Raptor:
		e.type = w.type;
		e.pos.x = w.spawnX;
		e.pos.y = 0.f;
		e.state = 0;
		break;
	case EnemyType::Avenger:
		abort();
	}
	world.enemies.push_back(e);
}

void
GameState::updateEnemy(Enemy &e, float dt)
{
	switch (e.type)
	{
	case EnemyType::Eagle:
		switch (e.state)
		{
		case 0:
			e.frameIndex = FRAME_ENEMY1;
			e.pos.y -= frames[e.frameIndex].size.y;
			e.vel = glm::vec2(0.f, 60.f);
			e.state++;
			break;
		case 1:
			e.pos += e.vel * dt;
			break;
		}
		break;
	case EnemyType::Raptor:
		switch (e.state)
		{
		case 0:
			e.frameIndex = FRAME_ENEMY2;
			e.pos.y -= frames[e.frameIndex].size.y;
			e.xCenter = 320.f - frames[e.frameIndex].size.x * 0.5f;
			e.vel.y = 80.f;
			e.state++;
			break;
		case 1:
			e.vel.x += (e.xCenter - e.pos.x) * dt;
			e.pos += e.vel * dt;
			break;
		}
		break;
	case EnemyType::Avenger:
		abort();
	}
}

bool
GameState::handleEvent(const Event &event)
{
	if (const auto ep(std::get_if<KeyPressed>(&event)); ep)
	{
		if (ep->key == GLFW_KEY_ESCAPE)
		{
			world.states.pushState(StateID::Pause);
			return true;
		}
	}
	return false;
}

void
GameState::draw(RenderTarget &target)
{
	target.clear(Color::fromRGBA(0, 0, 40, 100));
        // NOTE: draw the world
	target.beginFrames(world.textures.get(TextureID::Entities));
	for (const auto &e : world.enemies)
	{
		target.addFrame(frames[e.frameIndex], e.pos);
	}
	for (const auto &b : world.playerBullets)
	{
		target.addFrame(frames[b.frameIndex], b.pos);
	}
	target.addFrame(frames[world.player.frameIndex], world.player.pos);
	target.endFrames();

	// NOTE: Explosions use a different texture
	target.beginFrames(world.textures.get(TextureID::Explosion));
	for (const auto &e : world.explosions)
	{
		target.addFrame(expFrames[e.frameIndex], e.pos);
	}
	target.endFrames();
}

void
GameState::getPlayerBulletRect(const PlayerBullet &b, FloatRect &r)
{
	r.pos = b.pos;
	r.size = glm::vec2(3.f, 14.f);
}

void
GameState::getEnemyRect(const Enemy &e, FloatRect &r)
{
	switch (e.type)
	{
	case EnemyType::Eagle:
		r.pos = e.pos + glm::vec2(10.f, 10.f);
		r.size = glm::vec2(64.f, 44.f);
		break;
	case EnemyType::Raptor:
		r.pos = e.pos + glm::vec2(5.f, 10.f);
		r.size = glm::vec2(50.f, 44.f);
		break;
	case EnemyType::Avenger:
		abort();
	}
}

void
GameState::collideBulletsEnemies()
{
	auto e = world.enemies.begin();
	while (e != world.enemies.end())
	{
		FloatRect enemyRect;
		getEnemyRect(*e, enemyRect);

		bool collision = false;
		auto b = world.playerBullets.begin();
		while (b != world.playerBullets.end())
		{
			FloatRect bulletRect;
			getPlayerBulletRect(*b, bulletRect);

			if (enemyRect.overlaps(bulletRect))
			{
				collision = true;
				break;
			}

			++b;
		}

		if (collision)
		{
			createExplosion(enemyRect.center());
			e = world.enemies.erase(e);
			world.playerBullets.erase(b);
		}
		else
		{
			++e;
		}
	}
}

void
GameState::createExplosion(glm::vec2 pos)
{
	world.explosions.emplace_back(
		pos - glm::vec2(96.f) * 0.5f,
		0,
		.03333f);
}

void
GameState::updateExplosions(float dt)
{
	auto ex = world.explosions.begin();
	while (ex != world.explosions.end())
	{
		ex->delay -= dt;
		if (ex->delay <= 0.f)
		{
			ex->delay += .03333f;
			ex->frameIndex++;
			if (ex->frameIndex >= expFrames.size())
			{
				ex = world.explosions.erase(ex);
				continue;
			}
		}
		++ex;
	}
}
