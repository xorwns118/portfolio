#pragma once

#include "CAnimation.h"

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim;		// 모든 Animation
	CAnimation*					m_pPrevAnim;	// 이전에 재생한 Animation
	CAnimation*					m_pCurAnim;		// 현재 재생중인 Animation
	CObject*					m_pOwner;		// Animator 소유 Object
	bool						m_bRepeat;		// 반복재생 여부

	int							m_iDir;			// 애니메이션 좌우 대칭 방향

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, vector<tAnimFrm> _vecList, UINT _iFrameCount);
	void LoadAnimation(const wstring& _strRelativePath);
	void CreateAnimDiffSize(vector<tAnimFrm>& _vecList, Vec2 _vLT, Vec2 _vRB, float _f);

	void SetAnimDir(ANIM_DIR _eDir) { m_iDir = (int)_eDir; }
	int GetAnimDir() { return m_iDir; }

	CAnimation* GetCurAnim() { return m_pCurAnim; }

	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName, bool _b);

	void update();
	void finalupdate();
	void render(HDC _dc);

public:
	CAnimator();
	~CAnimator();

	friend class CObject;
};

