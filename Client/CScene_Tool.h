#pragma once
#include "CScene.h"

class CUI;

class CScene_Tool : public CScene
{
private:
	CUI*		m_pUI;

	CObject*	m_pDragObj;		// 드래그 대상 obj
	wstring		m_strObjName;	// 대상 Obj 이름
	int			m_iTargetIdx;	// 삭제 대상 obj idx
	bool		m_bMouseOn;
	bool		m_bLbtnDown;

	Vec2		m_vDragStart;	// 드래그 시작지점

public:
	virtual void update();
	virtual void render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	void MouseOnCheck();
	void MouseOn();
	void MouseLbtnDown();
	void MouseLbtnUp();
	void MouseLbtnClicked();

public:
	void SetTileIdx();

	void SaveTileData();
	void SaveTile(const wstring& _strFilePath);

	void LoadTileData();

	void SetDelTargetIdx(int _i) { m_iTargetIdx = _i; }
	int GetDelTargetIdx() { return m_iTargetIdx; }
	CObject* GetTargetObj() { return m_pDragObj; }

	void SaveMapData();
	void SaveMap(const wstring& _strFilePath);

	void LoadMapData();
	
public:
	CScene_Tool();
	~CScene_Tool();
};

