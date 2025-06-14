#pragma once
#include "CObject.h"

class CAnimation;

enum class PLAYER_STATE
{
	IDLE,
	WALK,
	RUN,
	ATTACK,
	JUMP,
	FLY,
	TUMBLING_FALL,
	FALL,
	DEEP_FALL,
	LANDING,
	INHALE_START,
	INHALE_DEEP,
	INHALE_FINISH,
	DEAD,
};

enum class PLAYER_ATTACK_STATE
{
	NORMAL, // 뱉는 상황
	NORMAL_ATT_2,
	NORMAL_ATT_3,

	SKILL_ATT_1,
	// ....
};

class CTexture;

class CPlayer : public CObject
{
private:
	vector<CObject*>	m_vecIsGround;

	Vec2				m_vCurPos;
	Vec2				m_vPrevPos;

	PLAYER_STATE		m_eCurState;
	PLAYER_STATE		m_ePrevState;
	int					m_iDir;				// 양수가 오른쪽 기준으로
	int					m_iPrevDir;

	float				m_fFallCount;		// 떨어지는 시간

	bool				m_bInhaleFinish;	// 빨아들이는 상태가 끝났는지
	float				m_fInhaleCount;		// 빨아들이는 중인지

	bool				m_bCanMove;			// 중간에 끊기면 안되는 애니메이션인지

public:
	virtual void update();
	virtual void render(HDC _dc);

public:
	Vec2 GetPrevPos() { return m_vPrevPos; }
	Vec2 GetCurPos() { return m_vCurPos; }
	int GetDir() { return m_iDir; }

	PLAYER_STATE GetCurState() { return m_eCurState; }

	vector<CObject*>& GetGroundGroup() { return m_vecIsGround; }

	void SetCanMove(bool _b) { m_bCanMove = _b; }
	bool GetCanMove(bool _b) { return m_bCanMove; }

public:
	virtual void Attack() {};
	virtual void Fly() {};
	virtual void Inhale();
	virtual void Exhale() {};

	void Fall();

private:
	void CreateMissile();
	void update_state();
	void update_move();
	void update_animation();
	void update_gravity();
	void MapRangeCheck();

	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

	CLONE(CPlayer);

public:
	CPlayer();
	~CPlayer();
};

