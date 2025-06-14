#include "pch.h"
#include "CScene_Tool.h"

#include "CKeyMgr.h"
#include "CTile.h"

#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"

#include "resource.h"

#include "CUIMgr.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CPathMgr.h"

#include "SelectGDI.h"

#include "CPlayer.h"
#include "CGround.h"
#include "CElement.h"
#include "CPotal.h"

#include "CCollider.h"
#include "CCollisionMgr.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
	, m_pDragObj(nullptr)
	, m_iTargetIdx(-1)
	, m_bMouseOn{}
	, m_bLbtnDown(false)
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::Enter()
{
	// Tool Scene 에서 사용할 메뉴 생성
	CCore::GetInst()->DockMenu();

	// 타일 생성
	//CreateTile(5, 5);

	// UI 하나 만들어보기
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f, 150.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	AddObject(pPanelUI, GROUP_TYPE::UI);

	CBtnUI* pSaveBtn = new CBtnUI;
	pSaveBtn->SetName(L"Save");
	pSaveBtn->SetScale(Vec2(100.f, 40.f));
	pSaveBtn->SetPos(Vec2(150.f, 30.f));
	// 전역함수의 경우 &를 붙이지 않아도 주소로 인식하지만 멤버함수의 경우 &를 붙여주어야 인식함
	((CBtnUI*)pSaveBtn)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveMapData);
	pPanelUI->AddChild(pSaveBtn);

	CBtnUI* pLoadBtn = new CBtnUI;
	pLoadBtn->SetName(L"Load");
	pLoadBtn->SetScale(Vec2(100.f, 40.f));
	pLoadBtn->SetPos(Vec2(150.f, 80.f));
	// 전역함수의 경우 &를 붙이지 않아도 주소로 인식하지만 멤버함수의 경우 &를 붙여주어야 인식함
	((CBtnUI*)pLoadBtn)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::LoadMapData);
	pPanelUI->AddChild(pLoadBtn);

	// 복사본 UI
	/*CUI* pClonePanel = pPanelUI->Clone();
	pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f, 0.f));
	((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(ChangeScene, 0, 0);
	AddObject(pClonePanel, GROUP_TYPE::UI);

	m_pUI = pClonePanel;*/

	// Camera Look 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
}

void CScene_Tool::Exit()
{
	CCore::GetInst()->DivideMenu();

	DeleteAll();
}

void CScene_Tool::update()
{
	CScene::update();

	if (KEY_TAP(KEY::LBTN))
	{
		MouseOnCheck();
	}

	if (m_pDragObj && KEY_HOLD(KEY::LBTN))
	{
		MouseOn();
	}
}

void CScene_Tool::render(HDC _dc)
{
	CScene::render(_dc);

	if (m_iTargetIdx >= 0)
	{
		CObject* pObj = nullptr;

		if(m_strObjName == L"Ground")
			pObj = GetGroupObject(GROUP_TYPE::GROUND)[m_iTargetIdx];
		else if(m_strObjName == L"Element")
			pObj = GetGroupObject(GROUP_TYPE::ELEMENT)[m_iTargetIdx];
		else if(m_strObjName == L"Potal")
			pObj = GetGroupObject(GROUP_TYPE::POTAL)[m_iTargetIdx];

		Vec2 vPos = CCamera::GetInst()->GetRenderPos(pObj->GetPos());
		Vec2 vScale = pObj->GetScale();

		SelectGDI select(_dc, PEN_TYPE::RED);
		SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);

		CCollider* pCol = m_pDragObj->GetCollider();

		Rectangle(_dc
			, (int)(vPos.x - vScale.x / 2.f)
			, (int)(vPos.y - vScale.y / 2.f)
			, (int)(vPos.x + vScale.x / 2.f)
			, (int)(vPos.y + vScale.y / 2.f));
	}
}

