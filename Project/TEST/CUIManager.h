#pragma once
class CUI;

class CUIManager
{
public:
	CUIManager();
	~CUIManager();

public:
	void Init();
	void Render(HDC _dc);
	void SetUI(CUI* _pUI);
	CUI* GetUI() const { return m_pCurrentUI; }
	bool IsShowedAll() const { return m_isShowedAll; }
private:
	void Clear();
private:
	CUI*                            m_pCurrentUI;
	HFONT                           m_font;
	std::queue<std::deque<wchar_t>> m_queueLine;
	std::vector<std::wstring>       m_vecTexts;
	float                           m_delay;
	int                             m_length;
	int                             m_height;
	std::wstring                    m_text;
	bool                            m_isShowedAll;
};
