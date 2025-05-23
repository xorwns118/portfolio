#pragma once
#include "CObject.h"

struct tElementInfo
{
	Vec2	m_vElementLT;	// 수풀 Tex 좌상단 위치
	Vec2	m_vElementRB;	// 수풀 Tex 우하단 위치
	Vec2	m_vElementPos;	// 수풀 위치
};

class CTexture;

class CElement : public CObject
{
private:
	CTexture*	m_pTex;
	Vec2		m_vLT;
	Vec2		m_vSize;

	bool		m_bIsPlay;

public:
	virtual void update();
	virtual void render(HDC _dc);

private:
	CLONE(CElement);

public:
	CElement(CTexture* _pTex, Vec2 _vLT, Vec2 _vRB);
	~CElement();
};

