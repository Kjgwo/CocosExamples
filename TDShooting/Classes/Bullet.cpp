#include "stdafx.h"
#include "bullet.h"

Bullet * Bullet::create(int bitmask, int tag)
{
	auto ret = new Bullet();
	if (ret && ret->init(bitmask, tag)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool Bullet::init(int bitmask, int tag)
{
	if (!Node::init()) return false;

	body = PhysicsBody::createCircle(5.0);
	body->setCollisionBitmask(0); // 투사체 끼리 충돌 X
	body->setContactTestBitmask(bitmask);
	body->setTag(tag);
	addComponent(body);


	return true;
}

PhysicsBody * Bullet::getBody()
{
	return this->body;
}
