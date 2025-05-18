#pragma once

#include "rect.hpp"
#include "state.hpp"
#include "world.hpp"

class GameState: public State
{
public:
	GameState();

	bool update(float dt) override;
	bool handleEvent(const Event &event) override;
	void draw(RenderTarget &target) override;

private:
	void updateMap(float dt);
	void updateWaves(float dt);
	void updateEnemies(float dt);
	void updateBullets(float dt);

	void updatePlayer(Player &player, float dt);
	void updatePlayerPosition(Player &player, float dt);
	void firePlayerBullet(Player &player);

	void createEnemy(const EnemyWave &w);
	void updateEnemy(Enemy &e, float dt);

	void getPlayerBulletRect(const PlayerBullet &b, FloatRect &r);
	void getEnemyRect(const Enemy &e, FloatRect &r);
	void collideBulletsEnemies();

	void createExplosion(glm::vec2 pos);
	void updateExplosions(float dt);
};
