#pragma once
#include "Niflect/NiflectType.h"

namespace ECS
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
		CNode* m_owner;
		Niflect::CNiflectType* m_type;
	};
	using CSharedComponent = Niflect::TSharedPtr<CComponent>;

	class CComponentNata : public Niflect::CNata
	{
		typedef CComponentNata CThis;
	public:
		template <typename TSystemType>
		CThis& ConfirmCompatible()
		{
			auto type = Niflect::StaticGetType<TSystemType>();
			auto ret = m_setCompatibleSysType.insert(type);
			ASSERT(ret.second);
			return *this;
		}

	public:
		Niflect::TSet<Niflect::CNiflectType*> m_setCompatibleSysType;
	};
}