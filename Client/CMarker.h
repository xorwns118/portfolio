#pragma once

class CObject;
class CTexture;

class CMarker
{
private:
	static UINT g_iNextID;
	CObject*	m_pOwner;

	CTexture*	m_pVeilTex;

	UINT		m_iID;

public:
	void render(HDC _dc);

public:
	CMarker(Vec2 _vScale);
	~CMarker();

	friend class CObject;
};

