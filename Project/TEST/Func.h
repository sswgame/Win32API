#pragma once
namespace Util
{
	template <typename T>
	void SafeDeleteVector(std::vector<T>& _vec)
	{
		static_assert(std::is_pointer_v<T>);

		for (const T& pData : _vec)
		{
			delete pData;
		}
		_vec.clear();
	}

	template <typename T, int Size>
	void SafeDeleteArray(T (&_arr)[Size])
	{
		for (int i = 0; i < Size; ++i)
		{
			if (nullptr != _arr[i])
			{
				delete _arr[i];
				_arr[i] = nullptr;
			}
		}
	}
}
