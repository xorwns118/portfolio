#pragma once

struct tTriangle
{
	POINT pDot[3];
};

class CObject;

class CCollider
{
private:
	static UINT g_iNextID;

	CObject*		m_pOwner;		// collider 를 소유하고 있는 오브젝트
	Vec2			m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치값
	Vec2			m_vFinalPos;	// finalupdate 에서 매 프레임마다 계산
	Vec2			m_vScale;		// 충돌체의 크기

	UINT			m_iID;			// 충돌체 간의 고유한 ID 값
	int				m_iCol;

	bool			m_bActive;		// 충돌체 활성화 여부

	COLLIDER_TYPE	m_eType;		// Collider 종류
	tTriangle		m_tOriginPoint;	// 0, 0 기준 삼각형 꼭짓점 포인트
	tTriangle		m_tPoint;		// 현재 위치 기반 포인트

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	void InitTrianglePoint();
	void SetTriPos(Vec2 _vPos);

	void SetActive(bool _bActive) { m_bActive = _bActive; }
	bool GetActive() { return m_bActive; }

	COLLIDER_TYPE GetColliderType() { return m_eType; }
	tTriangle GetTriPoint() { return m_tPoint; }
	Vec2 GetTriScale();

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }

	UINT GetID() { return m_iID; }

	POINT GetMinTriPos(POINT _pPoint[3]);
	POINT GetMaxTriPos(POINT _pPoint[3]);

public:
	void finalupdate();
	void render(HDC _dc);

public:
	// 충돌 시점 함수
	void OnCollision(CCollider* _pOther);		// 충동 중인 경우 호출되는 함수
	void OnCollisionEnter(CCollider* _pOther);	// 충돌 진입 시
	void OnCollisionExit(CCollider* _pOther);	// 충돌 해제 시

	CCollider& operator = (CCollider& _origin) = delete; // = 연산자를 삭제

private:
	bool UpdateCollisionActive();

public:
	CCollider();
	CCollider(Vec2 _pDot1, Vec2 _pDot2, Vec2 _pDot3);
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};