void CScene_Tool::MouseOnCheck()
{
	Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

	vector<CObject*> vecElement = GetGroupObject(GROUP_TYPE::ELEMENT);
	vector<CObject*> vecGround = GetGroupObject(GROUP_TYPE::GROUND);
	vector<CObject*> vecPotal = GetGroupObject(GROUP_TYPE::POTAL);

	if (vecElement.size() + vecGround.size() + vecPotal.size() < 1)
		return;

	m_bMouseOn = false;
	m_pDragObj = nullptr;
	m_iTargetIdx = -1;

	for (int i = (int)vecElement.size() - 1; i >= 0; --i)
	{
		Vec2 vScale = vecElement[i]->GetScale();
		Vec2 vPos = vecElement[i]->GetPos();

		if (vPos.x - vScale.x / 2 <= vMousePos.x && vMousePos.x <= vPos.x + vScale.x / 2
			&& vPos.y + vScale.y / 2 >= vMousePos.y && vMousePos.y >= vPos.y - vScale.y / 2)
		{
			m_bMouseOn = true;
			m_pDragObj = vecElement[i];
			m_vDragStart = vMousePos;
			m_iTargetIdx = i;
			m_strObjName = L"Element";
			break;
		}
	}

	if (!m_pDragObj)
	{
		for (int i = (int)vecGround.size() - 1; i >= 0; --i)
		{
			Vec2 vScale = vecGround[i]->GetScale();
			Vec2 vPos = vecGround[i]->GetPos();

			if (vPos.x - vScale.x / 2 <= vMousePos.x && vMousePos.x <= vPos.x + vScale.x / 2
				&& vPos.y + vScale.y / 2 >= vMousePos.y && vMousePos.y >= vPos.y - vScale.y / 2)
			{
				m_bMouseOn = true;
				m_pDragObj = vecGround[i];
				m_vDragStart = vMousePos;
				m_iTargetIdx = i;
				m_strObjName = L"Ground";
				break;
			}
		}
	}

	if (!m_pDragObj)
	{
		for (int i = (int)vecPotal.size() - 1; i >= 0; --i)
		{
			Vec2 vScale = vecPotal[i]->GetScale();
			Vec2 vPos = vecPotal[i]->GetPos();

			if (vPos.x - vScale.x / 2 <= vMousePos.x && vMousePos.x <= vPos.x + vScale.x / 2
				&& vPos.y + vScale.y / 2 >= vMousePos.y && vMousePos.y >= vPos.y - vScale.y / 2)
			{
				m_bMouseOn = true;
				m_pDragObj = vecPotal[i];
				m_vDragStart = vMousePos;
				m_iTargetIdx = i;
				m_strObjName = L"Potal";
				break;
			}
		}
	}
}

void CScene_Tool::MouseOn()
{
	if (m_pDragObj)
	{
		Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		Vec2 vDiff = vMousePos - m_vDragStart;

		Vec2 vCurPos = m_pDragObj->GetPos();
		vCurPos += vDiff;
		m_pDragObj->SetPos(vCurPos);

		m_vDragStart = vMousePos;
	}
}

void CScene_Tool::MouseLbtnDown()
{
}

void CScene_Tool::MouseLbtnUp()
{
}

void CScene_Tool::MouseLbtnClicked()
{
}

void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
		
		int iTileX = GetTileX();
		int iTileY = GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		if (vMousePos.x < 0.f || iTileX <= iCol
			|| vMousePos.y < 0.f || iTileY <= iRow)
		{
			return;
		}

		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		((CTile*)vecTile[iIdx])->AddImage();
	}
}

// 이중 포인터
// 포인터 타입의 값에 접근 하기 위해 원본 포인터에 포인터를 사용하는 것 **
// (주소를 가진 변수의 주소)

void CScene_Tool::SaveTileData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal
	if (GetSaveFileName(&ofn))
	{
		SaveTile(szName);
	}
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	assert(pFile);

	// 타일 가로 세로 개수 저장
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);

	// 모든 타일들을 개별적으로 저장할 데이터를 저장하게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}

	fclose(pFile);
}

void CScene_Tool::LoadTileData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}

void CScene_Tool::SaveMapData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Map\0*.map\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strMapFolder = CPathMgr::GetInst()->GetContentPath();
	strMapFolder += L"map";

	ofn.lpstrInitialDir = strMapFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn))
	{
		SaveMap(szName);
	}
}

