#include "global.h"
#include "CGround.h"

#include "CCollider.h"
#include "CGravity.h"
#include "CRigidBody.h"
#include "CPlayer.h"

#include "CCore.h"

CGround::CGround()
	: m_bVertical(false)
{
	CreateCollider();
}

CGround::CGround(const Vec2 _vStartPos, const Vec2 _vMiddlePos, const Vec2 _vEndPos)
	: m_bVertical(false)
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
	ColliderCheck(_pOther);
}

void CGround::OnCollision(CCollider* _pOther)
{
	ColliderCheck(_pOther);
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
	Vec2 vPlayerPos = _pOther->GetFinalPos();
	Vec2 vPlayerScale = _pOther->GetScale();

	Vec2 vLeftPoint = GetCollider()->GetMinTriPos(GetCollider()->GetTriPoint().pDot);
	Vec2 vRightPoint = GetCollider()->GetMaxTriPos(GetCollider()->GetTriPoint().pDot);

	if (vPlayerPos.x < vLeftPoint.x || vPlayerPos.x > vRightPoint.x)
		return;

	float fGroundY = GetYAtX(vLeftPoint, vRightPoint, vPlayerPos.x);

	float fNewY = fGroundY - vPlayerScale.y / 2.f - _pOther->GetOffsetPos().y - 3.f;
	_pOther->GetObj()->SetPos(Vec2(vPlayerPos.x, fNewY));

	_pOther->GetObj()->GetGravity()->SetGround(true);
}

float CGround::GetYAtX(Vec2 _vLP, Vec2 _vRP, float _fX)
{
	float fDx = _vRP.x - _vLP.x;
	if (fDx == 0.f) return _vLP.y;

	float t = (_fX - _vLP.x) / fDx;
	return _vLP.y + t * (_vRP.y - _vLP.y);
}

void CGround::ColliderCheck(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player")
	{
		// 전통 C 스타일의 강제캐스팅 방법(컴파일 시 검사, 가장 단순한 만큼 가장 안전하지 않음)
		// CPlayer* pPlayer = (CPlayer*)pOtherObj;

		// 정확한 타입인지 확신할 수 없는 경우 (런타임시 검사하기 때문에 런타임 비용 발생, 다만 실패 시 nullptr 을 반환하기 때문에 안전함)
		// CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetObj());
		
		// 정확한 타입을 알고 런타임 성능이 중요시 되는 경우 (컴파일 시 검사, 타입이 맞다고 착각할 수 있음)
		CPlayer* pPlayer = static_cast<CPlayer*>(_pOther->GetObj()); // 어떤 타입인지 확실히 알고 사용하는 것이기 때문에 가장 적합하다고 판단

		if (GetCollider()->GetColliderType() == COLLIDER_TYPE::RECTANGLE)
		{
			Vec2 vObjPos = _pOther->GetFinalPos();
			Vec2 vObjScale = _pOther->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();
			
			float fPlayerBottom = vObjPos.y + vObjScale.y / 2.f;
			float fGroundTop = vPos.y - vScale.y / 2.f;

			if (fPlayerBottom <= fGroundTop + 5.f)
				m_bVertical = true;
			else
				m_bVertical = false;

			if (m_bVertical)
			{
				float fLen = abs(vObjPos.y - vPos.y);
				float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

				vObjPos = pOtherObj->GetPos();
				vObjPos.y -= fValue;

				pOtherObj->SetPos(vObjPos);
				
				vector<CObject*>::iterator iter = find(pPlayer->GetGroundGroup().begin(), pPlayer->GetGroundGroup().end(), this);
				if (iter == pPlayer->GetGroundGroup().end())
					pPlayer->GetGroundGroup().push_back(this);

				if(pOtherObj->GetRigidBody()->GetVelocity().y >= 0)
					pOtherObj->GetGravity()->SetGround(true);
			}
			else
			{
				float fLen = abs(vObjPos.x - vPos.x);
				float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

				if (pPlayer->GetPrevPos().x - pPlayer->GetPos().x > 0) // 왼쪽방향으로 진입
				{
					vObjPos = pOtherObj->GetPos();
					vObjPos.x += fValue;
					pOtherObj->SetPos(vObjPos);
				}
				else if(pPlayer->GetPrevPos().x - pPlayer->GetPos().x < 0) // 오른쪽 방향으로 진입
				{
					vObjPos = pOtherObj->GetPos();
					vObjPos.x -= fValue;
					pOtherObj->SetPos(vObjPos);
				}
			}
		}
		else if (GetCollider()->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
		{
			TriGroundCollision(_pOther);
			vector<CObject*>::iterator iter = find(pPlayer->GetGroundGroup().begin(), pPlayer->GetGroundGroup().end(), this);
			if (iter == pPlayer->GetGroundGroup().end())
				pPlayer->GetGroundGroup().push_back(this);
			pOtherObj->GetGravity()->SetGround(true);
		}
	}
}
