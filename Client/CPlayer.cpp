#include "global.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CProjectile.h"

#include "CResMgr.h"
#include "CTexture.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidBody.h"
#include "CGravity.h"

#include "CGround.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(-1)
	, m_fFallCount(0.f)
	, m_bInhaleFinish(false)
	, m_fInhaleCount(0.f)
	, m_bCanMove(true)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));
	GetCollider()->SetScale(Vec2(20.f, 20.f));

	CreateRigidBody();

	CreateAnimator();

	CreateGravity();

	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_idle.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_walk.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_jump.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_deep_fall.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_fall.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_tumbling_fall.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_landing.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_run.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_inhale_start.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_inhale_deep.anim");
	GetAnimator()->LoadAnimation(L"animation\\kirby_anim\\kirby_inhale_finish.anim");
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
		SetPos(Vec2(GetPos().x, 10.f));
		GetGravity()->SetGround(false);
	}

	GetAnimator()->update();

	MapRangeCheck();

	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;

	m_vCurPos = GetPos();
	m_vPrevPos = m_vCurPos;
}

void CPlayer::render(HDC _dc)
{
	component_render(_dc);
}

void CPlayer::Inhale()
{
	m_bCanMove = false;
	m_fInhaleCount += fDT;
	CAnimation* pCurAnim = GetAnimator()->GetCurAnim();

	if (m_eCurState == PLAYER_STATE::IDLE)
		return;

	if (m_fInhaleCount < 1.5f)
	{
		m_eCurState = PLAYER_STATE::INHALE_START;
		if (pCurAnim->GetName() == L"KIRBY_INHALE_START")
		{
			if (pCurAnim->GetCurFrameIdx() == 5)
				pCurAnim->SetFrame(3);
		}
	}
	else if (1.5f <= m_fInhaleCount && m_fInhaleCount <= 3.f)
	{
		m_eCurState = PLAYER_STATE::INHALE_DEEP;
	}
	else if (3.f < m_fInhaleCount)
	{
		m_eCurState = PLAYER_STATE::INHALE_FINISH;

		if (GetAnimator()->GetCurAnim()->GetName() == L"KIRBY_INHALE_FINISH")
		{
			if (GetAnimator()->GetCurAnim()->IsFinish())
				m_bCanMove = true;
		}
	}
}

void CPlayer::Fall()
{
	m_fFallCount += fDT;
	// 땅에서 떨어질 땐 회전 없이 낙하 애니메이션, 점프 후 떨어질 땐 회전 후 낙하 애니메이션

	if (m_fFallCount >= 0.8f)
		m_eCurState = PLAYER_STATE::DEEP_FALL;
}

void CPlayer::CreateMissile()
{
	Vec2 vProjPos = GetPos();
	vProjPos.x += (GetScale().x / 2.f) * m_iDir;

	// Missile Object
	CProjectile* pProj = new CProjectile;
	pProj->SetName(L"Missile_Player");
	pProj->SetPos(vProjPos);
	pProj->SetScale(Vec2(25.f, 25.f));
	pProj->SetDir(Vec2(0.f, -1.f));

	CreateObject(pProj, GROUP_TYPE::PROJ_PLAYER);
}

// 캐릭터의 상태관리
void CPlayer::update_state()
{
	if (KEY_HOLD(KEY::X) && GetGravity()->IsGround() && !m_bInhaleFinish)
	{
		m_eCurState = PLAYER_STATE::INHALE_START;
		Inhale();
	}

	if (KEY_AWAY(KEY::X))
	{
		m_bInhaleFinish = false;
		m_fInhaleCount = 0.f;
		m_bCanMove = true;
	}

	if (!m_bCanMove)
		return;

	if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;

		GetAnimator()->SetAnimDir(ANIM_DIR::LEFT);
	}

	if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;

		GetAnimator()->SetAnimDir(ANIM_DIR::RIGHT);
	}

	CRigidBody* pRigid = GetRigidBody();

	if (GetGravity()->IsGround())
	{
		if (pRigid->GetSpeed() <= 0.2f && m_eCurState != PLAYER_STATE::JUMP)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
		else if (pRigid->GetSpeed() >= 0.f)
		{
			if (KEY_HOLD(KEY::LSHIFT))
				m_eCurState = PLAYER_STATE::RUN;
			else
				m_eCurState = PLAYER_STATE::WALK;
		}
	}

	if (KEY_TAP(KEY::SPACE) && GetGravity()->IsGround())
	{
 		m_eCurState = PLAYER_STATE::JUMP;
		
		if (pRigid)
			pRigid->AddVelocity(Vec2(0.f, -550.f));
	}

	if (!GetGravity()->IsGround() && m_eCurState != PLAYER_STATE::FLY)
	{
		float fVelocityY = pRigid->GetVelocity().y;
		if (fabs(pRigid->GetVelocity().y) < 0.1f)
			fVelocityY = 0.f;

		if (fVelocityY > 0.1f)
		{
			if (m_eCurState != PLAYER_STATE::TUMBLING_FALL && m_eCurState != PLAYER_STATE::FALL)
			{
				if (m_eCurState == PLAYER_STATE::JUMP)
					m_eCurState = PLAYER_STATE::TUMBLING_FALL;
				else
					m_eCurState = PLAYER_STATE::FALL;
			}
		}
	}

	if (m_eCurState == PLAYER_STATE::FALL)
	{
		Fall();
	}

	if (m_vecIsGround.size() > 0)
		m_fFallCount = 0.f;

}

