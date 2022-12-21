#pragma once
#include "CAnimation.h"
#include "CComponent.h"

struct AnimGroup
{
	std::wstring left;
	std::wstring right;
	std::wstring up;
	std::wstring down;
};

class CAnimation;

class CAnimator : public IComponent<CAnimator>
{
public:
	CAnimator();
	CAnimator(const CAnimator& _other);
	CAnimator& operator=(const CAnimator&) = delete;
	virtual ~CAnimator();

public:
	void        Update() override;
	void        Render(HDC _dc) override;
public:
	void Play(const std::wstring& _name, bool _needRepeat = true);

	void        LoadAnimation(const std::wstring& _filePath, const std::wstring& _xmlPath);
	CAnimation* FindAnimation(const std::wstring& _animationName);
	CAnimation& GetCurrent() const { return *m_pCurrentAnimation; }

	void SetAnimGroup(const std::wstring& _groupName, const AnimGroup& _group);
	void PlayGroup(const Vec2& _dir, const std::wstring& _groupName, bool _repeat = true);
private:
	CComponent* Clone() override { return new CAnimator{ *this }; };

private:
	std::map<std::wstring, CAnimation*> m_mapAnimations;
	CAnimation*                         m_pCurrentAnimation;
	bool                                m_needRepeat;

	std::map<std::wstring, AnimGroup> m_mapAnimGroups;
};
