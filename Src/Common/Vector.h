#pragma once
#include <DxLib.h>
#include <functional>

struct IntVector3;
struct IntVector2;

struct IntVector3
{
	union 
	{
		struct { int x, y, z; };
		struct { int u, v, w; };
		struct { int r, g, b; };
		int data[3];
	};
	IntVector3(void) : x(0), y(0), z(0) {}
	IntVector3(int vX, int vY, int vZ) : x(vX), y(vY), z(vZ) {}
	IntVector3(VECTOR _value) : x(static_cast<int>(_value.x)), y(static_cast<int>(_value.y)), z(static_cast<int>(_value.z)) {}

	//演算
	IntVector3 operator+(const IntVector3& _value)const;
	void operator+=(const IntVector3& _value);
	IntVector3 operator-(const IntVector3& _value)const;
	void operator-=(const IntVector3& _value);
	IntVector3 operator*(const int _value)const;
	void operator*=(const int _value);
	IntVector3 operator/(const int _value)const;
	void operator/=(const int _value);
	VECTOR operator*(const VECTOR& _value)const;
	//比較
	bool operator==(const IntVector3& _value)const;
	bool operator<(const IntVector3& _value)const;
	bool operator<=(const IntVector3& _value)const;
	bool operator>(const IntVector3& _value)const;
	bool operator>=(const IntVector3& _value)const;
	//変換
	VECTOR ToVECTOR(void)const;

};


struct IntVector3Hash
{
	size_t operator()(const IntVector3& v) const
	{
		return
			std::hash<int>()(v.x) ^
			(std::hash<int>()(v.y) << 1) ^
			(std::hash<int>()(v.z) << 2);
	}
};
	
template<>
struct std::hash<IntVector3>
{
	size_t operator()(const IntVector3& v) const
	{
		return
			std::hash<int>{}(v.x) ^
			(std::hash<int>{}(v.y) << 1) ^
			(std::hash<int>{}(v.z) << 2);
	}
};

struct FloatVector2
{
	union
	{
		struct { float x, y; };
		struct { float u, v; };
		float data[2];
	};

	FloatVector2() : x(0.0f), y(0.0f) {}
	FloatVector2(float vX, float vY) : x(vX), y(vY) {}
	FloatVector2(const IntVector2& _value);

	// 演算
	FloatVector2 operator+(const FloatVector2& _value) const;
	void operator+=(const FloatVector2& _value);
	FloatVector2 operator-(const FloatVector2& _value) const;
	void operator-=(const FloatVector2& _value);
	FloatVector2 operator*(float _value) const;
	void operator*=(float _value);
	FloatVector2 operator/(float _value) const;
	void operator/=(float _value);
	FloatVector2 operator*(const FloatVector2& _value) const;
	void operator*=(const FloatVector2& _value);
	FloatVector2 operator/(const FloatVector2& _value) const;
	void operator/=(const FloatVector2& _value);

	// 比較
	bool operator==(const FloatVector2& _value) const;
	bool operator!=(const FloatVector2& _value) const;
};

struct IntVector2
{
	union
	{
		struct { int x, y; };
		struct { int u, v; };
		int data[2];
	};

	IntVector2() : x(0), y(0) {}
	IntVector2(int vX, int vY) : x(vX), y(vY) {}
	IntVector2(const FloatVector2& _value);

	// 演算
	IntVector2 operator+(const IntVector2& _value) const;
	void operator+=(const IntVector2& _value);
	IntVector2 operator-(const IntVector2& _value) const;
	void operator-=(const IntVector2& _value);
	IntVector2 operator*(int _value) const;
	void operator*=(int _value);
	IntVector2 operator/(int _value) const;
	void operator/=(int _value);
	IntVector2 operator*(const IntVector2& _value) const;
	void operator*=(const IntVector2& _value);
	IntVector2 operator/(const IntVector2& _value) const;
	void operator/=(const IntVector2& _value);
	// 比較
	bool operator==(const IntVector2& _value) const;
	bool operator!=(const IntVector2& _value) const;
};

//static constexpr IntVector3 ZERO_INT_VECTOR3 = {0, 0, 0};