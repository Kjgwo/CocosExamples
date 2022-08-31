#ifndef __BULLET_H__
#define __BULLET_H__

#include "stdafx.h"

class Bullet :public Node {
private:
	PhysicsBody* body;
public:
	static Bullet* create(int bitmask, int tag);
	virtual bool init(int bitmask, int tag); // 파라미터가 있기에 override X

	PhysicsBody* getBody();
};

#endif