void CScene_Tool::SaveMap(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	assert(pFile);
	
	wstring strTexPath = GetMapPath();
	string strName = string(strTexPath.begin(), strTexPath.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	vector<CObject*> vecMap = GetGroupObject(GROUP_TYPE::FORE_GROUND);

	int iMapSliceCount = (int)vecMap.size();
	fwrite(&iMapSliceCount, sizeof(float), 1, pFile);

	for (int i = 0; i < iMapSliceCount; ++i)
	{
		Vec2 vLT = GetMapInfo()[i].m_vFgLT;
		Vec2 vRB = GetMapInfo()[i].m_vFgRB;
		Vec2 vPos = GetMapInfo()[i].m_vFgPos;

		fwrite(&vLT.x, sizeof(float), 1, pFile);
		fwrite(&vLT.y, sizeof(float), 1, pFile);
		fwrite(&vRB.x, sizeof(float), 1, pFile);
		fwrite(&vRB.y, sizeof(float), 1, pFile);
		fwrite(&vPos.x, sizeof(float), 1, pFile);
		fwrite(&vPos.y, sizeof(float), 1, pFile);
	}

	int iElementCount = (int)GetElementPos().size();
	fwrite(&iElementCount, sizeof(int), 1, pFile);

	for (int i = 0; i < iElementCount; ++i)
	{
		Vec2 vLT = GetElementPos()[i].m_vElementLT;
		Vec2 vRB = GetElementPos()[i].m_vElementRB;
		Vec2 vPos = GetGroupObject(GROUP_TYPE::ELEMENT)[i]->GetPos();

		fwrite(&vLT.x, sizeof(float), 1, pFile);
		fwrite(&vLT.y, sizeof(float), 1, pFile);
		fwrite(&vRB.x, sizeof(float), 1, pFile);
		fwrite(&vRB.y, sizeof(float), 1, pFile);
		fwrite(&vPos.x, sizeof(float), 1, pFile);
		fwrite(&vPos.y, sizeof(float), 1, pFile);
	}

	vector<CObject*> vecGround = GetGroupObject(GROUP_TYPE::GROUND);

	int iGroundCount = (int)vecGround.size();
	fwrite(&iGroundCount, sizeof(int), 1, pFile);

	UINT iGroundType = 0;

	for (int i = 0; i < iGroundCount; ++i)
	{
		if (vecGround[i]->GetCollider()->GetColliderType() == COLLIDER_TYPE::RECTANGLE)
		{
			iGroundType = (UINT)COLLIDER_TYPE::RECTANGLE;
			fwrite(&iGroundType, sizeof(UINT), 1, pFile);

			Vec2 vPos = vecGround[i]->GetPos();
			Vec2 vScale = vecGround[i]->GetScale();

			fwrite(&vPos.x, sizeof(float), 1, pFile);
			fwrite(&vPos.y, sizeof(float), 1, pFile);
			fwrite(&vScale.x, sizeof(float), 1, pFile);
			fwrite(&vScale.y, sizeof(float), 1, pFile);
		}
		else if (vecGround[i]->GetCollider()->GetColliderType() == COLLIDER_TYPE::TRIANGLE)
		{
			iGroundType = (UINT)COLLIDER_TYPE::TRIANGLE;
			fwrite(&iGroundType, sizeof(UINT), 1, pFile);

			tTriangle vecDot = vecGround[i]->GetCollider()->GetTriPoint();
			Vec2 vPos = vecGround[i]->GetPos();
			Vec2 vStartPos = vecDot.pDot[0];
			Vec2 vMiddlePos = vecDot.pDot[1];
			Vec2 vEndPos = vecDot.pDot[2];

			fwrite(&vPos.x, sizeof(float), 1, pFile);
			fwrite(&vPos.y, sizeof(float), 1, pFile);
			fwrite(&vStartPos.x, sizeof(float), 1, pFile);
			fwrite(&vStartPos.y, sizeof(float), 1, pFile);
			fwrite(&vMiddlePos.x, sizeof(float), 1, pFile);
			fwrite(&vMiddlePos.y, sizeof(float), 1, pFile);
			fwrite(&vEndPos.x, sizeof(float), 1, pFile);
			fwrite(&vEndPos.y, sizeof(float), 1, pFile);
		}
	}

	vector<CObject*> vecPotal = GetGroupObject(GROUP_TYPE::POTAL);

	int iPotalCount = (int)vecPotal.size();
	fwrite(&iPotalCount, sizeof(int), 1, pFile);

	for (int i = 0; i < iPotalCount; ++i)
	{
		Vec2 vPos = vecPotal[i]->GetPos();
		CPotal* pPotal = (CPotal*)vecPotal[i];
		UINT iNextScene = (UINT)pPotal->GetNextScene();

		fwrite(&vPos.x, sizeof(float), 1, pFile);
		fwrite(&vPos.y, sizeof(float), 1, pFile);
		fwrite(&iNextScene, sizeof(UINT), 1, pFile);
	}

	fclose(pFile);
}

void CScene_Tool::LoadMapData()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Map\0*.map\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strMapFolder = CPathMgr::GetInst()->GetContentPath();
	strMapFolder += L"map";

	ofn.lpstrInitialDir = strMapFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadMap(strRelativePath);
	}
}

