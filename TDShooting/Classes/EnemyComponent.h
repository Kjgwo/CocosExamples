#ifndef __ENEMY_COMPONENT_H__
#define __ENEMY_COMPONENT_H__

#include "stdafx.h"
#include "Unit.h"

enum class EnemyAttackType {
	NORMAL, DOUBLE, TRIPLE
};

class EnemyAttackRoutine : public Component {
private:
	EnemyAttackType type;
	float elapsed = 0;
public:
	static EnemyAttackRoutine* create(EnemyAttackType t);
	virtual bool init(EnemyAttackType t);

	virtual void update(float dt);

	void logic_normal(float dt);
	void logic_double(float dt);
	void logic_triple(float dt);

	void createBulletAndGo(float speed, float delta_angle);
};

enum class EnemyMovementType {
	FROM_TOP, FROM_LEFT, FROM_RIGHT
};

class EnemyMovementRoutine : public Component {
private:
	Unit* u = nullptr;
	EnemyMovementType type;
public:
	static EnemyMovementRoutine* create(EnemyMovementType t);
	virtual bool init(EnemyMovementType t);

	virtual void onEnter() override;

	virtual void update(float dt);

	void logic_from_top(float dt);
	void logic_from_left(float dt);
	void logic_from_right(float dt);
};

#endif