#pragma once
#include "CScene.h"

class CStage_02 : public CScene
{
private:


public:
	virtual void update();
	virtual void render(HDC _dc);

	virtual void Enter();
	virtual void Exit();

public:

public:
	CStage_02();
	~CStage_02();
};

