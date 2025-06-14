#include "pch.h"
#include "CScene.h"

#include "CObject.h"
#include "CTile.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "CCamera.h"
#include "CCore.h"

#include "CForeground.h"
#include "CElement.h"
#include "CGround.h"
#include "CPotal.h"

#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CSceneMgr.h"
#include "CCollider.h"

CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j 물체 삭제
			delete m_arrObj[i][j];
		}
	}
}

void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->start();
		}
	}
}

void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->update();
			}
		}
	}
}

void CScene::finalupdate()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->finalupdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		if (i == (UINT)GROUP_TYPE::TILE)
		{
			render_tile(_dc);
			continue;
		}

		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter < m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);
				++iter;
			}
			else
			{
				iter = m_arrObj[i].erase(iter);
			}
		}
	}
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vLeftPos = vCamLook - vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	int iLTCol = (int)vLeftPos.x / iTileSize;
	int iLTRow = (int)vLeftPos.y / iTileSize;
	int iLTIdx = (m_iTileX * iLTRow) + iLTCol;

	int iClientWidth = ((int)vResolution.x / iTileSize) + 1;
	int iClientHeight = ((int)vResolution.y / iTileSize) + 1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			if (iCurCol < 0 || (int)m_iTileX <= iCurCol
				|| iCurRow < 0 || (int)m_iTileY <= iCurRow)
				continue;

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			vecTile[iIdx]->render(_dc);
		}
	}

	// 전체 배경 텍스쳐 한장 만들어서 BitBlt 으로 한번에 찍어내서 부분적으로 렌더링하면 좀 더 최적화된 상황을 만들 수 있음.
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eTarget]); // 함수 템플릿
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;

	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\TILE.bmp");

	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();

			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			pTile->SetTexture(pTileTex);

			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb"); // 읽어올 때 R, 쓸 때 W .... (b는 binary)
	assert(pFile);

	// 타일 가로 세로 개수 불러오기
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);
	
	// 불러온 개수에 맞게 EmptyTile 들 만들어두기
	CreateTile(xCount, yCount);

	// 만들어진 타일 개별로 필요한 정보를 불러오게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	fclose(pFile);
}

void CScene::DeleteElementInfo(int _iIdx)
{
	if (_iIdx >= 0 && _iIdx < (int)m_vecElementInfo.size())
	{
		m_vecElementInfo.erase(m_vecElementInfo.begin() + _iIdx);
	}
}

void CScene::CreateMap(Vec2 _vLT, Vec2 _vRB, Vec2 _vPos, UINT _iCount)
{
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(m_strMapKey, m_strMapPath);

	for (UINT i = 0; i < _iCount; ++i)
	{
		float fScale = _vRB.x - _vLT.x + 1.f;

		tMapInfo info = {};

		info.m_vFgLT = Vec2(_vLT.x + (i * fScale), _vLT.y);
		info.m_vFgRB = Vec2(_vRB.x + (i * fScale), _vRB.y);
		info.m_vFgPos = Vec2(_vPos.x + (i * fScale * 2), _vPos.y);

		CForeground* pFg = new CForeground(info.m_vFgLT, info.m_vFgRB - info.m_vFgLT + Vec2(1.f, 1.f), info.m_vFgPos);

		pFg->SetPos(_vPos);
		pFg->SetForegroundTex(pTex);

		AddObject(pFg, GROUP_TYPE::FORE_GROUND);

		m_vecMapInfo.push_back(info);
	}
}

void CScene::CreateElement(Vec2 _vLT, Vec2 _vRB, Vec2 _vPos)
{
	tElementInfo info = {};

	info.m_vElementLT = _vLT;
	info.m_vElementRB = _vRB;
	info.m_vElementPos = _vPos;

	// L"Stage_1-1", L"texture\\Kirby\\Foreground\\Kirby_Foreground_1-1.bmp"
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(m_strMapKey, m_strMapPath);

	// 예시
	// CBush* pBush = new CBush(pTex, Vec2(165.f, 220.f), Vec2(276.f, 234.f));

	CElement* pElement = new CElement(pTex, _vLT, _vRB);

	pElement->SetPos(_vPos);
	pElement->SetScale((_vRB - _vLT + Vec2(1.f, 1.f)) * 2.f);

	AddObject(pElement, GROUP_TYPE::ELEMENT);

	m_vecElementInfo.push_back(info);
}

void CScene::CreateGroundRectangle(Vec2 _vPos, Vec2 _vScale)
{
	CGround* pGround = new CGround;

	pGround->SetName(L"Ground");
	pGround->SetPos(_vPos);
	pGround->SetScale(_vScale);
	if (GetName() == L"Tool Scene")
	{
		pGround->CreateMarker(_vScale);
	}
	AddObject(pGround, GROUP_TYPE::GROUND);
}

