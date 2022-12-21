#include "pch.h"
#include "CNpc.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "Name.h"

CNpc::CNpc()
{
	CAnimator* pAnimator = AddComponent<CAnimator>();
	pAnimator->LoadAnimation(LR"(texture\npc.bmp)", LR"(animation\npc.xml)");
	pAnimator->Play(NPC::ANIMATIONS[NPC::IDLE_LEFT_0], false);

	GetComponent<CTransform>()->SetScale(Vec2{40.f, 40.f});

	CCollider* pCollider = AddComponent<CCollider>();
	pCollider->SetOffsetPos(Vec2{0.f, 0.f});
	pCollider->SetScale(GetComponent<CTransform>()->GetScale());
}
CNpc::~CNpc() = default;
