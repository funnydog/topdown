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
	void updateMap(Time dt);
	void updateWaves(Time dt);
	void updateEnemies(Time dt);
	void updateBullets(Time dt);

	void updatePlayer(Player &player, Time dt);
	void updatePlayerPosition(Player &player, Time dt);
	void firePlayerBullet(Player &player);

	void createEnemy(const EnemyWave &w);
	void updateEnemy(Enemy &e, Time dt);

};
