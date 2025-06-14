#include "pch.h"
#include "CAnimation.h"

#include "CAnimator.h"
#include "CTexture.h"

#include "CObject.h"
#include "CTimeMgr.h"
#include "CCamera.h"
#include "CResMgr.h"

#include "CPathMgr.h"

CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
	, m_iSecStartFrm(0)
	, m_iSecEndFrm(0)
{

}

CAnimation::~CAnimation()
{
	
}

void CAnimation::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;

	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();
	vPos += m_vecFrm[m_iCurFrm].vOffset;

	// 화면 렌더링 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	const tAnimFrm& frame = m_vecFrm[m_iCurFrm];
	int frameW = (int)frame.vSlice.x;
	int frameH = (int)frame.vSlice.y;

	int destW = frameW * 2;
	int destH = frameH * 2;

	int destX = (int)(vPos.x - frameW);
	int destY = (int)(vPos.y - frameH);

	HDC hMemDC = CreateCompatibleDC(_dc);
	HBITMAP hMemBmp = CreateCompatibleBitmap(_dc, destW, destH);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hMemBmp);

	// 좌우 반전 여부
	bool bFlip = (m_pAnimator->GetAnimDir() == -1);

	// 이미지 복사 (반전 포함)
	if (bFlip)
	{
		StretchBlt(hMemDC,
			0, 0, destW, destH,
			m_pTex->GetDC(),
			(int)(frame.vLT.x + frame.vSlice.x - 1),
			(int)frame.vLT.y,
			-frameW, frameH,
			SRCCOPY);
	}
	else
	{
		StretchBlt(hMemDC,
			0, 0, destW, destH,
			m_pTex->GetDC(),
			(int)frame.vLT.x,
			(int)frame.vLT.y,
			frameW, frameH,
			SRCCOPY);
	}

	TransparentBlt(_dc,
		destX, destY,
		destW, destH,
		hMemDC,
		0, 0,
		destW, destH,
		RGB(255, 0, 255));

	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hMemBmp);
	DeleteDC(hMemDC);
}

// 잘라내야할 텍스쳐 여러개가 같은 사이즈로 일정한 간격일 때
void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i;

		m_vecFrm.push_back(frm);
	}
}

// 잘라내야할 텍스쳐 여러개의 위치 및 크기가 각각 다를 때
void CAnimation::Create(CTexture* _pTex, vector<tAnimFrm> _vecList, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _vecList[i].fDuration;
		frm.vSlice = _vecList[i].vSlice;
		frm.vLT = _vecList[i].vLT;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// Animation 의 이름을 저장한다. (데이터 직렬화)
	fprintf(pFile, "[Animation Name]\n");
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	// Animation 이 사용하는 텍스쳐
	fprintf(pFile, "[Texture Name]\n");
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");
	
	// 프레임 개수
	fprintf(pFile, "[Frame Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());
	fprintf(pFile, "\n\n");

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n", (int)i);

		fprintf(pFile, "[Left Top]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", m_vecFrm[i].fDuration);

		fprintf(pFile, "\n\n");
	}

	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);
	
	// Animation 의 이름을 읽어온다.
	string str;
	char szBuff[256] = {};

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);
	
	str = szBuff;
	m_strName = wstring(str.begin(), str.end());
	
	// 참조하는 텍스쳐 이름 및 경로
	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);

	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);

	// 프레임 개수
	FScanf(szBuff, pFile);
	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount);

	// 모든 프레임 정보
	tAnimFrm frm = {};

	for (int i = 0; i < iFrameCount; ++i)
	{
		POINT pt = {};

		while (true)
		{
			FScanf(szBuff, pFile);

			if (!strcmp("[Frame Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left Top]", szBuff))
			{
				fscanf_s(pFile, "%d, %d", &pt.x, &pt.y);

				frm.vLT = pt;
			}
			else if (!strcmp("[Slice Size]", szBuff))
			{
				fscanf_s(pFile, "%d, %d", &pt.x, &pt.y);

				frm.vSlice = pt;
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d, %d", &pt.x, &pt.y);

				frm.vOffset = pt;
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &frm.fDuration);
				break;
			}
		}

		m_vecFrm.push_back(frm);
	}

	fclose(pFile);
}

// 애니메이션을 사용하는 주체별로 파일을 분리해서, 파일 하나에 모든 애니메이션 정보를 넣는 방법도 있음.