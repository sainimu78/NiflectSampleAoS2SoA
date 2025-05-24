#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/ComponentBufferElementRef.h"

namespace ECS
{
	class CAosSoaBinder
	{
	public:
		//CAosToSoaConverter()
		//{
		//}
		void Convert(Niflect::InstanceType* base, Niflect::TArray<Niflect::CField*> vecFieldInvolved)
		{
			for (auto& it : vecFieldInvolved)
			{
				auto fieldBase =  Niflect::GetOffsetAddr(base, it->GetOffset());
				auto instance = static_cast<CComponentBufferElementRef*>(fieldBase);
				ASSERT(instance->m_type == it->GetType());
				m_vecBufElemRef.push_back(instance);
			}
		}
		template <typename TField, typename TType>
		void Add(Niflect::InstanceType* base, TField TType::* fieldAddr)
		{
			auto type = Niflect::StaticGetType<TType>();
			auto offsetSpecified = Niflect::GetFieldOffset<TField, TType>(fieldAddr);
			for (auto& it : type->GetFields())
			{
				if (it.GetOffset() == offsetSpecified)
				{
					auto fieldBase = Niflect::GetOffsetAddr(base, it.GetOffset());
					auto instance = static_cast<CComponentBufferElementRef*>(fieldBase);
					ASSERT(instance->m_type == Niflect::StaticGetType<TField::RefType>());
					m_vecBufElemRef.push_back(instance);
					break;
				}
			}
		}
		Niflect::TArray<CComponentBufferElementRef*> m_vecBufElemRef;
		//Niflect::CNiflectType* m_type;
		//Niflect::TArray<Niflect::CField*> m_vecFieldInvolved;
		//Niflect::InstanceType* m_base;
	};
	//class CAosToSoaDesc
	//{
	//public:
	//	CAosToSoaDesc()
	//		: m_type(NULL)
	//		, m_base(NULL)
	//	{
	//	}
	//	Niflect::CNiflectType* m_type;
	//	Niflect::TArray<Niflect::CField*> m_vecFieldInvolved;
	//	Niflect::InstanceType* m_base;
	//};
}