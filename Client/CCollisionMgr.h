#pragma once

class CCollider;

union COLLIDER_ID
{
	struct {
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr);
private:
	map<ULONGLONG, bool> m_mapColInfo;				// 충돌체 간의 이전 프레임 충돌 정보
	UINT	m_arrCheck[(UINT)GROUP_TYPE::END];		// 그룹건의 충돌 체크 매트릭스

public:
	void update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END); } // memset() 지정한 주소를 지정한 값으로 세팅 (byte 단위로)

private:
	void CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool CollisionNotInRange(CCollider* _pLeftCol, CCollider* _pRightCol);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
	bool RectPointInRectangle(CCollider* _pLeftCol, CCollider* _pRightCol);
	bool TriangleRectCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
	bool PointInRect(const Vec2& _vPt, const Vec2& _vLT, const Vec2& vScale);
	bool PointInTriangle(const Vec2& _vPt, const Vec2& vDot1, const Vec2& vDot2, const Vec2& vDot3);
	float sign(const Vec2& _vPt, const Vec2& vDot1, const Vec2& vDot2);
	bool LinesIntersect(const Vec2& a1, const Vec2& a2, const Vec2& b1, const Vec2& b2);
	bool ccw(const Vec2& p1, const Vec2& p2, const Vec2& p3);
	bool TrianglesIntersect(CCollider* _pLeftCol, CCollider* _pRightCol);
	std::pair<Vec2, Vec2> edge(const Vec2* t, int i);
};

