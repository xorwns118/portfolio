#pragma once
#include "CObject.h"

class CGround : public CObject
{
private:
	bool	m_bVertical; // 수직 방향으로 접근시

public:
	virtual void start();
	virtual void render(HDC _dc);
	virtual void update();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

private:
	void TriGroundCollision(CCollider* _pOther);
	float GetYAtX(Vec2 _vLP, Vec2 _vRP, float _fX);
	void ColliderCheck(CCollider* _pOther);

	CLONE(CGround);

public:
	CGround();
	CGround(const Vec2 _vStartPos, const Vec2 _vMiddlePos, const Vec2 _vEndPos);
	~CGround();
};

