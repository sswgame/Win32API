#pragma once
class CStage;

class CStageManager
{
public:
	CStageManager();
	~CStageManager();

public:
	void Init();
	void Update() const;
	void Render(HDC _dc) const;

	void ChangeStage(STAGE_TYPE _stage);
public:
	CStage&               GetCurrent() const { return *m_pCurrent; }
	CStage&               GetNext() const { return *m_pNext; }
	STAGE_TYPE            GetStageType() const { return m_stageType; }
	CStage&               GetStage(STAGE_TYPE _type) const { return *m_vecStages[static_cast<UINT>(_type)]; }
	std::vector<CStage*>& GetAllStage() { return m_vecStages; }
private:
	STAGE_TYPE m_stageType;
	std::vector<CStage*> m_vecStages;
	CStage*              m_pCurrent;
	CStage*              m_pNext;
};