void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}

//============
// Window Proc
//============
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);
			
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			
			// ToolScene 확인
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
			pToolScene->CreateTile(iXCount, iYCount);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MapSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iLTX = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iLTY = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			UINT iRBX = GetDlgItemInt(hDlg, IDC_EDIT3, nullptr, false);
			UINT iRBY = GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, false);

			UINT iPosX = GetDlgItemInt(hDlg, IDC_EDIT5, nullptr, false);
			UINT iPosY = GetDlgItemInt(hDlg, IDC_EDIT6, nullptr, false);

			UINT iMapNum = GetDlgItemInt(hDlg, IDC_EDIT7, nullptr, false);

			UINT iCount = GetDlgItemInt(hDlg, IDC_EDIT8, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			wchar_t szPath[256] = {};
			swprintf_s(szPath, L"texture\\Kirby\\Foreground\\Kirby_Foreground_1-%d.bmp", iMapNum);

			pCurScene->SetMapKey(L"Map");
			pCurScene->SetMapPath(szPath);

			pToolScene->DeleteGroup(GROUP_TYPE::BACK_GROUND);
			pToolScene->CreateMap(Vec2((float)iLTX, (float)iLTY)
								, Vec2((float)iRBX, (float)iRBY)
								, Vec2((float)iPosX, (float)iPosY), iCount);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreatElementProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iLTX = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iLTY = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			UINT iRBX = GetDlgItemInt(hDlg, IDC_EDIT3, nullptr, false);
			UINT iRBY = GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, false);

			UINT iMapNum = GetDlgItemInt(hDlg, IDC_EDIT5, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			wchar_t szPath[256] = {};
			swprintf_s(szPath, L"texture\\Kirby\\Foreground\\Kirby_Foreground_1-%d.bmp", iMapNum);

			pCurScene->SetMapKey(L"Map");
			pCurScene->SetMapPath(szPath);

			pToolScene->CreateElement(Vec2((float)iLTX, (float)iLTY)
				, Vec2((float)iRBX, (float)iRBY)
				, CCamera::GetInst()->GetLookAt());

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DeleteElementProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			int iIdx = pToolScene->GetDelTargetIdx();
			vector<CObject*> vecElement = pCurScene->GetGroupObject(GROUP_TYPE::ELEMENT);

			CElement* pElement = dynamic_cast<CElement*>(pToolScene->GetTargetObj());

			if (pElement && iIdx < vecElement.size())
			{
				DeleteObject(pElement);
				pToolScene->SetDelTargetIdx(-1);
				pCurScene->DeleteElementInfo(iIdx);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateRectGroundProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iSizeX = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iSizeY = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->CreateGroundRectangle
			(
				CCamera::GetInst()->GetLookAt(), Vec2((float)iSizeX, (float)iSizeY)
			);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateTriGroundProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			// 꼭짓점을 이을 위치
			// dot1, dot2, dot3
			UINT iStartX = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iStartY = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);
			UINT iMiddleX = GetDlgItemInt(hDlg, IDC_EDIT3, nullptr, false);
			UINT iMiddleY = GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, false);
			UINT iEndX = GetDlgItemInt(hDlg, IDC_EDIT5, nullptr, false);
			UINT iEndY = GetDlgItemInt(hDlg, IDC_EDIT6, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->CreateGroundTriangle
			(
				CCamera::GetInst()->GetLookAt(),
				Vec2((float)iStartX, (float)iStartY),
				Vec2((float)iMiddleX, (float)iMiddleY),
				Vec2((float)iEndX, (float)iEndY)
			);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DeleteGroundProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			int iIdx = pToolScene->GetDelTargetIdx();
			vector<CObject*> vecGround = pCurScene->GetGroupObject(GROUP_TYPE::GROUND);

			CGround* pGround = dynamic_cast<CGround*>(pToolScene->GetTargetObj());

			if (pGround && iIdx < vecGround.size())
			{
				DeleteObject(pGround);
				pToolScene->SetDelTargetIdx(-1);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreatePotalProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iNextScene = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);

			if (iNextScene >= (UINT)SCENE_TYPE::END)
				return (INT_PTR)TRUE;

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->CreatePotal(iNextScene, CCamera::GetInst()->GetLookAt());

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}