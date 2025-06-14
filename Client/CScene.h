#pragma once
#include "CElement.h"

struct tMapInfo
{
	Vec2					m_vFgLT;	// 맵 Tex 좌상단 위치
	Vec2					m_vFgRB;	// 맵 Tex 우하단 위치
	Vec2					m_vFgPos;	// 맵 Pos
};

struct tPlayerInfo
{
	// 씬 이동시 이전씬의 플레이어 정보 저장
};

// 전방선언 => 컴파일의 속도에 영향을 주지 않기 위함, 오브젝트 쪽 코드가 변경될 시 매번 검사(대응)할 필요가 없음
class CObject;

class CScene
{
private:
	vector<CObject*>		m_arrObj[(UINT)GROUP_TYPE::END];// 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	wstring					m_strName;						// Scene 이름

	UINT					m_iTileX;	// 타일 가로 개수	
	UINT					m_iTileY;	// 타일 세로 개수

	wstring					m_strMapKey;	// 맵 Tex 키
	wstring					m_strMapPath;	// 맵 Tex 경로

	vector<tMapInfo>		m_vecMapInfo;
	vector<tElementInfo>	m_vecElementInfo;	// 맵 외 오브젝트 요소 정보

	CObject*				m_pPlayer;	// Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	void SetMapPath(const wstring& _strName) { m_strMapPath = _strName; }
	const wstring& GetMapPath() { return m_strMapPath; }

	void SetMapKey(const wstring& _strName) { m_strMapKey = _strName; }
	const wstring& GetMapKey() { return m_strMapKey; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	vector<tMapInfo> GetMapInfo() { return m_vecMapInfo; }
	vector<tElementInfo> GetElementPos() { return m_vecElementInfo; }

	CObject* GetPlayer() { return m_pPlayer; }

	virtual void start();
	virtual void update(); // 오브젝트 업데이트
	virtual void finalupdate(); // 그 외 작업을 마무리해주는 업데이트
	virtual void render(HDC _dc);

	void render_tile(HDC _dc);

	// 순수 가상함수, 부모 클래스에 해당하는 씬은 직접 객체생성이 불가능해짐 => 추상 클래스
	virtual void Enter() = 0; // 해당 Scene 에 진입 시 호출
	virtual void Exit() = 0;  // 해당 Scene 을 탈출 시 호출

public:
	// 클래스는 헤더에 구현하면 함수 호출 비용이 드는것이 아닌 복사 붙여넣기 효과
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();
	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);

	void DeleteElementInfo(int _iIdx);

	void CreateMap(Vec2 _vLT, Vec2 _vRB, Vec2 _vPos, UINT _iCount);
	void CreateElement(Vec2 _vLT, Vec2 _vRB, Vec2 _vPos);
	void CreateGroundRectangle(Vec2 _vPos, Vec2 _vScale);
	void CreateGroundTriangle(Vec2 _vPos, const Vec2 _vStartPos, const Vec2 _vMiddlePos, const Vec2 _vEndPos);
	void CreatePotal(UINT _iNextScene, Vec2 _vPos);
	void LoadMap(const wstring& _strRelativePath);

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

public:
	CScene();
	virtual ~CScene();
};

