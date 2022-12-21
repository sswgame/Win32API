#pragma once
#include "Cstage.h"
#include "CTexture.h"

struct BGInfo
{
	CTexture* pTexture;
	Vec2      offset;
};
class IntroStage : public CStage
{
public:
	IntroStage();
	virtual ~IntroStage();

public:
	void Update() override;
	void Render(HDC _dc) override;
	void Init() override;
	void Enter() override;
	void Exit() override;

private:
	void LoadInfo();
	void SetTexture();

private:
	std::vector<BGInfo>    m_vecBackGround;
	std::vector<CTexture*> m_vecFront;
	float                  m_scrollSpeed;
	float                  m_delay;
	int                    m_height;

};
