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

const Frame frames[] = {
	{
		{ 48.f, 64.f },
		{  0.f / 288.f,  0.f / 104.f },
		{ 48.f / 288.f, 64.f / 104.f },
	},
	{
		{ 48.f, 64.f },
		{ 48.f / 288.f,  0.f / 104.f },
		{ 48.f / 288.f, 64.f / 104.f },
	},
	{
		{ 48.f, 64.f },
		{ 96.f / 288.f,  0.f / 104.f },
		{ 48.f / 288.f, 64.f / 104.f },
	},
	{
		{ 84.f, 64.f },
		{ 144.f / 288.f,  0.f / 104.f },
		{  84.f / 288.f, 64.f / 104.f },
	},
	{
		{ 60.f, 64.f },
		{ 228.f / 288.f,  0.f / 104.f },
		{  60.f / 288.f, 64.f / 104.f },
	},
	{
		{ 40.f, 40.f },
		{  0.f / 288.f, 64.f / 104.f },
		{ 40.f / 288.f, 40.f / 104.f },
	},
	{
		{ 40.f, 40.f },
		{ 40.f / 288.f, 64.f / 104.f },
		{ 40.f / 288.f, 40.f / 104.f },
	},
	{
		{ 40.f, 40.f },
		{ 80.f / 288.f, 64.f / 104.f },
		{ 40.f / 288.f, 40.f / 104.f },
	},
	{
		{ 40.f, 40.f },
		{ 120.f / 288.f, 64.f / 104.f },
		{  40.f / 288.f, 40.f / 104.f },
	},
	{
		{ 15.f, 32.f },
		{ 160.f / 288.f, 64.f / 104.f },
		{  15.f / 288.f, 32.f / 104.f },
	},
	{
		{ 3.f, 14.f },
		{ 175.f / 288.f, 64.f / 104.f },
		{   3.f / 288.f, 14.f / 104.f },
	},
	{
		{ 3.f, 14.f },
		{ 178.f / 288.f, 64.f / 104.f },
		{   3.f / 288.f, 14.f / 104.f },
	}

};
}

GameState::GameState()
{
	world.player.pos = (glm::vec2(640.f, 480.f) - glm::vec2(48.f, 64.f))
		* glm::vec2(0.5f, 0.8f);
	world.player.maxBulletCount = 3;
}

bool
GameState::update(Time dt)
{
	auto b = world.playerBullets.begin();
	while (b != world.playerBullets.end())
	{
		b->pos += b->vel * dt.asSeconds();
		if (b->pos.y < 0.f)
		{
			b = world.playerBullets.erase(b);
		}
		else
		{
			++b;
		}
	}
	updatePlayer(world.player, dt);
	return true;
}

void
GameState::updatePlayer(Player &player, Time dt)
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
		world.player.delay -= dt.asSeconds();
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
GameState::updatePlayerPosition(Player &player, Time dt)
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
	pvel = player.pos + pvel * dt.asSeconds();
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
	for (const auto &b : world.playerBullets)
	{
		target.addFrame(frames[b.frameIndex], b.pos);
	}
	target.addFrame(frames[world.player.frameIndex], world.player.pos);
	target.endFrames();
}
