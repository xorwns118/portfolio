#include "global.h"
#include "CBackground.h"

#include "CKeyMgr.h"

#include "CScene.h"

#include "CCore.h"
#include "CTexture.h"

CBackground::CBackground(Vec2 _vLT, Vec2 _vSize)
	: m_pCurBgTex(nullptr)
	, m_vLT(_vLT)
	, m_vSize(_vSize)
{
}

CBackground::~CBackground()
{
}

void CBackground::update()
{
	if (KEY_HOLD(KEY::A))
	{
		
	}

	if (KEY_HOLD(KEY::D))
	{

	}
}

void CBackground::render(HDC _dc)
{
	if (m_pCurBgTex != nullptr)
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();

		StretchBlt(_dc
			, 0, 0
			, (int)vResolution.x + 50.f, (int)vResolution.y
			, m_pCurBgTex->GetDC()
			, (int)m_vLT.x, (int)m_vLT.y
			, (int)m_vSize.x, (int)m_vSize.y
			, SRCCOPY);
	}
}
