#include "global.h"
#include "CStage_02.h"

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

CStage_02::CStage_02()
{
}

CStage_02::~CStage_02()
{
}

void CStage_02::update()
{
	CScene::update();
}

void CStage_02::render(HDC _dc)
{
	CScene::render(_dc);
}

void CStage_02::Enter()
{
	CTexture* pBgTex = CResMgr::GetInst()->LoadTexture(L"Stage_01", L"texture\\Kirby\\Background\\Kirby_Background_1.bmp");

	CBackground* bg = new CBackground(Vec2(0.f, 0.f), Vec2(520.f, 190.f));
	bg->SetBackgroundTex(pBgTex);
	AddObject(bg, GROUP_TYPE::BACK_GROUND);

	LoadMap(L"map\\GroundTest.map");

	CForeground* pMap = (CForeground*)GetGroupObject(GROUP_TYPE::FORE_GROUND)[0];
	assert(pMap);
	
	Vec2 vScale = pMap->GetImageSize();
	CCamera::GetInst()->SetMapScale(Vec2(vScale.x * 2.f, vScale.y * 2));

	CObject* pObj = new CPlayer;
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(270.f, 72.f));
	pObj->SetScale(Vec2(25.f, 25.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	CCamera::GetInst()->SetTarget(pObj);

	// 물리 충돌 체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

	// Camera Effect 설정
	CCamera::GetInst()->FadeIn(1.f);

	start();
}

void CStage_02::Exit()
{
	CCamera::GetInst()->FadeOut(1.f);
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}
