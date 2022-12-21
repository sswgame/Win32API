#pragma once
class CPathManager
{
public:
	CPathManager()  = default;
	~CPathManager() = default;

public:
	void Init();

public:
	const std::wstring& GetContentPath() const { return m_path; }
private:
	std::wstring m_path;
};
