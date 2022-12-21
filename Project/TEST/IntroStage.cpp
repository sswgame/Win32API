#include "pch.h"
#include "IntroStage.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CColliderManager.h"
#include "CEventManager.h"
#include "CKeyManager.h"
#include "CPlayer.h"
#include "CRayCast.h"
#include "CResourceManager.h"
#include "CSound.h"
#include "CStageManager.h"
#include "CTimer.h"

IntroStage::IntroStage()
	:
	m_scrollSpeed{-200.f},
	m_delay{0},
	m_height{0} { }

IntroStage::~IntroStage() = default;

void IntroStage::Update()
{
	CStage::Update();
	if (KEY_TAP(KEY::ENTER))
	{
		CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(ChangeStageEvent{STAGE_TYPE::HOUSE});
	}
	for (BGInfo& bg : m_vecBackGround)
	{
		// Update the x offset
		bg.offset.x += m_scrollSpeed * DS;
		// If this is completely off the screen, reset offset to
		// the right of the last bg texture
		if (bg.offset.x < -bg.pTexture->GetSize().x)
		{
			bg.offset.x = (m_vecBackGround.size() - 1) * bg.pTexture->GetSize().x - 1;
		}
	}
}

void IntroStage::Render(HDC _dc)
{
	for (BGInfo& bg : m_vecBackGround)
	{
		BitBlt(_dc,
		       static_cast<int>(bg.offset.x),
		       static_cast<int>(bg.offset.y),
		       static_cast<int>(bg.pTexture->GetSize().x),
		       static_cast<int>(bg.pTexture->GetSize().y),
		       bg.pTexture->GetTextureDC(),
		       0,
		       0,
		       SRCCOPY);
	}
	GdiTransparentBlt(_dc,
	                  23,
	                  100,
	                  static_cast<int>(m_vecFront[0]->GetSize().x),
	                  static_cast<int>(m_vecFront[0]->GetSize().y),
	                  m_vecFront[0]->GetTextureDC(),
	                  0,
	                  0,
	                  static_cast<int>(m_vecFront[0]->GetSize().x),
	                  static_cast<int>(m_vecFront[0]->GetSize().y),
	                  RGB(255, 0, 255));

	GdiTransparentBlt(_dc,
	                  220,
	                  472,
	                  static_cast<int>(m_vecFront[1]->GetSize().x),
	                  static_cast<int>(m_vecFront[1]->GetSize().y) / 4,
	                  m_vecFront[1]->GetTextureDC(),
	                  0,
	                  m_height,
	                  static_cast<int>(m_vecFront[1]->GetSize().x),
	                  static_cast<int>(m_vecFront[1]->GetSize().y) / 4,
	                  RGB(255, 0, 255));

	m_delay += DS;
	if (m_delay > 0.2f)
	{
		m_height += 44;
		if (m_height >= m_vecFront[1]->GetSize().y)
		{
			m_height = 0;
		}
		m_delay = 0;
	}
}

void IntroStage::Init()
{
	LoadInfo();
	SetTexture();
}

void IntroStage::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);
	Init();
}

void IntroStage::Exit()
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"IngameSound");
	pSound->PlayToBGM(true);
}

void IntroStage::LoadInfo()
{
	CCore::GetInstance().GetResourceManager().LoadSound(L"IngameSound", LR"(sound\IngameSound.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"water", LR"(sound\water.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"axe", LR"(sound\axe.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"sickle", LR"(sound\sickle.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"hammer", LR"(sound\hammer.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"jump", LR"(sound\jump.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"plot", LR"(sound\plot.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"putdown", LR"(sound\putdown.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"running", LR"(sound\running.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"seeding", LR"(sound\seeding.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"take", LR"(sound\take.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"wrong", LR"(sound\wrong input.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"opening", LR"(sound\opening.wav)");
	CCore::GetInstance().GetResourceManager().LoadSound(L"text", LR"(sound\text.wav)");

	CCore::GetInstance().GetResourceManager().LoadSpriteInfos(LR"(animation\tools.xml)");
	CCore::GetInstance().GetResourceManager().LoadSpriteInfos(LR"(animation\extra.xml)");
	CCore::GetInstance().GetResourceManager().LoadSpriteInfos(LR"(animation\crops.xml)");

	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"opening");
	pSound->PlayToBGM(true);


	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::PLAYER, OBJECT_TYPE::TELEPORTER, true);
	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::PLAYER, OBJECT_TYPE::ITEM, true);
	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::PLAYER, OBJECT_TYPE::ANIMAL, true);

	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::RAYCAST, OBJECT_TYPE::ITEM, true);
	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::RAYCAST, OBJECT_TYPE::ANIMAL, true);
	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::RAYCAST, OBJECT_TYPE::INVOKER, true);

	CCore::GetInstance().GetColliderManager().SetCollisionBetween(OBJECT_TYPE::ANIMAL, OBJECT_TYPE::ITEM, true);

	const std::vector<CStage*>& vecStages   = CCore::GetInstance().GetStageManager().GetAllStage();
	CPlayer*                    pGameObject = new CPlayer{};
	s_pPlayer                               = pGameObject;
	AddGameObject(pGameObject, OBJECT_TYPE::PLAYER);

	CRayCast* pRay = new CRayCast{};
	pRay->SetName(L"CAST");
	pRay->SetOwner(pGameObject);
	pRay->GetComponent<CCollider>()->Enable(false);
	AddGameObject(pRay, OBJECT_TYPE::RAYCAST);

	pGameObject->SetRay(pRay);

	for (int i = 0; i < vecStages.size(); ++i)
	{
		if (vecStages[i] != this)
		{
			vecStages[i]->Init();
		}
	}
	MoveObjectsTo(STAGE_TYPE::HOUSE, OBJECT_TYPE::PLAYER, pGameObject);
	MoveObjectsTo(STAGE_TYPE::HOUSE, OBJECT_TYPE::RAYCAST, pRay);
}

void IntroStage::SetTexture()
{
	CTexture* pTexture = CCore::GetInstance().GetResourceManager().LoadTexture(
		L"BG_CENTER", LR"(texture\bg_center.bmp)");
	m_vecFront.push_back(pTexture);
	pTexture = CCore::GetInstance().GetResourceManager().LoadTexture(
		L"BG_START", LR"(texture\bg_start.bmp)");
	m_vecFront.push_back(pTexture);

	const std::vector<CTexture*> vecBgTextures =
	{
		CCore::GetInstance().GetResourceManager().LoadTexture(L"BG1", LR"(texture\bg1.bmp)"),
		CCore::GetInstance().GetResourceManager().LoadTexture(L"BG2", LR"(texture\bg2.bmp)"),
		CCore::GetInstance().GetResourceManager().LoadTexture(L"BG3", LR"(texture\bg3.bmp)")
	};

	int count = 0;
	for (int i = 0; i < vecBgTextures.size(); ++i)
	{
		BGInfo temp{};
		temp.pTexture = vecBgTextures[i];
		temp.offset.x = count * vecBgTextures[i]->GetSize().x;
		temp.offset.y = 0;
		m_vecBackGround.push_back(std::move(temp));
		++count;
	}
}
