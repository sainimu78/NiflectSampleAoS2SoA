#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/ComponentBufferElementRef.h"

namespace ECS
{
	struct CFieldRefTypeAndIndex
	{
		Niflect::CNiflectType* m_fieldRef;
		Niflect::CNiflectType* m_ownerType;
		Niflect::OffsetType m_fieldOffset;
	};

	class CArchetypeFieldRefArray
	{
		typedef CArchetypeFieldRefArray CThis;
	public:
		template <typename TField, typename TType>
		CThis& Add(TField TType::* fieldAddr)
		{
			auto type = Niflect::StaticGetType<TType>();
			auto fieldRef = Niflect::StaticGetType<TField::RefType>();
			auto offsetSpecified = Niflect::GetFieldOffset<TField, TType>(fieldAddr);
			Niflect::OffsetType fieldOffset = Niflect::OFFSET_NONE;
			for (auto& it : type->GetFields())
			{
				if (it.GetOffset() == offsetSpecified)
				{
					fieldOffset = it.GetOffset();
					break;
				}
			}
			m_vec.push_back({ fieldRef, type, fieldOffset });
			return *this;
		}
		Niflect::TArray<CFieldRefTypeAndIndex> m_vec;
	};
}