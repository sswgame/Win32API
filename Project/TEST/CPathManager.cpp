#include "pch.h"
#include "CPathManager.h"

namespace
{
	const std::wstring FOLDER_PATH = LR"(\Release\content\)";
}

void CPathManager::Init()
{
	//C++17 version
	/*
	namespace fs = std::filesystem;
	m_path = fs::current_path().parent_path() / L"Release" / L"content" / L"";
	*/

#ifdef _WIN32
	m_path.resize(256);
	GetCurrentDirectory(static_cast<DWORD>(m_path.length()), &m_path[0]);

	size_t pos = m_path.find_last_of(LR"(\)");
	assert(pos != m_path.npos);

	m_path.replace(m_path.begin() + pos, m_path.end(), L"\0");
	m_path += FOLDER_PATH;
#endif
}
