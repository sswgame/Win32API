#pragma once
class CResource
{
	friend class CResourceManager;

public:
	CResource()          = default;
	virtual ~CResource() = default;

public:
	void SetKey(const std::wstring& _key) { m_key = _key; }
	void SetPath(const std::wstring& _path) { m_relativePath = _path; }

	const std::wstring& GetKey() const { return m_key; }
	const std::wstring& GetPath() const { return m_relativePath; }

private:
	virtual bool Load(const std::wstring& _fullPath) = 0;

private:
	std::wstring m_key;
	std::wstring m_relativePath;
};
