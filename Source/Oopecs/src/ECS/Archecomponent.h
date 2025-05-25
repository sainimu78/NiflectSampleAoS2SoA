#pragma once
#include "Niflect/NiflectType.h"

namespace ECS
{
	class CArchecomponentBuffer
	{
	public:
		CArchecomponentBuffer()
			: m_archetype(NULL)
			, m_entitiesCount(0)
		{
		}
		~CArchecomponentBuffer()
		{
			this->DestroyAll();
		}
		void Init(Niflect::CNiflectType* archetype)
		{
			m_archetype = archetype;
		}
		void Alloc(uint32 countToAdd)
		{
			auto& size = m_archetype->GetTypeSize();
			auto newCount = m_entitiesCount + countToAdd;
			auto newSize = size * newCount;
			m_bytes.resize(newSize);
			auto& ctorInfo = m_archetype->m_vecConstructorInfo[0];
			ASSERT(ctorInfo.m_vecInput.size() == 0);
			for (uint32 idx = m_entitiesCount; idx < newCount; ++idx)
				ctorInfo.m_Func(this->GetData(size * idx), NULL);
			m_entitiesCount = newCount;
		}
		void Free(uint32 countToDelete)
		{
			auto newCount = m_entitiesCount - countToDelete;
			auto& size = m_archetype->GetTypeSize();
			auto newSize = size * newCount;
			this->Destroy(newCount, m_entitiesCount);
			m_bytes.resize(newSize);
			m_entitiesCount = newCount;
		}
		uint32 GetEntitiesCount() const
		{
			return m_entitiesCount;
		}

	private:
		void Destroy(uint32 start, uint32 end)
		{
			auto& size = m_archetype->GetTypeSize();
			auto& DtorFunc = m_archetype->m_InvokeDestructorFunc;
			for (uint32 idx = start; idx < end; ++idx)
				DtorFunc(this->GetData(size * idx));
		}
		void DestroyAll()
		{
			this->Destroy(0, m_entitiesCount);
		}
		void* GetData(uint32 offset)
		{
			return static_cast<void*>(&m_bytes[offset]);
		}

	public:
		Niflect::CNiflectType* m_archetype;
		Niflect::TArray<uint8> m_bytes;
		uint32 m_entitiesCount;
	};
}