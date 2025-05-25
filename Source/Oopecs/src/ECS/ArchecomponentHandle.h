#pragma once
#include "Niflect/NiflectType.h"

namespace ECS
{
	class CSoaEntitiesBuffer;

	class CArchecomponentHandle
	{
		friend class CSoaEntitiesBuffer;
	protected:
		CArchecomponentHandle(Niflect::CNiflectType* debugArchetype)
			: m_entityOffset(Niflect::OFFSET_NONE)
			, m_buffer(NULL)
			, m_archecompIdx(INDEX_NONE)
#ifdef NODEBUG
#else
			, m_debugArchetype(debugArchetype)
#endif
		{
		}
		void* GetAddr() const;

	private:
		void InitHandle(CSoaEntitiesBuffer* buffer, uint32 archecompIdx, const Niflect::OffsetType& entityOffset)
		{
			m_buffer = buffer;
			m_archecompIdx = archecompIdx;
			m_entityOffset = entityOffset;
		}

#ifdef NODEBUG
#else
	public:
		Niflect::CNiflectType* m_debugArchetype;
#endif

	private:
		Niflect::OffsetType m_entityOffset;
		CSoaEntitiesBuffer* m_buffer;
		uint32 m_archecompIdx;
	};

	template <typename T>
	class TArchecomponentHandle : public CArchecomponentHandle
	{
		typedef CArchecomponentHandle inherited;
	public:
		using TArchetype = T;
	public:
		TArchecomponentHandle()
			: inherited(Niflect::StaticGetType<TArchetype>())
		{
		}
		TArchetype& GetMutable()
		{
			return *static_cast<TArchetype*>(this->GetAddr());
		}
		const TArchetype& Get() const
		{
			return *static_cast<const TArchetype*>(this->GetAddr());
		}
	};
}