#pragma once
#include "CPlayer.h"

class CIce_Kirby : public CPlayer
{
public:
	PLAYER_STATE m_eState;

private:
	virtual void Attack();

public:
	CIce_Kirby();
	~CIce_Kirby();
};

