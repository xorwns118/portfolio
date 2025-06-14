#include "global.h"
#include "CRigidBody.h"

#include "CTimeMgr.h"
#include "CObject.h"

CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(200.f)
	, m_vMaxVelocity(Vec2(200.f, 1000.f))
	, m_bBlockedByCollision(false)
{

}

// 복사생성자 추가

CRigidBody::~CRigidBody()
{

}

void CRigidBody::finalupdate()
{
	// 힘의 크기
	float fForce = m_vForce.Length();

	if (fForce != 0.f)
	{
		// 힘의 방향
		m_vForce.Normalize();

		// 가속도의 크기
		float m_fAccel = fForce / m_fMass;

		// 가속도
		m_vAccel = m_vForce * m_fAccel;

		// 속도
		m_vVelocity += m_vAccel * fDT;
	}

	// 추가 가속도
	m_vAccel += m_vAccelA;

	// 속도
	m_vVelocity += m_vAccel * fDT;

	// 마찰력에 의한 반대방향으로의 가속도
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFricDir = -m_vVelocity;
		vFricDir.Normalize();

		// x, y 둘 다 마찰력의 영향을 받음
		Vec2 vFriction = vFricDir * m_fFricCoeff * fDT;
		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// 마찰 가속도가 본래 속도보다 더 큰 경우
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// 속도 제한 검사
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		float fV = abs(m_vVelocity.x);
		float fMaxV = abs(m_vMaxVelocity.x);
		m_vVelocity.x = (m_vVelocity.x / fV) * fMaxV;
	}

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		float fV = abs(m_vVelocity.y);
		float fMaxV = abs(m_vMaxVelocity.y);
		m_vVelocity.y = (m_vVelocity.y / fV) * fMaxV;
	}
	
	// 속도에 따른 이동
	Move();

	// 힘 초기화
	m_vForce = Vec2(0.f, 0.f);

	// 가속도 초기화
	m_vAccel = Vec2(0.f, 0.f);
	m_vAccelA = Vec2(0.f, 0.f);
}

void CRigidBody::Move()
{
	// 이동 속력
	float fSpeed = m_vVelocity.Length();
	
	if (fSpeed != 0)
	{
		// 이동 방향
		Vec2 vDir = m_vVelocity;
		vDir.Length();

		Vec2 vPos = m_pOwner->GetPos();

		vPos += m_vVelocity * fDT;

		m_pOwner->SetPos(vPos);
	}
}