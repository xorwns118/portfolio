#pragma once
#include "CObject.h"

class CTexture;
class CScene;

class CBackground : public CObject
{
private:
	CTexture*	m_pCurBgTex;
	Vec2		m_vLT;
	Vec2		m_vScale;		// 백그라운드 이미지 크기

public:
	void update();
	void render(HDC _dc);

	CLONE(CBackground);

public:
	void SetBackgroundTex(CTexture* _pTex) { m_pCurBgTex = _pTex; }

	Vec2 GetImageSize() { return m_vScale; }

public:
	CBackground(Vec2 _vLT, Vec2 _vScale);
	~CBackground();
};

