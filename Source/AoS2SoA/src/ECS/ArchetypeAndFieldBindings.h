#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/ArchecomponentHandle.h"

namespace ECS
{
	struct CArchecomponentAndField
	{
		Niflect::CNiflectType* m_soaArchecomponentType;
		Niflect::CNiflectType* m_aosFieldOwnerType;
		Niflect::OffsetType m_aosFieldOffset;
	};

	class CArchecomponentAndFieldBindings
	{
		typedef CArchecomponentAndFieldBindings CThis;
	public:
		template <typename TField, typename TType>
		CThis& AddByFieldAddr(TField TType::* fieldAddr)
		{
			auto soaArchetypeSpecified = Niflect::StaticGetType<TField::TArchetype>();
			auto aosFieldOwnerType = Niflect::StaticGetType<TType>();
			auto aosOffsetSpecified = Niflect::GetFieldOffset<TField, TType>(fieldAddr);
			Niflect::OffsetType aosFieldOffset = Niflect::OFFSET_NONE;
			for (auto& it : aosFieldOwnerType->GetFields())
			{
				if (it.GetOffset() == aosOffsetSpecified)
				{
					aosFieldOffset = it.GetOffset();
					break;
				}
			}
			m_vec.push_back({ soaArchetypeSpecified, aosFieldOwnerType, aosFieldOffset });


			//auto soaArchetypeSpecified = Niflect::StaticGetType<TField::TArchetype>();
			//auto aosFieldOwnerType = Niflect::StaticGetType<TType>();
			//auto aosOffsetSpecified = Niflect::GetFieldOffset<TField, TType>(fieldAddr);
			//Niflect::OffsetType aosFieldOffset = Niflect::OFFSET_NONE;
			//for (auto& it : archecompFieldSpecified.m_aosFieldOwnerType->GetFields())
			//{
			//	if (it.GetOffset() == archecompFieldSpecified.m_aosFieldOffset)
			//	{
			//		aosFieldOffset = it.GetOffset();
			//		break;
			//	}
			//}
			//m_vec.push_back({ archecompFieldSpecified.m_soaArchecomponentType, archecompFieldSpecified.m_aosFieldOwnerType, aosFieldOffset });


			return *this;
		}
		void Add2(const CArchecomponentAndField& archecompFieldSpecified)
		{
			m_vec.push_back(archecompFieldSpecified);
		}
		Niflect::TArray<CArchecomponentAndField> m_vec;
	};
}