#include "stdafx.h"
#include "EnemyComponent.h"
#include "SceneIngame.h"
#include "Bullet.h"

EnemyAttackRoutine * EnemyAttackRoutine::create(EnemyAttackType t)
{
	auto ret = new EnemyAttackRoutine();
	if (ret && ret->init(t)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool EnemyAttackRoutine::init(EnemyAttackType t)
{
	if (!Component::init()) return false;

	this->type = t;

	return true;
}

void EnemyAttackRoutine::update(float dt)
{
	switch (this->type) {
	case EnemyAttackType::NORMAL: logic_normal(dt); break;
	}
}

void EnemyAttackRoutine::logic_normal(float dt)
{
	elapsed += dt;


	if (elapsed > 1.0f) {
		elapsed = 0.0f;
		SceneIngame* scene = (SceneIngame*)Director::getInstance()->getRunningScene(); // ���� �� ��������
		auto player = scene->getPlayer();
		if (player == nullptr) return;

		auto bullet = Bullet::create(ENEMY_BULLET_MASK, TAG_ENEMY_BULLET);
		bullet->setPosition(getOwner()->getPosition()); // getOwner() : �ش� component�� ���� node�� ������ (Enemy��ġ)
		scene->addChild(bullet);

		Vec2 pos = player->getPosition() - getOwner()->getPosition(); // Player�� ��ġ Enemy�� ��ġ�� ���� ���͸� ���Ѵ�.
		pos = pos.getNormalized() * ENEMY_BULLET_SPEED; // getNormalized() : ���̰� 1�� ���ͷ� ���� (��������)
		bullet->getBody()->setVelocity(pos);

		bullet->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr));
	}
}
