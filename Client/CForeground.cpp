#include "global.h"
#include "CForeground.h"

#include "CTexture.h"
#include "CCore.h"

CForeground::CForeground(Vec2 _vLT, Vec2 _vSize)
	: m_pCurFgTex(nullptr)
	, m_vLT(_vLT)
	, m_vSize(_vSize)
{
}

CForeground::~CForeground()
{
}

void CForeground::update()
{
}

void CForeground::render(HDC _dc)
{
	if (m_pCurFgTex != nullptr)
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		Vec2 vCamPos = CCamera::GetInst()->GetRenderPos({ 0, 0 });

		TransparentBlt(_dc
			, (int)vCamPos.x, (int)vCamPos.y
			, (int)m_vSize.x * 2, (int)m_vSize.y * 2
			, m_pCurFgTex->GetDC()
			, (int)m_vLT.x, (int)m_vLT.y
			, (int)m_vSize.x, (int)m_vSize.y
			, RGB(255, 0, 255));
	}
}