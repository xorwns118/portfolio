#include "global.h"
#include "CMonFactory.h"

#include "CRigidBody.h"

#include "CMonster.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CCollider.h"

#include "CResMgr.h"

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;
		pMon->SetPos(_vPos);

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 150.f;

		pMon->SetMonInfo(info);

		pMon->CreateRigidBody();
		pMon->GetRigidBody()->SetMass(1.f);

		pMon->GetCollider()->SetScale(Vec2(60.f, 70.f));
		pMon->GetCollider()->SetOffsetPos(Vec2(0.f, 25.f));

		CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"MonsterTex", L"texture\\Mushroom.bmp");

		pMon->CreateAnimator();
		pMon->GetAnimator()->CreateAnimation(L"MON_IDLE_LEFT", pTex, Vec2(0.f, 0.f), Vec2(100.f, 120.f), Vec2(0.f, 0.f), 0.3f, 1);
		pMon->GetAnimator()->CreateAnimation(L"MON_ATTACK_LEFT", pTex, Vec2(100.f, 0.f), Vec2(100.f, 120.f), Vec2(0.f, 0.f), 0.3f, 1);
		pMon->GetAnimator()->CreateAnimation(L"MON_HIT_LEFT", pTex, Vec2(200.f, 0.f), Vec2(100.f, 120.f), Vec2(0.f, 0.f), 0.3f, 1);
		pMon->GetAnimator()->CreateAnimation(L"MON_MOVE_LEFT", pTex, Vec2(0.f, 130.f), Vec2(100.f, 120.f), Vec2(100.f, 0.f), 0.3f, 5);
		pMon->GetAnimator()->CreateAnimation(L"MON_TRACE_LEFT", pTex, Vec2(0.f, 520.f), Vec2(100.f, 120.f), Vec2(100.f, 0.f), 0.3f, 5);

		pMon->GetAnimator()->CreateAnimation(L"MON_IDLE_RIGHT", pTex, Vec2(0.f, 390.f), Vec2(100.f, 120.f), Vec2(0.f, 0.f), 0.3f, 1);
		pMon->GetAnimator()->CreateAnimation(L"MON_ATTACK_RIGHT", pTex, Vec2(100.f, 390.f), Vec2(100.f, 120.f), Vec2(0.f, 0.f), 0.3f, 1);
		pMon->GetAnimator()->CreateAnimation(L"MON_HIT_RIGHT", pTex, Vec2(200.f, 390.f), Vec2(100.f, 120.f), Vec2(0.f, 0.f), 0.3f, 1);
		pMon->GetAnimator()->CreateAnimation(L"MON_MOVE_RIGHT", pTex, Vec2(0.f, 260.f), Vec2(100.f, 120.f), Vec2(100.f, 0.f), 0.3f, 5);
		pMon->GetAnimator()->CreateAnimation(L"MON_TRACE_RIGHT", pTex, Vec2(0.f, 650.f), Vec2(100.f, 120.f), Vec2(100.f, 0.f), 0.3f, 5);

		//pMon->GetAnimator()->Play(L"MON_TRACE_RIGHT", true);

		/*AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);
		pMon->SetAI(pAI);*/
	}
		break;
	case MON_TYPE::RANGE:
		break;
	default:
		break;
	}

	assert(pMon);

	return pMon;
}