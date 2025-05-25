#pragma once
#include "Niflect/NiflectType.h"

namespace ECS
{
	class CSoaEntityBuffer;

	class CComponentHandle
	{
		friend class CSoaEntityBuffer;
	protected:
		CComponentHandle(Niflect::CNiflectType* debugArchetype)
			: m_entityOffset(Niflect::OFFSET_NONE)
			, m_buffer(NULL)
			, m_componentIdx(INDEX_NONE)
#ifdef NODEBUG
#else
			, m_debugArchetype(debugArchetype)
#endif
		{
		}
		void* GetAddr() const;

	private:
		void InitHandle(CSoaEntityBuffer* buffer, uint32 componentIdx, const Niflect::OffsetType& entityOffset)
		{
			m_buffer = buffer;
			m_componentIdx = componentIdx;
			m_entityOffset = entityOffset;
		}

#ifdef NODEBUG
#else
	public:
		Niflect::CNiflectType* m_debugArchetype;
#endif

	private:
		Niflect::OffsetType m_entityOffset;
		CSoaEntityBuffer* m_buffer;
		uint32 m_componentIdx;
	};

	template <typename T>
	class TComponentHandle : public CComponentHandle
	{
		typedef CComponentHandle inherited;
	public:
		using TArchetype = T;
	public:
		TComponentHandle()
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