#pragma once
#include "CComponent.h"
#include "CResourceManager.h"

struct SpriteInfo;
class CTexture;

class CSpriteComponent : public IComponent<CSpriteComponent>
{
public:
	CSpriteComponent();
	virtual ~CSpriteComponent();

public:
	void Render(HDC _dc) override;

public:
	void SetTexture(const std::wstring& _name, const std::wstring& _path, bool _hasXMLInfo = true);
	void SetEnabled(bool _enable) { m_isEnabled = _enable; }

	Vec2        GetScale() const { return m_pInfo->scale; }
	void        SetInfo(SpriteInfo* _pInfo) { m_pInfo = _pInfo; }
	SpriteInfo* GetInfo() const { return m_pInfo; }
	void        EnableFixed(bool _enabled) { m_isFixed = _enabled; }
private:
	CComponent* Clone() override { return new CSpriteComponent{ *this }; };

private:
	CTexture*   m_pTexture;
	SpriteInfo* m_pInfo;
	bool        m_isEnabled;
	bool        m_isFixed;
};
