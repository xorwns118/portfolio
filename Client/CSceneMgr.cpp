#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CAnimation_Tool.h"
#include "CStage_01.h"
#include "CStage_02.h"
#include "CEventMgr.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	// 씬 전부 삭제
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (m_arrScene[i] != nullptr)
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneMgr::init()
{
	// Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");

	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");

	m_arrScene[(UINT)SCENE_TYPE::ANIM_TOOL] = new CAnimation_Tool;
	m_arrScene[(UINT)SCENE_TYPE::ANIM_TOOL]->SetName(L"Animation Tool Scene");
	
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CStage_01;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01]->SetName(L"Stage_1 Scene");

	m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CStage_02;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_02]->SetName(L"Stage_2 Scene");

	// 현재 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::STAGE_01];
	m_pCurScene->Enter();
}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->finalupdate();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();

	m_pCurScene = m_arrScene[(UINT)_eNext];

	m_pCurScene->Enter();
}