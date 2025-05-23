#pragma once
#include "CScene.h"
#include "CAnimation.h"

class CTexture;

class CAnimation_Tool : public CScene
{
private:
	CObject*			m_pAnimPlayer;
	vector<tAnimFrm>	m_vecFrm;

	wstring				m_strPath;

public:
	virtual void update();
	virtual void render(HDC _dc);

	virtual void Enter();
	virtual void Exit();

public:
	vector<tAnimFrm> GetFrameVector() { return m_vecFrm; }

	void SetTexPath(const wstring& _strPath) { m_strPath = _strPath; }
	const wstring& GetTexPath() { return m_strPath; }

	void LoadAnimData();

public:
	CAnimation_Tool();
	~CAnimation_Tool();
};

