#pragma once

class CObject;
class CTexture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT	eEffect;	  // 카메라 효과
	float		fDuration;  // 효과 최대 진행 시간
	float		fCurTime;	  // 카메라 효과 현재 진행된 시간
};

class CCamera
{
	SINGLE(CCamera);
private:
	Vec2				m_vLookAt;			// 카메라가 보는 위치
	Vec2				m_vCurLookAt;		// 이전 위치와 현재 위치 보정 위치
	Vec2				m_vPrevLookAt;		// 카메라가 보던 이전 프레임 위치

	Vec2				m_vMapScale;		// 카메라가 볼 수 있는 맵 사이즈

	CObject*			m_pTargetObj;		// 카메라 타겟 오브젝트
	Vec2				m_vDiff;			// 해상도 중심 위치와, 카메라 LookAt 간의 차이값

	float				m_fTime;			// 타겟을 따라가는데 걸리는 시간
	float				m_fSpeed;			// 타겟을 따라가는 속도
	float				m_fAccTime;			// 누적 시간

	list<tCamEffect>	m_listCamEffect;
	CTexture*			m_pVeilTex;			// 카메라 가림막 텍스쳐 (검은색으로)

public:
	void SetLookAt(Vec2 _vLook)
	{ 
		m_vLookAt = _vLook;
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;
	}
	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	void SetMapScale(Vec2 _vScale) { m_vMapScale = _vScale; }
	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; }
	Vec2 GetRenderDiff(Vec2 _vObjPos, float _f) { return _vObjPos - (m_vDiff * _f); }
	Vec2 GetMapScale() { return m_vMapScale; }

	void FadeIn(float _fDuration)
	{
		tCamEffect ef = {};

		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (_fDuration == 0)
		{
			assert(nullptr);
		}
	}

	void FadeOut(float _fDuration)
	{
		tCamEffect ef = {};

		ef.eEffect = CAM_EFFECT::FADE_OUT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (_fDuration == 0)
		{
			assert(nullptr);
		}
	}

public:
	void init();
	void update();
	void render(HDC _dc);


private:
	void ClampLookAt();
	void CalDiff();

};

