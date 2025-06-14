#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCamera.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
	, m_eType(COLLIDER_TYPE::RECTANGLE)
	, m_tOriginPoint{}
	, m_tPoint{}
{
}

CCollider::CCollider(Vec2 _pDot1, Vec2 _pDot2, Vec2 _pDot3)
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
	, m_eType(COLLIDER_TYPE::TRIANGLE)
{
	POINT point[3] = { {(LONG)_pDot1.x, (LONG)_pDot1.y} , {(LONG)_pDot2.x, (LONG)_pDot2.y} , {(LONG)_pDot3.x, (LONG)_pDot3.y} };

	for (int i = 0; i < 3; ++i)
	{
		m_tOriginPoint.pDot[i] = point[i];
	}

	InitTrianglePoint();
}

// 오브젝트가 복사 되었을 때 기본 복사생성자를 호출하게 되면,
// 같은 ID 값을 지닌 Collider 가 생길 수 있어서 직접 만들어야함
CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(_origin.m_bActive)
	, m_eType(_origin.m_eType)
	, m_tOriginPoint(_origin.m_tOriginPoint)
	, m_tPoint(_origin.m_tPoint)
{
}

CCollider::~CCollider()
{
}

void CCollider::InitTrianglePoint()
{
	if (m_eType == COLLIDER_TYPE::RECTANGLE)
		return;

	int iCount = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (m_tOriginPoint.pDot[i].x == 0 && m_tOriginPoint.pDot[i].y == 0)
			iCount++;

		if(iCount == 3)
			assert(nullptr);
	}

	LONG lMinX = GetMinTriPos(m_tOriginPoint.pDot).x;
	LONG lMinY = GetMinTriPos(m_tOriginPoint.pDot).y;

	for (int i = 0; i < 3; ++i)
	{
		m_tOriginPoint.pDot[i].x -= lMinX;
		m_tOriginPoint.pDot[i].y -= lMinY;
	}
}

void CCollider::SetTriPos(Vec2 _vPos)
{
	for (int i = 0; i < 3; ++i)
	{
		m_tPoint.pDot[i].x = (LONG)_vPos.x + m_tOriginPoint.pDot[i].x;
		m_tPoint.pDot[i].y = (LONG)_vPos.y + m_tOriginPoint.pDot[i].y;
	}
}

Vec2 CCollider::GetTriScale()
{
	POINT pMinPos = GetMinTriPos(m_tOriginPoint.pDot);
	POINT pMaxPos = GetMaxTriPos(m_tOriginPoint.pDot);

	Vec2 vScale = Vec2((float)pMaxPos.x - (float)pMinPos.x, (float)pMaxPos.y - (float)pMinPos.y);

	return vScale;
}

POINT CCollider::GetMinTriPos(POINT _pPoint[3])
{
	POINT vMinPos = _pPoint[0];

	for (int i = 0; i < 3; ++i)
	{
		if (vMinPos.x > _pPoint[i].x)
			vMinPos.x = _pPoint[i].x;

		if (vMinPos.y > _pPoint[i].y)
			vMinPos.y = _pPoint[i].y;
	}

	return vMinPos;
}

POINT CCollider::GetMaxTriPos(POINT _pPoint[3])
{
	POINT vMaxPos = _pPoint[0];

	for (int i = 0; i < 3; ++i)
	{
		if (vMaxPos.x < _pPoint[i].x)
			vMaxPos.x = _pPoint[i].x;

		if (vMaxPos.y < _pPoint[i].y)
			vMaxPos.y = _pPoint[i].y;
	}

	return vMaxPos;
}

void CCollider::finalupdate()
{
	// Object 의 위치를 따라감
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	if (m_eType == COLLIDER_TYPE::TRIANGLE)
		SetTriPos(m_vFinalPos);

	// m_bActive = UpdateCollisionActive();

	assert(0 <= m_iCol);
}

void CCollider::render(HDC _dc)
{
	if (!m_bActive)
		return;

	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
		ePen = PEN_TYPE::RED;

	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	if (m_eType == COLLIDER_TYPE::RECTANGLE)
	{
		Rectangle(_dc
			, (int)(vRenderPos.x - m_vScale.x / 2.f)
			, (int)(vRenderPos.y - m_vScale.y / 2.f)
			, (int)(vRenderPos.x + m_vScale.x / 2.f)
			, (int)(vRenderPos.y + m_vScale.y / 2.f));
	}
	else if (m_eType == COLLIDER_TYPE::TRIANGLE)
	{
		POINT points[3] = {};
		Vec2 vPoints[3] = {};

		for (int i = 0; i < 3; ++i)
		{
			vPoints[i] = m_tPoint.pDot[i];
			vPoints[i] = CCamera::GetInst()->GetRenderPos(vPoints[i]);

			points[i].x = (LONG)vPoints[i].x;
			points[i].y = (LONG)vPoints[i].y;
		}

		Polygon(_dc, points, 3);
	}
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCol;
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCol;
	m_pOwner->OnCollisionExit(_pOther);
}

bool CCollider::UpdateCollisionActive()
{
	if (m_pOwner->GetName() == L"Player")
		return true;

	// obj pos - obj scale < Player < obj pos + obj scale

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vLookAt = CCamera::GetInst()->GetLookAt();

	float fCamRX = vLookAt.x + vResolution.x / 2.f; // 우측 x좌표
	float fCamLX = vLookAt.x - vResolution.x / 2.f; // 좌측 x좌표

	float fObjRX = m_pOwner->GetPos().x + m_vScale.x;
	float fObjLX = m_pOwner->GetPos().x - m_vScale.x;

	if (fCamLX >= fObjLX && fCamRX <= fObjRX)
		return true;

	if (fCamLX <= fObjLX && fCamRX <= fObjRX && fCamRX >= fObjLX)
		return true;

	if (fCamLX >= fObjLX && fCamRX >= fObjRX && fCamLX <= fObjRX)
		return true;

	if (fCamLX <= fObjLX && fCamRX >= fObjRX)
		return true;

	return false;

	// 조건 체크 다시 해야함
}