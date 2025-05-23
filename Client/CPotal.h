#pragma once
#include "CObject.h"

class CTexture;

class CPotal : public CObject
{
private:
	CTexture*	m_pTex;
	Vec2		m_vLT;
	Vec2		m_vScale;

	SCENE_TYPE	m_eNextScene;	// 다음 씬

public:
	virtual void start();
	virtual void update();
	virtual void render(HDC _dc);

	virtual void OnCollision(CCollider* _pOther);

public:
	SCENE_TYPE GetNextScene() { return m_eNextScene; }
	
	CLONE(CPotal);

public:
	CPotal(UINT _iNextScene);
	~CPotal();
};