// 캐릭터의 움직임 관리
void CPlayer::update_move()
{
	if (!m_bCanMove)
		return;

	CRigidBody* pRigid = GetRigidBody();

	Vec2 vForce(0.f, 0.f);

	if (KEY_HOLD(KEY::A))
	{
		if (KEY_HOLD(KEY::LSHIFT))
			pRigid->SetVelocity(Vec2(-200.f, pRigid->GetVelocity().y));
		else
			pRigid->SetVelocity(Vec2(-150.f, pRigid->GetVelocity().y));
	}

	if (KEY_HOLD(KEY::D))
	{
		if (KEY_HOLD(KEY::LSHIFT))
			pRigid->SetVelocity(Vec2(200.f, pRigid->GetVelocity().y));
		else
			pRigid->SetVelocity(Vec2(150.f, pRigid->GetVelocity().y));
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
	if (m_ePrevState == m_eCurState && m_iDir == m_iPrevDir && !m_bCanMove)
		return;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
		GetAnimator()->Play(L"KIRBY_IDLE", true);
		break;
	case PLAYER_STATE::WALK:
		GetAnimator()->Play(L"KIRBY_WALK", true);
		break;
	case PLAYER_STATE::RUN: 
		GetAnimator()->Play(L"KIRBY_RUN", true);
		break;
	case PLAYER_STATE::JUMP:
		GetAnimator()->Play(L"KIRBY_JUMP", true);
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::FLY:
		break;
	case PLAYER_STATE::FALL:
		GetAnimator()->Play(L"KIRBY_FALL", false);
		break;
	case PLAYER_STATE::TUMBLING_FALL:
		GetAnimator()->Play(L"KIRBY_TUMBLING_FALL", false);
		break;
	case PLAYER_STATE::DEEP_FALL:
		GetAnimator()->Play(L"KIRBY_DEEP_FALL", false);
		break;
	case PLAYER_STATE::LANDING:
		GetAnimator()->Play(L"KIRBY_LANDING", false);
	case PLAYER_STATE::DEAD:
		break;
	case PLAYER_STATE::INHALE_START:
		GetAnimator()->Play(L"KIRBY_INHALE_START", false);
		break;
	case PLAYER_STATE::INHALE_DEEP:
		GetAnimator()->Play(L"KIRBY_INHALE_DEEP", true);
		break;
	case PLAYER_STATE::INHALE_FINISH:
		GetAnimator()->Play(L"KIRBY_INHALE_FINISH", false);
		break;
	}
}

void CPlayer::update_gravity()
{
	GetRigidBody()->AddForce(Vec2(0.f, 400.f));
}

void CPlayer::MapRangeCheck()
{
	Vec2 vMapScale = CCamera::GetInst()->GetMapScale();
	Vec2 vPos = GetPos();

	if (GetPos().x <= 0 || GetPos().x >= vMapScale.x)
		SetPos(Vec2(m_vPrevPos.x, vPos.y));
	else if(GetPos().y <= 0 || GetPos().y >= vMapScale.y)
		SetPos(Vec2(vPos.x, m_vPrevPos.y));
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOther = _pOther->GetObj();

	if (pOther->GetName() == L"Ground")
	{
		if (!m_vecIsGround.empty())
			GetGravity()->SetGround(true);

		if (m_ePrevState == PLAYER_STATE::FALL)
			m_eCurState = PLAYER_STATE::LANDING;
		
		/*if(KEY_HOLD(KEY::A) || KEY_HOLD(KEY::D))
			m_eCurState = PLAYER_STATE::WALK;
		else
			m_eCurState = PLAYER_STATE::IDLE;*/
	}
}

void CPlayer::OnCollision(CCollider* _pOther)
{

}

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Ground")
	{
		vector<CObject*>::iterator iter = find(m_vecIsGround.begin(), m_vecIsGround.end(), _pOther->GetObj());

		if (iter != m_vecIsGround.end())
			m_vecIsGround.erase(iter);

		if (m_vecIsGround.empty())
			GetGravity()->SetGround(false);
	}
}
