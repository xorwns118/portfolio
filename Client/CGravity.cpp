#include "global.h"
#include "CGravity.h"

#include "CObject.h"
#include "CRigidBody.h"

CGravity::CGravity()
	: m_pOwner(nullptr)
	, m_bGround(false)
{
}

CGravity::CGravity(const CGravity& _origin)
	: m_pOwner(nullptr)
	, m_bGround(false)
{
}

CGravity::~CGravity()
{
}

void CGravity::SetGround(bool _b)
{
	m_bGround = _b;
	if (m_bGround)
	{
		Vec2 vV = m_pOwner->GetRigidBody()->GetVelocity();
		m_pOwner->GetRigidBody()->SetVelocity(Vec2(vV.x, 0.f));
	}
}

void CGravity::finalupdate()
{
	if (!m_bGround)
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 1000.f)); // 중력 적용
	else
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 0.f));    // 지면이면 0
}
