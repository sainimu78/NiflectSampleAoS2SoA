#pragma once
#include "Niflect/Base/SharedPtr.h"
#include "Niflect/NiflectType.h"

namespace OOP
{
	class CNode;

	class CComponent
	{
	public:
		CComponent()
			: m_owner(NULL)
			, m_type(NULL)
		{
		}
		void Init(CNode* owner, Niflect::CNiflectType* type)
		{
			m_owner = owner;
			m_type = type;
		}
		Niflect::CNiflectType* GetType() const
		{
			return m_type;
		}

	public:
		virtual void StartFrame(float deltaTime) {}
		virtual void Tick(float deltaTime) = 0;

	public:
		CNode* m_owner;
		Niflect::CNiflectType* m_type;
	};
	using CSharedComponent = Niflect::TSharedPtr<CComponent>;
}