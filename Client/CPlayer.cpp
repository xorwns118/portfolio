#include "global.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CMissile.h"

#include "CResMgr.h"
#include "CTexture.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidBody.h"
#include "CGravity.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(-1)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(18.f, 18.f));

	CreateRigidBody();

	// Texture 로딩하기
	CTexture* pTexR = CResMgr::GetInst()->LoadTexture(L"KirbyTexR", L"texture\\Kirby\\Kirby\\Kirby_Kirby_Right.bmp");
	CTexture* pTexL = CResMgr::GetInst()->LoadTexture(L"KirbyTexL", L"texture\\Kirby\\Kirby\\Kirby_Kirby_Left.bmp");
	
	CreateAnimator();

	//vector<tAnimFrm> vecList;

	//// Right
	//GetAnimator()->CreateAnimDiffSize(vecList, Vec2(254.f, 11.f), Vec2(273.f, 28.f), 0.1f); // 1

	//// Animation 저장해보기
	//GetAnimator()->FindAnimation(L"KIRBY_IDLE_RIGHT")->Save(L"animation\\kirby_anim\\kirby_idle_right.anim");

	CreateGravity();

	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_idle_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_idle_right.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_walk_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_walk_right.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_jump_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_jump_right.anim");
}

CPlayer::~CPlayer()
{
	
}

void CPlayer::update()
{
	update_state();

	update_move();

	update_animation();

	if (KEY_TAP(KEY::ENTER))
	{
		SetPos(Vec2(200.f, 50.f));
	}

	GetAnimator()->update();

	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;

	m_vCurPos = GetPos();
}

void CPlayer::render(HDC _dc)
{
	component_render(_dc);
}

void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	// Missile Object
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(Vec2(0.f, -1.f));

	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}

void CPlayer::update_state()
{
	if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;
		if (m_eCurState != PLAYER_STATE::JUMP)
		{
			if(KEY_HOLD(KEY::LSHIFT))
				m_eCurState = PLAYER_STATE::RUN;
			else
				m_eCurState = PLAYER_STATE::WALK;
		}
	}

	if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;
		if (m_eCurState != PLAYER_STATE::JUMP)
		{
			if (KEY_HOLD(KEY::LSHIFT))
				m_eCurState = PLAYER_STATE::RUN;
			else
				m_eCurState = PLAYER_STATE::WALK;
		}
	}

	CRigidBody* pRigid = GetRigidBody();

	if (pRigid->GetSpeed() <= 0.2f && m_eCurState != PLAYER_STATE::JUMP)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	if (m_eCurState != PLAYER_STATE::JUMP && KEY_HOLD(KEY::SPACE))
	{
 		m_eCurState = PLAYER_STATE::JUMP;

		if (pRigid)
		{
			pRigid->AddVelocity(Vec2(0.f, -200.f));
		}
	}
}

void CPlayer::update_move()
{
	CRigidBody* pRigid = GetRigidBody();

	Vec2 vForce(0.f, 0.f);

	if (KEY_HOLD(KEY::A))
	{
		if (KEY_HOLD(KEY::LSHIFT))
		{
			pRigid->SetVelocity(Vec2(-200.f, pRigid->GetVelocity().y));
		}
		else
		{
			pRigid->SetVelocity(Vec2(-150.f, pRigid->GetVelocity().y));
		}
	}

	if (KEY_HOLD(KEY::D))
	{
		if (KEY_HOLD(KEY::LSHIFT))
		{
			pRigid->SetVelocity(Vec2(200.f, pRigid->GetVelocity().y));
		}
		else
		{
			pRigid->SetVelocity(Vec2(150.f, pRigid->GetVelocity().y));
		}
	}

	// 대각선 이동 시 힘 정규화
	if (vForce.x != 0.f && vForce.y != 0.f)
	{
		vForce.Normalize();
		vForce *= 200.f;
	}

	pRigid->AddForce(vForce);
}

void CPlayer::update_animation()
{
	if (m_ePrevState == m_eCurState && m_iDir == m_iPrevDir)
		return;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
		if (m_iDir == 1)
			GetAnimator()->Play(L"KIRBY_IDLE_RIGHT", true);
		else
			GetAnimator()->Play(L"KIRBY_IDLE_LEFT", true);
		break;
	case PLAYER_STATE::WALK:
		if (m_iDir == 1)
			GetAnimator()->Play(L"KIRBY_WALK_RIGHT", true);
		else
			GetAnimator()->Play(L"KIRBY_WALK_LEFT", true);
		break;
	case PLAYER_STATE::JUMP:
		if (m_iDir == 1)
			GetAnimator()->Play(L"KIRBY_JUMP_RIGHT", true);
		else
			GetAnimator()->Play(L"KIRBY_JUMP_LEFT", true);
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::DEAD:
		break;
	}
}

void CPlayer::update_gravity()
{
	GetRigidBody()->AddForce(Vec2(0.f, 400.f));
}

bool CPlayer::canMove()
{
	Vec2 vPos = GetPos();
	float vDiffScale = GetScale().x / 2.f;

	Vec2 vMapScale = CCamera::GetInst()->GetMapScale();

	if (vPos.x + vDiffScale >= vMapScale.x || vPos.x - vDiffScale <= 0.f 
		|| vPos.y + vDiffScale <= 0.f || vPos.y - vDiffScale >= vMapScale.y)
		return false;

	return true;
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOther = _pOther->GetObj();

	if (pOther->GetName() == L"Ground")
	{
		if(GetRigidBody()->GetSpeed() <= 0.2f)
			m_eCurState = PLAYER_STATE::IDLE;
		else
			m_eCurState = PLAYER_STATE::WALK;
	}
}
