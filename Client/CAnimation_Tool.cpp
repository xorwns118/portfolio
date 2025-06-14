#include "global.h"
#include "CAnimation_Tool.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CCore.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"

#include "resource.h"

#include "CMonster.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CCollider.h"

CAnimation_Tool::CAnimation_Tool()
	: m_pAnimPlayer(nullptr)
{
}

CAnimation_Tool::~CAnimation_Tool()
{
}

void CAnimation_Tool::update()
{
	CScene::update();
}

void CAnimation_Tool::render(HDC _dc)
{
	CScene::render(_dc);
}

void CAnimation_Tool::Enter()
{
	CCore::GetInst()->DockMenu();

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CObject* pMon = new CMonster;
	pMon->SetScale(Vec2(20.f, 20.f));
	pMon->SetPos(vResolution / 2.f);
	pMon->CreateAnimator();
	pMon->SetName(L"Animation Player");

	AddObject(pMon, GROUP_TYPE::MONSTER);
	m_pAnimPlayer = pMon;
	m_pAnimPlayer->GetCollider()->SetScale(m_pAnimPlayer->GetScale());

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f, 150.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	AddObject(pPanelUI, GROUP_TYPE::UI);

	CBtnUI* pLoadBtn = new CBtnUI;
	pLoadBtn->SetName(L"LoadAnim");
	pLoadBtn->SetScale(Vec2(100.f, 40.f));
	pLoadBtn->SetPos(Vec2(pPanelUI->GetScale().x / 2.f, pPanelUI->GetScale().y / 2.f));
	((CBtnUI*)pLoadBtn)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CAnimation_Tool::LoadAnimData);
	pPanelUI->AddChild(pLoadBtn);

	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	start();
}

void CAnimation_Tool::Exit()
{
	CCore::GetInst()->DivideMenu();

	DeleteAll();
}

void CAnimation_Tool::LoadAnimData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Animation\0*.anim\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"animation";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal
	if (GetOpenFileName(&ofn))
	{
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + CPathMgr::GetInst()->GetRelativePath(szName);

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
		assert(pFile);

		string str;
		char szBuff[256] = {};

		FScanf(szBuff, pFile);
		FScanf(szBuff, pFile);

		str = szBuff;
		wstring m_strName = wstring(str.begin(), str.end());
		m_pAnimPlayer->GetAnimator()->LoadAnimation(CPathMgr::GetInst()->GetRelativePath(szName));
		m_pAnimPlayer->GetAnimator()->Play(m_strName, true);
	}
}