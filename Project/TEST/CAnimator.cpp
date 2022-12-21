#include "pch.h"
#include "CAnimator.h"

#include "CAnimation.h"
#include "CCore.h"
#include "CPathManager.h"
#include "CResourceManager.h"

CAnimator::CAnimator()
	:
	IComponent<CAnimator>{COMPONENT_TYPE::ANIMATOR},
	m_pCurrentAnimation{nullptr},
	m_needRepeat{false} {}

CAnimator::CAnimator(const CAnimator& _other)
	:
	IComponent<CAnimator>{_other},
	m_pCurrentAnimation{nullptr},
	m_needRepeat{false}
{
	std::map<std::wstring, CAnimation*>::iterator iter = m_mapAnimations.begin();
	while (iter != m_mapAnimations.end())
	{
		CAnimation* pCloneAnimation = iter->second->Clone();
		pCloneAnimation->SetAnimator(this);
		m_mapAnimations.insert({iter->first, pCloneAnimation});
		++iter;
	}
}

CAnimator::~CAnimator()
{
	std::map<std::wstring, CAnimation*>::iterator iter = m_mapAnimations.begin();
	while (iter != m_mapAnimations.end())
	{
		delete iter->second;
		++iter;
	}
	m_mapAnimations.clear();
}

void CAnimator::Update()
{
	m_pCurrentAnimation->Update();

	if (true == m_needRepeat && true == m_pCurrentAnimation->IsFinished())
	{
		m_pCurrentAnimation->Reset();
	}
}

void CAnimator::Render(HDC _dc)
{
	m_pCurrentAnimation->Render(_dc);
}

void CAnimator::Play(const std::wstring& _name, bool _needRepeat)
{
	m_pCurrentAnimation = FindAnimation(_name);
	assert(m_pCurrentAnimation);

	m_needRepeat = _needRepeat;
}

void CAnimator::LoadAnimation(const std::wstring& _filePath, const std::wstring& _xmlPath)
{
	const std::wstring contentPath = CCore::GetInstance().GetPathManager().GetContentPath();
	//read XML Info
	const std::wstring xmlPath = contentPath + _xmlPath;
	std::string        path    = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(xmlPath);

	tinyxml2::XMLDocument    doc{};
	const tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	assert(error == tinyxml2::XML_SUCCESS && "ANIM ATLAS LOAD FAILED");

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("TextureAtlas");
	assert(pRoot);

	AnimAtlasInfo     atlasInfo{};
	const std::string fileName = pRoot->Attribute("imagePath");
	atlasInfo.atlasName        = std::wstring{fileName.begin(), fileName.end()};
	atlasInfo.width            = atoi(pRoot->Attribute("width"));
	atlasInfo.height           = atoi(pRoot->Attribute("height"));
	const int R                = atoi(pRoot->Attribute("R"));
	const int G                = atoi(pRoot->Attribute("G"));
	const int B                = atoi(pRoot->Attribute("B"));
	//const int A = atoi(pRoot->Attribute("A"));
	atlasInfo.backGroundColor = RGB(R, G, B);
	atlasInfo.pAtlasTexture   = CCore::GetInstance().GetResourceManager().LoadTexture(atlasInfo.atlasName, _filePath);

	tinyxml2::XMLNode* pCurrent = pRoot->FirstChild();
	while (true)
	{
		CAnimation* pAnimation = new CAnimation{};
		pAnimation->SetAnimator(this);
		pAnimation->SetAtlasInfo(atlasInfo);

		std::string currentName = pCurrent->ToElement()->Attribute("n");

		FrameInfo animInfo{};
		animInfo.leftTop.x    = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("x")));
		animInfo.leftTop.y    = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("y")));
		animInfo.size.x       = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("w")));
		animInfo.size.y       = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("h")));
		animInfo.centerPos.x  = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("centerX")));
		animInfo.centerPos.y  = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("centerY")));
		animInfo.duration     = static_cast<float>(atof(pCurrent->ToElement()->Attribute("duration")));
		std::string isFlipped = pCurrent->ToElement()->Attribute("isFlipped");
		if (isFlipped == "True")
		{
			animInfo.isFlipped = true;
		}
		else
		{
			animInfo.isFlipped = false;
		}

		pAnimation->SetAddFrame(animInfo);

		tinyxml2::XMLNode* pNext = pCurrent->NextSibling();

		//read frames info from xml
		while (pNext)
		{
			std::string nextName = pNext->ToElement()->Attribute("n");

			if (nextName == currentName)
			{
				animInfo             = {};
				animInfo.leftTop.x   = static_cast<float>(atoi(pNext->ToElement()->Attribute("x")));
				animInfo.leftTop.y   = static_cast<float>(atoi(pNext->ToElement()->Attribute("y")));
				animInfo.size.x      = static_cast<float>(atoi(pNext->ToElement()->Attribute("w")));
				animInfo.size.y      = static_cast<float>(atoi(pNext->ToElement()->Attribute("h")));
				animInfo.centerPos.x = static_cast<float>(atoi(pNext->ToElement()->Attribute("centerX")));
				animInfo.centerPos.y = static_cast<float>(atoi(pNext->ToElement()->Attribute("centerY")));
				animInfo.duration    = static_cast<float>(atof(pNext->ToElement()->Attribute("duration")));
				isFlipped            = pNext->ToElement()->Attribute("isFlipped");
				if (isFlipped == "True")
				{
					animInfo.isFlipped = true;
				}
				else
				{
					animInfo.isFlipped = false;
				}

				pAnimation->SetAddFrame(animInfo);
				pNext       = pNext->NextSibling();
				currentName = nextName;
			}
			else
			{
				pCurrent = pNext;
				break;
			}
		}
		const std::wstring name = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(currentName);
		pAnimation->SetName(name);
		m_mapAnimations.insert({pAnimation->GetName(), pAnimation});
		if (nullptr == pNext)
		{
			break;
		}
	}
}

CAnimation* CAnimator::FindAnimation(const std::wstring& _animationName)
{
	std::map<std::wstring, CAnimation*>::iterator iter = m_mapAnimations.find(_animationName);
	if (iter != m_mapAnimations.end())
	{
		return iter->second;
	}
	return nullptr;
}

void CAnimator::SetAnimGroup(const std::wstring& _groupName, const AnimGroup& _group)
{
	std::map<std::wstring, AnimGroup>::iterator iter = m_mapAnimGroups.find(_groupName);
	assert(iter == m_mapAnimGroups.end());

	m_mapAnimGroups.insert({_groupName, _group});
}

void CAnimator::PlayGroup(const Vec2& _dir, const std::wstring& _groupName, bool _repeat)
{
	const std::map<std::wstring, AnimGroup>::iterator iter = m_mapAnimGroups.find(_groupName);
	assert(iter != m_mapAnimGroups.end());

	if (-1 == _dir.x)
	{
		Play(iter->second.left, _repeat);
	}
	if (1 == _dir.x)
	{
		Play(iter->second.right, _repeat);
	}
	if (-1 == _dir.y)
	{
		Play(iter->second.up, _repeat);
	}
	if (1 == _dir.y)
	{
		Play(iter->second.down, _repeat);
	}
}
