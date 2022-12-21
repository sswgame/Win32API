#include "pch.h"
#include "CStageManager.h"

#include "CCore.h"
#include "CEventManager.h"
#include "CStage.h"
#include "CStage_Chicken.h"
#include "CStage_Cow.h"
#include "CStage_Farm.h"
#include "CStage_Garage.h"
#include "CStage_House.h"
#include "CStage_Mountain.h"
#include "CStage_Shop.h"
#include "CStage_Street.h"
#include "IntroStage.h"

CStageManager::CStageManager()
	:
	m_stageType{STAGE_TYPE::END},
	m_pCurrent{nullptr},
	m_pNext{nullptr} {}

CStageManager::~CStageManager()
{
	for (const CStage* pStage : m_vecStages)
	{
		delete pStage;
		pStage = nullptr;
	}
	m_vecStages.clear();
}

void CStageManager::Init()
{
	m_vecStages.resize(STAGE_TYPE_COUNT);

	//각 스테이지 만들기
	m_vecStages[static_cast<int>(STAGE_TYPE::INTRO)]    = new IntroStage{};
	m_vecStages[static_cast<int>(STAGE_TYPE::HOUSE)]    = new CStage_House{};
	m_vecStages[static_cast<int>(STAGE_TYPE::FARM)]     = new CStage_Farm{};
	m_vecStages[static_cast<int>(STAGE_TYPE::GARAGE)]   = new CStage_Garage{};
	m_vecStages[static_cast<int>(STAGE_TYPE::COW)]      = new CStage_Cow{};
	m_vecStages[static_cast<int>(STAGE_TYPE::CHICKEN)]  = new CStage_Chicken{};
	m_vecStages[static_cast<int>(STAGE_TYPE::STREET)]   = new CStage_Street{};
	m_vecStages[static_cast<int>(STAGE_TYPE::SHOP)]     = new CStage_Shop{};
	m_vecStages[static_cast<int>(STAGE_TYPE::MOUNTAIN)] = new CStage_Mountain{};

	ChangeStage(STAGE_TYPE::INTRO);
}

void CStageManager::Update() const
{
	m_pCurrent->Update();
}

void CStageManager::Render(HDC _dc) const
{
	m_pCurrent->Render(_dc);
}

void CStageManager::ChangeStage(STAGE_TYPE _stage)
{
	m_stageType = _stage;
	assert(m_pCurrent != m_vecStages[static_cast<UINT>(_stage)]);

	if (nullptr != m_pCurrent)
	{
		assert(m_pNext != m_pCurrent);
		m_pNext = m_vecStages[static_cast<UINT>(_stage)];
		m_pCurrent->Exit();

		CCore::GetInstance().GetEventManager().UnRegisterCallback<CStage, DeletionEvent>(
			m_pCurrent, &CStage::DeleteObject);
		CCore::GetInstance().GetEventManager().UnRegisterCallback<CStage, CreationEvent>(
			m_pCurrent, &CStage::CreateObject);
		CCore::GetInstance().GetEventManager().UnRegisterCallback<CStage, ChangeStageEvent>(
			m_pCurrent, &CStage::ChangeStage);
		CCore::GetInstance().GetEventManager().UnRegisterCallback<CStage, ChangeStateEvent>(
			m_pCurrent, &CStage::ChangeState);
	}

	m_pCurrent = m_vecStages[static_cast<UINT>(_stage)];
	m_pCurrent->Enter();

	CCore::GetInstance().GetEventManager().RegisterCallback<CStage, DeletionEvent>(m_pCurrent, &CStage::DeleteObject);
	CCore::GetInstance().GetEventManager().RegisterCallback<CStage, CreationEvent>(m_pCurrent, &CStage::CreateObject);
	CCore::GetInstance().GetEventManager().RegisterCallback<CStage, ChangeStageEvent>(m_pCurrent, &CStage::ChangeStage);
	CCore::GetInstance().GetEventManager().RegisterCallback<CStage, ChangeStateEvent>(m_pCurrent, &CStage::ChangeState);

	m_pNext = nullptr;
}
