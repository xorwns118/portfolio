#include "global.h"
#include "CMarker.h"

#include "CObject.h"
#include "CTexture.h"

#include "CCore.h"

#include "CCamera.h"
#include "CResMgr.h"

#include "SelectGDI.h"

#include "CCollider.h"

UINT CMarker::g_iNextID = 0;

CMarker::CMarker(Vec2 _vScale)
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
{
	wchar_t szPath[256] = {};
	swprintf_s(szPath, L"Marker%d", m_iID);
	m_pVeilTex = CResMgr::GetInst()->CreateTexture(szPath, (UINT)_vScale.x, (UINT)_vScale.y);
}

CMarker::~CMarker()
{
}

void CMarker::render(HDC _dc)
{
	Vec2 vObjScale = m_pOwner->GetScale();
	Vec2 vObjPos = m_pOwner->GetPos();

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vObjPos);

	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = 200;

	AlphaBlend(_dc
		, (int)(vRenderPos.x - vObjScale.x / 2.f)
		, (int)(vRenderPos.y - vObjScale.y / 2.f)
		, (int)vObjScale.x
		, (int)vObjScale.y
		, m_pVeilTex->GetDC()
		, 0, 0
		, (int)vObjScale.x
		, (int)vObjScale.y, bf);

	if (m_pOwner->GetCollider()->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
	{
		SelectGDI pen(_dc, PEN_TYPE::BLUE);

		POINT points[3] = {};
		Vec2 vPoints[3] = {};

		for (int i = 0; i < 3; ++i)
		{
			vPoints[i] = m_pOwner->GetCollider()->GetTriPoint().pDot[i];
			vPoints[i] = CCamera::GetInst()->GetRenderPos(vPoints[i]);

			points[i].x = (LONG)vPoints[i].x;
			points[i].y = (LONG)vPoints[i].y;
		}

		Polygon(_dc, points, 3);
	}
}