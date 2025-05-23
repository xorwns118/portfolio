#pragma once
#include "CRes.h"

class CTexture : public CRes
{
private:
	HDC			m_dc;
	HBITMAP		m_hBit;
	BITMAP		m_bitInfo;

public:
	void Load(const wstring& _strFilePath);
	void Create(UINT _iWidth, UINT _iHeight);

	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HDC GetDC() { return m_dc; }

private:
	CTexture();
	virtual ~CTexture();

	friend class CResMgr;
	// 아무나 동적할당 할 수 없게 ResMgr 와 friend 선언 후 관리
};