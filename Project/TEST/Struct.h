#pragma once
enum class DIR
{
	UP,
	RIGHT,
	LEFT,
	DOWN
};

struct Vec2
{
public:
	float x;
	float y;

public:
	Vec2()
		:
		x{0},
		y{0} { }

	Vec2(const float _x, const float _y)
		:
		x{_x},
		y{_y} { }

	Vec2(const long _x, const long _y)
		:
		x{static_cast<float>(_x)},
		y{static_cast<float>(_y)} { }

	Vec2(const int _x, const int _y)
		:
		x{static_cast<float>(_x)},
		y{static_cast<float>(_y)} { }

	Vec2(const POINT& _point)
		:
		x{static_cast<float>(_point.x)},
		y{static_cast<float>(_point.y)} { }

public:
	float GetDistance(const Vec2& _other) const { return sqrtf(powf(x - _other.x, 2.f) + powf(y - _other.y, 2.f)); }
	float GetLength() const { return sqrtf(x * x + y * y); }

	Vec2& Normalize()
	{
		const float fDist = sqrt(powf(x, 2.f) + powf(y, 2.f));
		x /= fDist;
		y /= fDist;

		return *this;
	}

	float Dot(Vec2 _vOther) const { return x * _vOther.x + y * +_vOther.y; }

public:
	Vec2 operator +(const float _f) const { return {x + _f, y + _f}; }
	Vec2 operator +(const Vec2& _other) const { return {x + _other.x, y + _other.y}; }

	Vec2 operator -(const float _f) const { return {x - _f, y - _f}; }
	Vec2 operator -(const Vec2& _other) const { return {x - _other.x, y - _other.y}; }

	Vec2 operator *(const float _f) const { return {x * _f, y * _f}; }
	Vec2 operator *(const Vec2& _other) const { return {x * _other.x, y * _other.y}; }

	Vec2 operator /(const float _f) const
	{
		assert(_f);
		return {x / _f, y / _f};
	}

	Vec2 operator /(const Vec2& _other) const
	{
		assert(_other.x);
		assert(_other.y);
		return {x / _other.x, y / _other.y};
	}

	void operator +=(const float _f)
	{
		x += _f;
		y += _f;
	}

	void operator +=(const Vec2& _other)
	{
		x += _other.x;
		y += _other.y;
	}

	void operator -=(const Vec2& _other)
	{
		x -= _other.x;
		y -= _other.y;
	}

	void operator /=(const float _f)
	{
		assert(_f);
		x /= _f;
		y /= _f;
	}

	void operator =(const POINT& _point)
	{
		x = static_cast<float>(_point.x);
		y = static_cast<float>(_point.y);
	}

	bool operator ==(const Vec2& _other) const
	{
		return x == _other.x && y == _other.y;
	}

	bool operator!=(const Vec2& _other) const
	{
		return !(_other == (*this));
	}

	bool operator ==(DIR _type) const
	{
		return (_type == DIR::LEFT && x == -1 && y == 0) ||
				(_type == DIR::RIGHT && x == 1 && y == 0) ||
				(_type == DIR::UP && x == 0 && y == -1) ||
				(_type == DIR::DOWN && x == 0 && y == 1);
	}

	bool operator!=(DIR _type) const
	{
		return !((*this) == _type);
	}
};
