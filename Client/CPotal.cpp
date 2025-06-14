#include "global.h"
#include "CPotal.h"

#include "CKeyMgr.h"
#include "CEventMgr.h"
#include "CResMgr.h"

#include "CTexture.h"
#include "CCollider.h"

CPotal::CPotal(UINT _iNextScene)
	: m_pTex(nullptr)
	, m_eNextScene((SCENE_TYPE)_iNextScene)
{
	CreateCollider();

	wchar_t szBuff[256] = {};

	if (m_eNextScene == SCENE_TYPE::END)
	{
		swprintf_s(szBuff, L"Finish_Potal");
		m_vLT = Vec2(14.f, 50.f);
		m_vScale = Vec2(22.f, 24.f);
	}
	else
	{
		swprintf_s(szBuff, L"Potal");
		m_vLT = Vec2(17.f, 6.f);
		m_vScale = Vec2(16.f, 24.f);
	}

	SetScale(m_vScale);
	GetCollider()->SetScale(m_vScale);

	m_pTex = CResMgr::GetInst()->LoadTexture(szBuff, L"texture\\Kirby\\Foreground\\Kirby_Potal.bmp");
	SetName(szBuff);
}

CPotal::~CPotal()
{
}

void CPotal::start()
{
	
}

void CPotal::update()
{
}

void CPotal::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());

	TransparentBlt(_dc
		, (int)(vRenderPos.x - m_vScale.x)
		, (int)(vRenderPos.y - m_vScale.y)
		, (int)(m_vScale.x * 2)
		, (int)(m_vScale.y * 2)
		, m_pTex->GetDC()
		, (int)m_vLT.x
		, (int)m_vLT.y
		, (int)m_vScale.x
		, (int)m_vScale.y
		, RGB(255, 0, 255));

	component_render(_dc);
}

void CPotal::OnCollision(CCollider* _pOther)
{
	if (KEY_TAP(KEY::W))
	{
		ChangeScene(m_eNextScene);
	}
}
