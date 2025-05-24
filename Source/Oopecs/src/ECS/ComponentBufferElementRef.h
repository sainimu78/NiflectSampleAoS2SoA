#pragma once
#include "Niflect/NiflectType.h"

namespace ECS
{
	class CComponentBufferElementRef
	{
	public:
		CComponentBufferElementRef()
			: m_type(NULL)
			, m_offset(Niflect::OFFSET_NONE)
		{
		}
		CComponentBufferElementRef(Niflect::CNiflectType* type)
			: m_type(type)
			, m_offset(Niflect::OFFSET_NONE)
		{
		}
		void InitOffset(const Niflect::OffsetType& offset)
		{
			m_offset = offset;
		}

	public:
		Niflect::CNiflectType* m_type;

	private:
		Niflect::OffsetType m_offset;
	};

	template <typename T>
	class TComponentBufferElementRef : public CComponentBufferElementRef
	{
		typedef CComponentBufferElementRef inherited;
	public:
		using RefType = T;
	public:
		TComponentBufferElementRef()
			: inherited(Niflect::StaticGetType<T>())
		{
		}
	};
}