void CScene::CreateGroundTriangle(Vec2 _vPos, const Vec2 _vStartPos, const Vec2 _vMiddlePos, const Vec2 _vEndPos)
{
	CGround* pGround = new CGround(_vStartPos, _vMiddlePos, _vEndPos);

	pGround->SetName(L"Ground");
	pGround->SetPos(_vPos);

	CCollider* pCol = pGround->GetCollider();

	pGround->SetScale(pCol->GetTriScale());
	pCol->SetTriPos(_vPos);
	if (GetName() == L"Tool Scene")
	{
		pGround->CreateMarker(pGround->GetScale());
	}

	AddObject(pGround, GROUP_TYPE::GROUND);
}

void CScene::CreatePotal(UINT _iNextScene, Vec2 _vPos)
{
	CPotal* pPotal = new CPotal(_iNextScene);

	pPotal->SetPos(_vPos);

	AddObject(pPotal, GROUP_TYPE::POTAL);
}

void CScene::LoadMap(const wstring& _strRelativePath)
{
	DeleteGroup(GROUP_TYPE::GROUND);
	DeleteGroup(GROUP_TYPE::FORE_GROUND);
	DeleteGroup(GROUP_TYPE::ELEMENT);
	DeleteGroup(GROUP_TYPE::POTAL);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	string str;
	char szBuff[256] = {};

	FScanf(szBuff, pFile);

	str = szBuff;
	m_strMapPath = wstring(str.begin(), str.end());
	
	m_strMapKey = L"Map";

	int iMapSliceCount = 0;
	fread(&iMapSliceCount, sizeof(float), 1, pFile);

	for (int i = 0; i < iMapSliceCount; ++i)
	{
		Vec2 vLT = Vec2(0.f, 0.f);
		Vec2 vRB = Vec2(0.f, 0.f);
		Vec2 vPos = Vec2(0.f, 0.f);

		fread(&vLT.x, sizeof(float), 1, pFile);
		fread(&vLT.y, sizeof(float), 1, pFile);
		fread(&vRB.x, sizeof(float), 1, pFile);
		fread(&vRB.y, sizeof(float), 1, pFile);
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		CreateMap(vLT, vRB, vPos, 1);
	}

	int elementCount = 0;
	fread(&elementCount, sizeof(int), 1, pFile);

	for (int i = 0; i < elementCount; ++i)
	{
		Vec2 iElementLT = Vec2(0.f, 0.f);
		Vec2 iElementRB = Vec2(0.f, 0.f);
		Vec2 iElementPos = Vec2(0.f, 0.f);

		fread(&iElementLT.x, sizeof(float), 1, pFile);
		fread(&iElementLT.y, sizeof(float), 1, pFile);
		fread(&iElementRB.x, sizeof(float), 1, pFile);
		fread(&iElementRB.y, sizeof(float), 1, pFile);
		fread(&iElementPos.x, sizeof(float), 1, pFile);
		fread(&iElementPos.y, sizeof(float), 1, pFile);

		CreateElement(iElementLT, iElementRB, iElementPos);
	}

	int iGroundCount = 0;
	fread(&iGroundCount, sizeof(int), 1, pFile);

	UINT iGroundType = 0;

	for (int i = 0; i < iGroundCount; ++i)
	{
		fread(&iGroundType, sizeof(int), 1, pFile);
		if (iGroundType == (UINT)COLLIDER_TYPE::RECTANGLE)
		{
			Vec2 vPos = Vec2(0.f, 0.f);
			Vec2 vScale = Vec2(0.f, 0.f);

			fread(&vPos.x, sizeof(float), 1, pFile);
			fread(&vPos.y, sizeof(float), 1, pFile);
			fread(&vScale.x, sizeof(float), 1, pFile);
			fread(&vScale.y, sizeof(float), 1, pFile);

			CreateGroundRectangle(vPos, vScale);
		}
		else if (iGroundType == (UINT)COLLIDER_TYPE::TRIANGLE)
		{
			Vec2 vPos = Vec2(0.f, 0.f);
			Vec2 vStartPos = Vec2(0.f, 0.f);
			Vec2 vMiddlePos = Vec2(0.f, 0.f);
			Vec2 vEndPos = Vec2(0.f, 0.f);

			fread(&vPos.x, sizeof(float), 1, pFile);
			fread(&vPos.y, sizeof(float), 1, pFile);
			fread(&vStartPos.x, sizeof(float), 1, pFile);
			fread(&vStartPos.y, sizeof(float), 1, pFile);
			fread(&vMiddlePos.x, sizeof(float), 1, pFile);
			fread(&vMiddlePos.y, sizeof(float), 1, pFile);
			fread(&vEndPos.x, sizeof(float), 1, pFile);
			fread(&vEndPos.y, sizeof(float), 1, pFile);

			CreateGroundTriangle(vPos, vStartPos, vMiddlePos, vEndPos);
		}
	}

	int iPotalCount = 0;
	fread(&iPotalCount, sizeof(int), 1, pFile);

	for (int i = 0; i < iPotalCount; ++i)
	{
		Vec2 vPos = Vec2(0.f, 0.f);
		UINT iNextScene = 0;

		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);
		fread(&iNextScene, sizeof(UINT), 1, pFile);

		CreatePotal(iNextScene, vPos);
	}

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CCamera::GetInst()->SetLookAt(vResolution / 2);

	fclose(pFile);
}