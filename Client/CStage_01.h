#pragma once
#include "CScene.h"

class CTexture;

class CStage_01 : public CScene
{
private:
	float			m_fForceRadius;
	float			m_fCurRadius;
	float			m_fForce;

public:
	virtual void update();
	virtual void render(HDC _dc);

	virtual void Enter();
	virtual void Exit();

public:

public:
	CStage_01();
	~CStage_01();
};

