#include "pch.h"
#include "CMissile.h"

#include "CTimeMgr.h"
#include "CCollider.h"

CMissile::CMissile()
	: m_fTheta((45.f / 180.f) * PI)
	, m_vDir(Vec2(0.f, 1.f))
{
	m_vDir.Normalize();
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
}

CMissile::~CMissile()
{

}

void CMissile::update()
{
	Vec2 vPos = GetPos();

	// vPos.x += 600.f * cosf(m_fTheta) * fDT;
	// vPos.y -= 600.f * sinf(m_fTheta) * fDT; ������ ��ǥ���� y���� +�� �Ʒ��� ����Ű�� ������ - ����

	vPos.x += 600.f * m_vDir.x * fDT;
	vPos.y += 600.f * m_vDir.y * fDT;

	SetPos(vPos);
}

void CMissile::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Ellipse(_dc, (int)(vPos.x - vScale.x / 2.f), (int)(vPos.y - vScale.y / 2.f)
		, (int)(vPos.x + vScale.x / 2.f), (int)(vPos.y + vScale.y / 2.f));

	component_render(_dc);
}

void CMissile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}
}

// 1����
// �������� ���̰� r�� ������ ���̰� r�� ȣ�� ���� �߽ɰ��� ũ�⸦ �� ��� ���� �� �� �� ũ��
// r : 2��r = �ᨬ : 360��
// �� �ᨬ = 180�� / ��

// �ﰢ�Լ� �� = radian
// cos�� : �غ� / ���� (x ��ǥ)
// sin�� : ���� / ���� (y ��ǥ)
// tan�� : ���� / �غ� => ������ �����Ŀ��� ���⸦ ����

// ����
// ������ ���̸� 1�� ������� �� �غ��� ���̸� ���Ͽ� x, y ��ǥ ����