#pragma once
#include "Niflect/Base/SharedPtr.h"
#include "Niflect/NiflectType.h"
#include "ECS/AosSoaBinder.h"

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
		//virtual void StartFrame(float deltaTime) {}
		//virtual void Tick(float deltaTime) = 0;

	public:
		CNode* m_owner;
		Niflect::CNiflectType* m_type;
	};
	using CSharedComponent = Niflect::TSharedPtr<CComponent>;

	class CComponentBuffer
	{
	public:
		void Alloc(uint32 entityStartIdx, uint32 entitiesCount, const CAosSoaBinder& binder)
		{
			m_vecArchetypeIdxToComponentBuffer.resize(binder.m_vecBufElemRef.size());
			for (uint32 idx0 = 0; idx0 < m_vecArchetypeIdxToComponentBuffer.size(); ++idx0)
			{
				auto& ref = binder.m_vecBufElemRef[idx0];
				auto& it0 = ref->m_type;
				auto& size = it0->GetTypeSize();
				//auto& alignment = it0->GetTypeAlignment();
				//auto bytesCount = size + (alignment - (size % alignment));
				auto& buffer = m_vecArchetypeIdxToComponentBuffer[idx0];
				auto oldSize = buffer.m_data.size();
				ref->InitOffset(oldSize);
				buffer.m_type = it0;
				buffer.m_size = size;
				auto newSize = size * (entityStartIdx + entitiesCount);
				buffer.m_data.resize(newSize);
				auto& ctorInfo = it0->m_vecConstructorInfo[0];
				ASSERT(ctorInfo.m_vecInput.size() == 0);
				ctorInfo.m_Func(&buffer.m_data[oldSize], NULL);
			}
		}
		void Free(uint32 entityStartIdx, uint32 entitiesCount)
		{
			for (uint32 idx0 = 0; idx0 < m_vecArchetypeIdxToComponentBuffer.size(); ++idx0)
			{
				auto& buffer = m_vecArchetypeIdxToComponentBuffer[idx0];
				auto remainingSize = buffer.m_data.size() - buffer.m_size * (entityStartIdx + entitiesCount);
				buffer.m_data.resize(remainingSize);
			}
		}
		template <typename T>
		T* GetMutableComponentBase(uint32 archetypeIdx)
		{
			auto& buffer = m_vecArchetypeIdxToComponentBuffer[archetypeIdx];
			auto base = buffer.m_data.data();
			ASSERT(buffer.m_type == Niflect::StaticGetType<T>());
			return reinterpret_cast<T*>(base);
		}
		template <typename T>
		const T* GetComponentBase(uint32 archetypeIdx) const
		{
			auto& buffer = m_vecArchetypeIdxToComponentBuffer[archetypeIdx];
			auto base = buffer.m_data.data();
			ASSERT(buffer.m_type == Niflect::StaticGetType<T>());
			return reinterpret_cast<const T*>(base);
		}

	private:
		class CBuffer
		{
		public:
			CBuffer()
				: m_size(0)
				, m_type(NULL)
			{
			}
			uint32 m_size;
			Niflect::CNiflectType* m_type;
			Niflect::TArray<uint8> m_data;
		};

	public:
		Niflect::TArray<CBuffer> m_vecArchetypeIdxToComponentBuffer;
	};
}