#ifndef __ENEMY_COMPONENT_H__
#define __ENEMY_COMPONENT_H__

#include "stdafx.h"

enum class EnemyAttackType {
	NORMAL
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
};

class EnemyMovementRoutine : public Component {

};

#endif