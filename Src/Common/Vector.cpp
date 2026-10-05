#include "Vector.h"

IntVector3 IntVector3::operator+(const IntVector3& _value)const
{
	return { x + _value.x , y + _value.y, z + _value.z };
}

void IntVector3::operator+=(const IntVector3& _value)
{
	x += _value.x;
	y += _value.y;
	z += _value.z;
}

IntVector3 IntVector3::operator-(const IntVector3& _value)const
{
	return { x - _value.x , y - _value.y, z - _value.z };
}

void IntVector3::operator-=(const IntVector3& _value)
{
	x -= _value.x;
	y -= _value.y;
	z -= _value.z;
}

IntVector3 IntVector3::operator*(const int _value)const
{
	return { x * _value , y * _value, z * _value };
}

void IntVector3::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
	z *= _value;
}

IntVector3 IntVector3::operator/(const int _value)const
{
	return { x / _value , y / _value, z / _value };
}

void IntVector3::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
	z /= _value;
}

VECTOR IntVector3::operator*(const VECTOR& _value) const
{
	return VECTOR(static_cast<float>(x) * _value.x, static_cast<float>(y) * _value.y, static_cast<float>(z) * _value.z);
}

bool IntVector3::operator==(const IntVector3& _value) const
{
 	bool diffX = x == _value.x;
	bool diffY = y == _value.y;
	bool diffZ = z == _value.z;

	return diffX && diffY && diffZ;
}

bool IntVector3::operator<(const IntVector3& _value) const
{
	if (x != _value.x)
	{
		return x < _value.x;
	}

	if (y != _value.y)
	{
		return y < _value.y;
	}

	return z < _value.z;
}

bool IntVector3::operator<=(const IntVector3& _value) const
{
	bool diffX = x <= _value.x;
	bool diffY = y <= _value.y;
	bool diffZ = z <= _value.z;

	return diffX && diffY && diffZ;
}

bool IntVector3::operator>(const IntVector3& _value) const
{
	if (x != _value.x)
	{
		return x > _value.x;
	}

	if (y != _value.y)
	{
		return y > _value.y;
	}

	return z > _value.z;
}

bool IntVector3::operator>=(const IntVector3& _value) const
{
	bool diffX = x >= _value.x;
	bool diffY = y >= _value.y;
	bool diffZ = z >= _value.z;

	return diffX && diffY && diffZ;
}

VECTOR IntVector3::ToVECTOR(void) const
{
	return VECTOR(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

VECTOR IntVectorToVECTOR(const IntVector3& _iv)
{
	float vx = static_cast<float>(_iv.x);
	float vy = static_cast<float>(_iv.y);
	float vz = static_cast<float>(_iv.z);

	return VGet(vx, vy, vz);
}

static IntVector3 VECTORToIntVector(VECTOR _v)
{
	IntVector3 intVec;

	intVec.x = static_cast<int>(_v.x);
	intVec.y = static_cast<int>(_v.y);
	intVec.z = static_cast<int>(_v.z);

	return intVec;
}

FloatVector2::FloatVector2(const IntVector2& _value)
{
	x = static_cast<float>(_value.x);
	y = static_cast<float>(_value.y);
}

FloatVector2 FloatVector2::operator+(const FloatVector2& _value) const
{
	return FloatVector2(x + _value.x, y + _value.y);
}

void FloatVector2::operator+=(const FloatVector2& _value)
{
	x += _value.x;
	y += _value.y;
}

FloatVector2 FloatVector2::operator-(const FloatVector2& _value) const
{
	return FloatVector2(x - _value.x, y - _value.y);
}

void FloatVector2::operator-=(const FloatVector2& _value)
{
	x -= _value.x;
	y -= _value.y;
}

FloatVector2 FloatVector2::operator*(const float _value) const
{
	return FloatVector2(x * _value, y * _value);
}

void FloatVector2::operator*=(const float _value)
{
	x *= _value;
	y *= _value;
}

FloatVector2 FloatVector2::operator/(const float _value) const
{
	return FloatVector2(x / _value, y / _value);
}

void FloatVector2::operator/=(const float _value)
{
	x /= _value;
	y /= _value;
}

FloatVector2 FloatVector2::operator*(const FloatVector2& _value) const
{
	return FloatVector2(x * _value.x, y * _value.y);
}

void FloatVector2::operator*=(const FloatVector2& _value)
{
	x *= _value.x;
	y *= _value.y;
}

FloatVector2 FloatVector2::operator/(const FloatVector2& _value) const
{
	return FloatVector2(x / _value.x, y / _value.y);
}

void FloatVector2::operator/=(const FloatVector2& _value)
{
	x /= _value.x;
	y /= _value.y;
}

IntVector2::IntVector2(const FloatVector2& _value)
{
	x = static_cast<int>(_value.x);
	y = static_cast<int>(_value.y);
}

IntVector2 IntVector2::operator+(const IntVector2& _value) const
{
	return IntVector2(x + _value.x, y + _value.y);
}

void IntVector2::operator+=(const IntVector2& _value)
{
	x += _value.x;
	y += _value.y;
}

IntVector2 IntVector2::operator-(const IntVector2& _value) const
{
	return IntVector2(x - _value.x, y - _value.y);
}

void IntVector2::operator-=(const IntVector2& _value)
{
	x -= _value.x;
	y -= _value.y;
}

IntVector2 IntVector2::operator*(const int _value) const
{
	return IntVector2(x * _value, y * _value);
}

void IntVector2::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
}

IntVector2 IntVector2::operator/(const int _value) const
{
	return IntVector2(x / _value, y / _value);
}

void IntVector2::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
}

IntVector2 IntVector2::operator*(const IntVector2& _value) const
{
	return IntVector2(x * _value.x, y * _value.y);
}

void IntVector2::operator*=(const IntVector2& _value)
{
	x *= _value.x;
	y *= _value.y;
}

IntVector2 IntVector2::operator/(const IntVector2& _value) const
{
	return IntVector2(x / _value.x, y / _value.y);
}

void IntVector2::operator/=(const IntVector2& _value)
{
	x /= _value.x;
	y /= _value.y;
}