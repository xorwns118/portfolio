#pragma once
#include "CObject.h"
class CTiltedGround : public CObject
{
private:

public:
	virtual void start();
	virtual void render(HDC _dc);
	virtual void update();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

	CLONE(CTiltedGround);

public:
	CTiltedGround(Vec2 _vStartPos, Vec2 _vMiddlePos, Vec2 _vEndPos);
	~CTiltedGround();
};
