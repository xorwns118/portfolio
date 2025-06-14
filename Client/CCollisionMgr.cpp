#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

#include "CGround.h"

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::update()
{
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	// 반환 타입이 래퍼런스이지만 & 안쓰면 지역변수에 복사가 되는 상황이기 때문에 & 꼭 써줘야함
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);
	map<ULONGLONG, bool>::iterator iter;

	bool bIsGround = false;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 충돌체를 보유하지 않거나 충돌체가 비활성화 된 경우
		if (vecLeft[i]->GetCollider() == nullptr || !vecLeft[i]->GetCollider()->GetActive())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나, 자기 자신과의 충돌일 경우, 충돌체가 비활성화 된 경우
			if (vecRight[j]->GetCollider() == nullptr || vecLeft[i] == vecRight[j] || !vecLeft[j]->GetCollider()->GetActive())
				continue;

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// 등록되어있는 충돌체간의 거리가 범위내에 없을 때 연산 x
			if (CollisionNotInRange(pLeftCol, pRightCol))
				continue;

			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// 충돌 정보가 미 등록 상태인 경우 등록(충돌하지 않았다. 로)
			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌중이다.
				if (iter->second)
				{
					// 이전에도 충돌하고 있었다.
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 둘 중 하나가 삭제 예정이라면 충돌 해제 시켜준다.
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
				else
				{
					// 이전에는 충돌하지 않았다.
					// 둘 중 하나가 삭제 예정이라면 충돌하지 않은것으로 취급
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}
			else
			{
				// 현재 충돌하고 있지 않다.
				if (iter->second)
				{
					// 이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::CollisionNotInRange(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	return (_pRightCol->GetFinalPos().x - _pRightCol->GetScale().x > _pLeftCol->GetFinalPos().x + _pLeftCol->GetScale().x
		|| _pRightCol->GetFinalPos().x + _pRightCol->GetScale().x < _pLeftCol->GetFinalPos().x - _pLeftCol->GetScale().x
		|| _pRightCol->GetFinalPos().y - _pRightCol->GetScale().y > _pLeftCol->GetFinalPos().y + _pLeftCol->GetScale().y
		|| _pRightCol->GetFinalPos().y + _pRightCol->GetScale().y < _pLeftCol->GetFinalPos().y - _pLeftCol->GetScale().y);
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	if (_pLeftCol->GetColliderType() == COLLIDER_TYPE::RECTANGLE && _pRightCol->GetColliderType() == COLLIDER_TYPE::RECTANGLE)
	{
		if(_pLeftCol->GetObj()->GetName() == L"Player" || _pRightCol->GetObj()->GetName() == L"Player")
			return RectPointInRectangle(_pLeftCol, _pRightCol);
	}
	else if (_pLeftCol->GetColliderType() == COLLIDER_TYPE::RECTANGLE && _pRightCol->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
	{
		if (_pLeftCol->GetObj()->GetName() == L"Player" || _pRightCol->GetObj()->GetName() == L"Player")
			return TriangleRectCollision(_pLeftCol, _pRightCol);
	}
	else if (_pLeftCol->GetColliderType() == COLLIDER_TYPE::TRIANGLE && _pRightCol->GetColliderType() == COLLIDER_TYPE::RECTANGLE)
	{
		if (_pLeftCol->GetObj()->GetName() == L"Player" || _pRightCol->GetObj()->GetName() == L"Player")
			return TriangleRectCollision(_pRightCol, _pLeftCol);
	}
	/*else if (_pLeftCol->GetColliderType() == COLLIDER_TYPE::TRIANGLE && _pRightCol->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
	{
		return TrianglesIntersect(_pLeftCol, _pRightCol);
	}*/

	return false;
}

// 사각형끼리의 충돌 체크
bool CCollisionMgr::RectPointInRectangle(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();


	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	return false;
}

// 삼각형과 사각형간의 충돌 체크
bool CCollisionMgr::TriangleRectCollision(CCollider* _pRectCol, CCollider* _pTriCol)
{
	Vec2 vRectPos = _pRectCol->GetFinalPos();
	Vec2 vRectScale = _pRectCol->GetScale();
	Vec2 vRectLT = vRectPos - (vRectScale / 2);

	// 사각형의 네 꼭지점
	Vec2 vRectPts[4] = {
		vRectLT,                                    // 좌상단
		vRectLT + Vec2(vRectScale.x, 0.f),         // 우상단
		vRectLT + vRectScale,                      // 우하단
		vRectLT + Vec2(0.f, vRectScale.y)          // 좌하단
	};

	// 삼각형의 세 꼭지점
	Vec2 vTriPts[3] = {
		_pTriCol->GetTriPoint().pDot[0],
		_pTriCol->GetTriPoint().pDot[1],
		_pTriCol->GetTriPoint().pDot[2]
	};

	// 1. 사각형의 변을 기준으로 분리축 검사
	Vec2 rectAxes[2] = {
		Vec2(1.f, 0.f),    // x축
		Vec2(0.f, 1.f)     // y축
	};

	for (int i = 0; i < 2; ++i)
	{
		float rectMin = FLT_MAX;
		float rectMax = -FLT_MAX;
		float triMin = FLT_MAX;
		float triMax = -FLT_MAX;

		// 사각형의 투영 범위 계산
		for (int j = 0; j < 4; ++j)
		{
			float proj = vRectPts[j].x * rectAxes[i].x + vRectPts[j].y * rectAxes[i].y;
			rectMin = min(rectMin, proj);
			rectMax = max(rectMax, proj);
		}

		// 삼각형의 투영 범위 계산
		for (int j = 0; j < 3; ++j)
		{
			float proj = vTriPts[j].x * rectAxes[i].x + vTriPts[j].y * rectAxes[i].y;
			triMin = min(triMin, proj);
			triMax = max(triMax, proj);
		}

		// 분리축이 존재하는 경우
		if (rectMax < triMin || triMax < rectMin)
			return false;
	}

	// 2. 삼각형의 변을 기준으로 분리축 검사
	for (int i = 0; i < 3; ++i)
	{
		Vec2 edge = vTriPts[(i + 1) % 3] - vTriPts[i];
		float length = sqrtf(edge.x * edge.x + edge.y * edge.y);
		Vec2 normal = Vec2(-edge.y / length, edge.x / length);

		float rectMin = FLT_MAX;
		float rectMax = -FLT_MAX;
		float triMin = FLT_MAX;
		float triMax = -FLT_MAX;

		// 사각형의 투영 범위 계산
		for (int j = 0; j < 4; ++j)
		{
			float proj = vRectPts[j].x * normal.x + vRectPts[j].y * normal.y;
			rectMin = min(rectMin, proj);
			rectMax = max(rectMax, proj);
		}

		// 삼각형의 투영 범위 계산
		for (int j = 0; j < 3; ++j)
		{
			float proj = vTriPts[j].x * normal.x + vTriPts[j].y * normal.y;
			triMin = min(triMin, proj);
			triMax = max(triMax, proj);
		}

		// 분리축이 존재하는 경우
		if (rectMax < triMin || triMax < rectMin)
			return false;
	}

	// 모든 분리축 검사를 통과했다면 충돌
	return true;
}

// 삼각형끼리의 충돌 체크
bool CCollisionMgr::TrianglesIntersect(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftTriPts[3] = {
		_pLeftCol->GetTriPoint().pDot[0],
		_pLeftCol->GetTriPoint().pDot[1],
		_pLeftCol->GetTriPoint().pDot[2]
	};

	Vec2 vRightTriPts[3] = {
		_pRightCol->GetTriPoint().pDot[0],
		_pRightCol->GetTriPoint().pDot[1],
		_pRightCol->GetTriPoint().pDot[2]
	};

	// 1. 삼각형의 꼭지점이 다른 삼각형 안에 있는지 검사
	for (int i = 0; i < 3; ++i)
	{
		if (PointInTriangle(vLeftTriPts[i], vRightTriPts[0], vRightTriPts[1], vRightTriPts[2]))
			return true;
		if (PointInTriangle(vRightTriPts[i], vLeftTriPts[0], vLeftTriPts[1], vLeftTriPts[2]))
			return true;
	}

	// 2. 변 교차 여부 검사
	for (int i = 0; i < 3; ++i)
	{
		Vec2 leftA = vLeftTriPts[i];
		Vec2 leftB = vLeftTriPts[(i + 1) % 3];

		for (int j = 0; j < 3; ++j)
		{
			Vec2 rightA = vRightTriPts[j];
			Vec2 rightB = vRightTriPts[(j + 1) % 3];

			if (LinesIntersect(leftA, leftB, rightA, rightB))
				return true;
		}
	}

	return false;
}

pair<Vec2, Vec2> CCollisionMgr::edge(const Vec2* t, int i)
{
	return std::make_pair(t[i], t[(i + 1) % 3]);
};

// 사각형 안에 점이 있는지
bool CCollisionMgr::PointInRect(const Vec2& _vPt, const Vec2& _vLT, const Vec2& vScale)
{
	return _vPt.x >= _vLT.x && _vPt.x <= _vLT.x + vScale.x &&
		_vPt.y >= _vLT.y && _vPt.y <= _vLT.y + vScale.y;

	return false;
}

// 삼각형 안에 점이 있는지
bool CCollisionMgr::PointInTriangle(const Vec2& _vPt, const Vec2& vDot1, const Vec2& vDot2, const Vec2& vDot3)
{
	float d1 = sign(_vPt, vDot1, vDot2);
	float d2 = sign(_vPt, vDot2, vDot3);
	float d3 = sign(_vPt, vDot3, vDot1);

	bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	// 점이 삼각형의 변 위에 있는 경우도 포함
	if (d1 == 0 || d2 == 0 || d3 == 0)
		return true;

	return !(hasNeg && hasPos);
}

float CCollisionMgr::sign(const Vec2& _vPt, const Vec2& vDot1, const Vec2& vDot2)
{
	return (_vPt.x - vDot2.x) * (vDot1.y - vDot2.y) - (vDot1.x - vDot2.x) * (_vPt.y - vDot2.y);
}

bool CCollisionMgr::LinesIntersect(const Vec2& a1, const Vec2& a2, const Vec2& b1, const Vec2& b2)
{
	// 선분이 겹치는 경우 처리
	if ((a1.x == b1.x && a1.y == b1.y) || (a1.x == b2.x && a1.y == b2.y) 
		|| (a2.x == b1.x && a2.y == b1.y) || (a2.x == b2.x && a2.y == b2.y))
		return true;

	// 선분이 평행한 경우
	float denominator = ((b2.y - b1.y) * (a2.x - a1.x)) - ((b2.x - b1.x) * (a2.y - a1.y));
	if (denominator == 0)
		return false;

	float ua = (((b2.x - b1.x) * (a1.y - b1.y)) - ((b2.y - b1.y) * (a1.x - b1.x))) / denominator;
	float ub = (((a2.x - a1.x) * (a1.y - b1.y)) - ((a2.y - a1.y) * (a1.x - b1.x))) / denominator;

	// 교차점이 두 선분 위에 있는지 확인
	return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}

bool CCollisionMgr::ccw(const Vec2& p1, const Vec2& p2, const Vec2& p3)
{
	float val = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	
	// 부동소수점 오차 처리
	const float EPSILON = 0.000001f;
	if (abs(val) < EPSILON)
		return false;
		
	return val > 0;
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로,
	// 큰 값을 열(비트) 로 사용

	UINT iRow = (UINT)_eLeft;	//행
	UINT iCol = (UINT)_eRight;	//열

	if (iRow > iCol)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol); // 지정된 행에서 칼럼값 만큼 이동 후 1을 뺌
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol); // 지정된 행에서 칼럼값 만큼 이동 후 1을 넣음
	}
}
