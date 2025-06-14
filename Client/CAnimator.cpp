#include "pch.h"
#include "CAnimator.h"

#include "CAnimation.h"

CAnimator::CAnimator()
	: m_pCurAnim(nullptr)
	, m_pPrevAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(true)
	, m_iDir(1)
{

}

// 복사생성자 추가

CAnimator::~CAnimator()
{
	Safe_Delete_Map<wstring, CAnimation*>(m_mapAnim);
}

void CAnimator::update()
{
	
}

void CAnimator::finalupdate()
{
	if (m_pCurAnim != nullptr)
	{
		if (m_pCurAnim != m_pPrevAnim)
		{
			m_pCurAnim->SetFinish(false);
			m_pCurAnim->SetFrame(0);
		}

		m_pCurAnim->update();

		if (m_bRepeat && m_pCurAnim->IsFinish()) 
		{
			m_pCurAnim->SetFrame(0);
		}
		
		if (!m_bRepeat && m_pCurAnim->IsFinish())
		{
			// 반복이 false 일 때 애니메이션의 마지막 프레임으로 고정
			m_pCurAnim->SetFrame(m_pCurAnim->GetMaxFrame() - 1);
		}
	}

	m_pPrevAnim = m_pCurAnim;
}

void CAnimator::render(HDC _dc)
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->render(_dc);
}

// 잘라내야할 텍스쳐 여러개가 같은 사이즈로 일정한 간격일 때
void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize
								, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(pAnim == nullptr);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

// 잘라내야할 텍스쳐 여러개의 위치 및 크기가 각각 다를 때
void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, vector<tAnimFrm> _vecList, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(pAnim == nullptr);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vecList, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::LoadAnimation(const wstring& _strRelativePath)
{
	CAnimation* pAnim = new CAnimation();
	pAnim->Load(_strRelativePath);

	pAnim->m_pAnimator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

void CAnimator::CreateAnimDiffSize(vector<tAnimFrm>& _vecList, Vec2 _vLT, Vec2 _vRB, float _f)
{
	tAnimFrm info = {};

	info.fDuration = _f;
	info.vLT = _vLT;
	info.vSlice = Vec2(_vRB.x - _vLT.x + 2.f, _vRB.y - _vLT.y + 2.f);

	_vecList.push_back(info);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _b)
{
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _b;
}