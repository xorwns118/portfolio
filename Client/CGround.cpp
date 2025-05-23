#include "global.h"
#include "CGround.h"

#include "CCollider.h"
#include "CGravity.h"
#include "CRigidBody.h"
#include "CPlayer.h"

CGround::CGround()
{
	CreateCollider();
}

CGround::CGround(const Vec2 _vStartPos, const Vec2 _vMiddlePos, const Vec2 _vEndPos)
{
	CreateCollider(_vStartPos, _vMiddlePos, _vEndPos);
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetCollider()->SetScale(GetScale());
}

void CGround::render(HDC _dc)
{
	component_render(_dc);
}

void CGround::update()
{
}

void CGround::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		if (GetCollider()->GetColliderType() == COLLIDER_TYPE::RECTANGLE) 
		{
			Vec2 vObjPos = _pOther->GetFinalPos();
			Vec2 vObjScale = _pOther->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();

			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= fValue;

			pOtherObj->SetPos(vObjPos);
		}
		else if(GetCollider()->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
		{
			TriGroundCollision(_pOther);
		}
	}
}

void CGround::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		if (GetCollider()->GetColliderType() == COLLIDER_TYPE::RECTANGLE)
		{
			Vec2 vObjPos = _pOther->GetFinalPos();
			Vec2 vObjScale = _pOther->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();

			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= fValue;

			pOtherObj->SetPos(vObjPos);
		}
		else if (GetCollider()->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
		{
			TriGroundCollision(_pOther);
		}
	}
}

void CGround::OnCollisionExit(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);
	}
}

void CGround::TriGroundCollision(CCollider* _pOther)
{
	Vec2 vPlayerPos = _pOther->GetObj()->GetPos();
	Vec2 vPlayerScale = _pOther->GetObj()->GetScale();
	
	float yVel = _pOther->GetObj()->GetRigidBody()->GetVelocity().y;

	Vec2 vLeftPoint = GetCollider()->GetMinTriPos(GetCollider()->GetTriPoint().pDot);
	Vec2 vRightPoint = GetCollider()->GetMaxTriPos(GetCollider()->GetTriPoint().pDot);

	if (vPlayerPos.x < vLeftPoint.x || vPlayerPos.x > vRightPoint.x)
		return;

	float fGroundY = GetYAtX(vLeftPoint, vRightPoint, vPlayerPos.x);

	float fNewY = fGroundY - vPlayerScale.y / 2.f;
	_pOther->GetObj()->SetPos(Vec2(vPlayerPos.x, fNewY));
	// 내리막길을 내려갈때 끊기는 느낌이 듦
}

float CGround::GetYAtX(Vec2 _vLP, Vec2 _vRP, float _fX)
{
	float dx = _vRP.x - _vLP.x;
	if (dx == 0.f) return _vLP.y;

	float t = (_fX - _vLP.x) / dx;
	return _vLP.y + t * (_vRP.y - _vLP.y);
}