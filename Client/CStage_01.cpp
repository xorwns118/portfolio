#include "global.h"
#include "CStage_01.h"

#include "CCore.h"

#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CResMgr.h"

#include "CTexture.h"
#include "CPlayer.h"
#include "CGround.h"
#include "CElement.h"

#include "CRigidBody.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CBackground.h"
#include "CForeground.h"

CStage_01::CStage_01()
	: m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
{
}

CStage_01::~CStage_01()
{
}

void CStage_01::update()
{
	CScene::update();
}

void CStage_01::render(HDC _dc)
{
	CScene::render(_dc);
}

void CStage_01::Enter()
{
	// 백그라운드 이미지 생성
	CTexture* pBgTex = CResMgr::GetInst()->LoadTexture(L"Stage_01", L"texture\\Kirby\\Background\\Kirby_Background_1.bmp");

	CBackground* bg = new CBackground(Vec2(0.f, 0.f), Vec2(520.f, 190.f));
	bg->SetBackgroundTex(pBgTex);
	AddObject(bg, GROUP_TYPE::BACK_GROUND);

	LoadMap(L"map\\GroundTest.map");

	vector<CObject*> pMap = GetGroupObject(GROUP_TYPE::FORE_GROUND);

	Vec2 vScale = Vec2(0.f, 0.f);

	for (size_t i = 0; i < pMap.size(); ++i)
	{
		CForeground* pGround = (CForeground*)pMap[i];
		vScale += pGround->GetImageSize() * 2.f;
	}

	CCamera::GetInst()->SetMapScale(vScale);

	CObject* pObj = new CPlayer;
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(270.f, 72.f));
	pObj->SetScale(Vec2(25.f, 25.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	RegisterPlayer(pObj);

	CCamera::GetInst()->SetTarget(pObj);

	// 물리 충돌 체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

	// Camera Effect 설정
	CCamera::GetInst()->FadeIn(1.f);

	start();
}

void CStage_01::Exit()
{
	CCamera::GetInst()->FadeOut(1.f);
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}
