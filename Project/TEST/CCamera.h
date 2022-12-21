#pragma once

class CTexture;

enum class CAMERA_EFFECT_TYPE
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct CameraEffect
{
	CAMERA_EFFECT_TYPE effect;
	float              elapsedTime;
	float              effectTime;
};

class CCamera
{
public:
	CCamera();
	~CCamera();

public:
	void Init();
	void Update();
	void Render(HDC _dc);

	void FadeIn(float _effectTime);
	void FadeOut(float _effectTime);

public:
	bool IsEffectFinished() const { return m_isFinished; }
public:
	void SetLookAt(const Vec2& _pos) { m_lookAt = _pos; }
	Vec2 GetRenderPos(const Vec2& _realPosition) const { return {_realPosition - m_diff}; }
	Vec2 GetRealPos(const Vec2& _renderPos) const { return {_renderPos + m_diff}; }
	Vec2 GetLookAtPos() const { return m_lookAt; }
private:
	Vec2                     m_centerPos;
	Vec2                     m_lookAt;
	Vec2                     m_diff;
	CTexture*                m_pVeilTexture;
	std::queue<CameraEffect> m_queueEffect;

	bool                     m_isFinished;
};
