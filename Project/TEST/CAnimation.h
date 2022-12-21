#pragma once

class CTexture;
class CAnimator;

struct FrameInfo
{
	Vec2  leftTop;
	Vec2  size;
	Vec2  centerPos;
	float duration;
	bool  isFlipped;
};

struct AnimAtlasInfo
{
	std::wstring atlasName;
	int          width;
	int          height;
	COLORREF     backGroundColor;
	CTexture*    pAtlasTexture;
};

class CAnimation
{
	friend class CAnimator;
public:
	CAnimation();
	~CAnimation();

public:
	void Update();
	void Render(HDC _dc);
	void Reset();

	CAnimation* Clone() const { return new CAnimation{*this}; }

public:
	const std::wstring& GetName() const { return m_name; }
	bool                IsFinished() const { return m_isFinished; }
	Vec2                GetSize() const { return m_vecFrames[m_currentIndex].size; }
	Vec2                GetCenterPos() const { return m_vecFrames[m_currentIndex].centerPos; }
	size_t              FrameCount() const { return m_vecFrames.size(); }
	int                 GetCurrentIndex() const { return m_currentIndex; }
	float               GetDuration() const { return m_vecFrames[m_currentIndex].duration; }
	float               GetTotalDuration() const;

private:
	void SetAnimator(CAnimator* _pAnimator) { m_pAnimator = _pAnimator; }
	void SetAtlasInfo(const AnimAtlasInfo& _atlasInfo) { m_atlasInfo = _atlasInfo; }
	void SetAddFrame(const FrameInfo& _frameInfo) { m_vecFrames.push_back(_frameInfo); }
	void SetName(const std::wstring& _name) { m_name = _name; }

private:
	CAnimator*             m_pAnimator;
	std::wstring           m_name;
	AnimAtlasInfo          m_atlasInfo;
	std::vector<FrameInfo> m_vecFrames;
	int                    m_currentIndex;
	float                  m_elapsedTime;
	bool                   m_isFinished;
};
