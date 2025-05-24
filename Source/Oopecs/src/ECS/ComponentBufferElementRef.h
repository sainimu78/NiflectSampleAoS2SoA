#pragma once
#include "Niflect/NiflectType.h"

namespace ECS
{
	class CArchetypeBuffer;

	class CComponentBufferElementRef
	{
	//public:
	//	CComponentBufferElementRef()
	//		: m_type(NULL)
	//		, m_offset(Niflect::OFFSET_NONE)
	//		, m_owner(NULL)
	//		, m_archetypeIdx(INDEX_NONE)
	//	{
	//	}

	protected:
		CComponentBufferElementRef(Niflect::CNiflectType* type)
			: m_type(type)
			, m_bufferOffset(Niflect::OFFSET_NONE)
			, m_buffer(NULL)
			, m_archetypeIdx(INDEX_NONE)
		{
		}

	public:
		void InitRef(CArchetypeBuffer* buffer, uint32 archetypeIdx, const Niflect::OffsetType& bufferOffset)
		{
			m_buffer = buffer;
			m_archetypeIdx = archetypeIdx;
			m_bufferOffset = bufferOffset;
		}
		void* GetAddr() const;

	public:
		Niflect::CNiflectType* m_type;

	private:
		Niflect::OffsetType m_bufferOffset;
		CArchetypeBuffer* m_buffer;
		uint32 m_archetypeIdx;
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
		T& GetMutable()
		{
			return *static_cast<T*>(this->GetAddr());
		}
		const T& Get() const
		{
			return *static_cast<const T*>(this->GetAddr());
		}
	};
}