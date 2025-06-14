#include "global.h"
#include "CForeground.h"

#include "CTexture.h"
#include "CCore.h"

CForeground::CForeground(Vec2 _vLT, Vec2 _vScale, Vec2 _vPos)
	: m_pCurFgTex(nullptr)
	, m_vLT(_vLT)
	, m_vScale(_vScale)
	, m_vPos(_vPos)
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
		Vec2 vLookAt = CCamera::GetInst()->GetLookAt();

		float fCamRX = vLookAt.x + vResolution.x / 2.f; // 우측 좌표x
		float fCamLX = vLookAt.x - vResolution.x / 2.f; // 좌측 좌표x

		float fMapRX = m_vPos.x + m_vScale.x;
		float fMapLX = m_vPos.x - m_vScale.x;

		// 카메라의 제일 좌측 x 좌표가 맵의 우측 끝 x 좌표보다 크거나,
		// 카메라의 제일 우측 x 좌표가 맵의 좌측 끝 x 좌표보다 작으면 랜더링 x
		if (fCamLX > fMapRX || fCamRX < fMapLX)
			return;

		Vec2 vCamPos = CCamera::GetInst()->GetRenderPos(m_vPos);

		TransparentBlt(_dc
			, (int)(vCamPos.x - m_vScale.x), (int)(vCamPos.y - m_vScale.y)
			, (int)m_vScale.x * 2, (int)m_vScale.y * 2
			, m_pCurFgTex->GetDC()
			, (int)m_vLT.x, (int)m_vLT.y
			, (int)m_vScale.x, (int)m_vScale.y
			, RGB(255, 0, 255));
	}
}
// 각 맵 사이즈 x좌표 300 * 2 씩 랜더링
// |맵 좌표 - 맵 사이즈 / 2| 