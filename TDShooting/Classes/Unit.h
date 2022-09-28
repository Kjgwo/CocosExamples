#ifndef __UNIT_H__
#define __UNIT_H__

#include "stdafx.h"

class Unit : public Node {
private:
	DrawNode* dn;
	PhysicsBody* body;
protected:
	Sprite* spr;
public:
	static Unit* create(const Size& size, int bitmask, int tag); // 충돌판정 크기때문에 유닛의 사이즈 필요
	virtual bool init(const Size& size, int bitmask, int tag);
	PhysicsBody* getBody();
};

#endif