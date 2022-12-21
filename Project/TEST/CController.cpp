#include "pch.h"
#include "CController.h"

CController::CController()
	:
	IComponent<CController>{COMPONENT_TYPE::CONTROLLER} {}

CController::~CController() = default;
