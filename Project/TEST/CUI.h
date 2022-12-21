#pragma once
#include "CGameObject.h"

enum class SELECTION_TYPE
{
	NONE,
	YES_NO,
	COW_CHICKEN,
	SEED,
};

enum class MOVE_TYPE
{
	LEFT,
	RIGHT,
};
enum class ANSWER_TYPE
{
	NONE,
	OKAY,
	CANCEL,
	TURNIP,
	CORN,
	TOMATO,
	POTATO,
	GRASS,
};

class CUI : public CGameObject
{
public:
	CUI();
	virtual ~CUI();
public:
	OBJECT_TYPE    GetType() override { return OBJECT_TYPE::UI; };
	CGameObject*   Clone() override { return new CUI{*this}; };
	std::wstring   GetText() const { return m_text; }
	void           SetText(const std::wstring& _text);
	void           Mark(int _type);
	ANSWER_TYPE    GetMark() const { return m_answer; }
	bool           IsYes() const { return m_answer == ANSWER_TYPE::OKAY; }
	void           SetShowSelection(bool _enabled) { m_show = _enabled; }
	void           SetSelectionType(SELECTION_TYPE _type);
	SELECTION_TYPE GetSelectionType() const { return m_selection; }
	void           DrawSelection(HDC _dc);
	bool           IsSetShowSelection() const { return m_show; }
	void           MoveTo(MOVE_TYPE _type);
private:
	std::wstring   m_text;
	ANSWER_TYPE    m_answer;
	SELECTION_TYPE m_selection;
	bool           m_show;
	int            m_range;
};
