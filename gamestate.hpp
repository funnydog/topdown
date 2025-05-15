#pragma once

#include "state.hpp"
#include "world.hpp"

class GameState: public State
{
public:
	GameState();

	bool update(Time dt) override;
	bool handleEvent(const Event &event) override;
	void draw(RenderTarget &target) override;

private:
	void updatePlayer(Player &player, Time dt);
	void updatePlayerPosition(Player &player, Time dt);
	void firePlayerBullet(Player &player);
};
