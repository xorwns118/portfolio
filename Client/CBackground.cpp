#include "global.h"
#include "CBackground.h"

#include "CKeyMgr.h"

#include "CScene.h"

#include "CCore.h"
#include "CTexture.h"

CBackground::CBackground(Vec2 _vLT, Vec2 _vScale)
	: m_pCurBgTex(nullptr)
	, m_vLT(_vLT)
	, m_vScale(_vScale)
{
}

CBackground::~CBackground()
{
}

void CBackground::update()
{
	
}

// 패럴랙스 스크롤링 (Parallax Scrolling) : 배경화면은 느리게, 캐릭터와 맵은 빠르게 이동
void CBackground::render(HDC _dc)
{
	if (m_pCurBgTex != nullptr)
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		Vec2 vRenderPos = CCamera::GetInst()->GetRenderDiff({0.f, 0.f}, 0.08f); // 수치 계산 로직 구현 필요

		StretchBlt(_dc
			, (int)vRenderPos.x, (int)vRenderPos.y
			, (int)(vResolution.x * 1.5f), (int)vResolution.y
			, m_pCurBgTex->GetDC()
			, (int)m_vLT.x, (int)m_vLT.y
			, (int)m_vScale.x, (int)m_vScale.y
			, SRCCOPY);
	}
}
