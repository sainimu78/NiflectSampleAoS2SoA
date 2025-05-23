#pragma once

class CVector3
{
public:
	CVector3()
		: m_x(0.0f)
		, m_y(0.0f)
		, m_z(0.0f)
	{
	}
	CVector3(float scalar)
		: m_x(scalar)
		, m_y(scalar)
		, m_z(scalar)
	{
	}
	CVector3(float x, float y, float z)
		: m_x(x)
		, m_y(y)
		, m_z(z)
	{
	}
	CVector3 operator*(float scalar) const
	{
		return CVector3(m_x * scalar, m_y * scalar, m_z * scalar);
	}
	CVector3& operator+=(const CVector3& vector)
	{
		m_x += vector.m_x;
		m_y += vector.m_y;
		m_z += vector.m_z;
		return *this;
	}
	float m_x;
	float m_y;
	float m_z;
};