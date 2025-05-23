#include "global.h"
#include "CElement.h"

#include "CResMgr.h"

#include "CTexture.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CCollider.h"

#include "CPlayer.h"

CElement::CElement(CTexture* _pTex, Vec2 _vLT, Vec2 _vRB) // 나열 방법 체크 추가, 높이 or 간격 추가
	: m_pTex(_pTex)
	, m_vLT(_vLT)
	, m_vSize(_vRB - _vLT)
	, m_bIsPlay(false)
{
	CreateAnimator();
	 
	vector<tAnimFrm> vecList;

	GetAnimator()->CreateAnimDiffSize(vecList, Vec2(m_vLT.x, _vRB.y - 14.f), _vRB, 0.1f);
	GetAnimator()->CreateAnimDiffSize(vecList, Vec2(m_vLT.x, _vRB.y + 2.f), Vec2(_vRB.x, _vRB.y + 16.f), 0.1f);
	GetAnimator()->CreateAnimDiffSize(vecList, Vec2(m_vLT.x, _vRB.y + 18.f), Vec2(_vRB.x, _vRB.y + 32.f), 0.1f);
	GetAnimator()->CreateAnimDiffSize(vecList, Vec2(m_vLT.x, _vRB.y + 34.f), Vec2(_vRB.x, _vRB.y + 48.f), 0.1f);
	GetAnimator()->CreateAnimation(L"ELEMENT_SHAKE", m_pTex, vecList, 4);
	vecList.clear();

	GetAnimator()->Play(L"ELEMENT_SHAKE", true);
}

CElement::~CElement()
{
}

void CElement::update()
{
}

void CElement::render(HDC _dc)
{
	component_render(_dc);
}