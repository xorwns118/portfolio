#pragma once
#include "CObject.h"

class CTexture;

class CForeground : public CObject
{
private:
	CTexture*	m_pCurFgTex;	// 현재 맵 이미지
	Vec2		m_vLT;			// 이미지 좌상단 좌표
	Vec2		m_vScale;		// 맵 크기
	Vec2		m_vPos;			// 맵 위치

public:
	void update();
	void render(HDC _dc);

	CLONE(CForeground);

public:
	void SetForegroundTex(CTexture* _pTex) { m_pCurFgTex = _pTex; }

	Vec2 GetImageSize() { return m_vScale; }

public:
	CForeground(Vec2 _vLT, Vec2 _vScale, Vec2 _vPos);
	~CForeground();
};

