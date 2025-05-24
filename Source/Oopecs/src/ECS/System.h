#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/Node.h"
#include "ECS/AosSoaBinder.h"

namespace ECS
{
	using CEntityArchetypeFieldInstancesArray = Niflect::TArray<Niflect::TArray<CComponentBufferElementRef*> >;

	class CSoaLayoutEntitiesFieldInstance
	{
	public:
		void Convert(const Niflect::TArray<CSharedNode>& vecNode, const CArchetypeFieldRefArray& soaArchetypeFieldRefArray)
		{
			auto archetypesCount = soaArchetypeFieldRefArray.m_vec.size();
			for (auto& it : soaArchetypeFieldRefArray.m_vec)
				m_vecArchetype.push_back(it.m_fieldRef);
			auto entitiesCount = vecNode.size();
			m_vecEntityArcheFieldInstances.resize(entitiesCount);
			for (uint32 idxEntity = 0; idxEntity < entitiesCount; ++idxEntity)
			{
				auto entity = vecNode[idxEntity].Get();
				auto& vecArchetypeFieldInstance = m_vecEntityArcheFieldInstances[idxEntity];
				for (uint32 idxArchetype = 0; idxArchetype < archetypesCount; ++idxArchetype)
				{
					auto& fieldRefInfo = soaArchetypeFieldRefArray.m_vec[idxArchetype];
					for (auto& it2 : entity->m_vecComponent)
					{
						if (it2->GetType() == fieldRefInfo.m_ownerType)
						{
							auto fieldBase = Niflect::GetOffsetAddr(it2.Get(), fieldRefInfo.m_fieldOffset);
							auto instance = static_cast<CComponentBufferElementRef*>(fieldBase);
							vecArchetypeFieldInstance.push_back(instance);
							break;
						}
					}
				}
			}
		}

		Niflect::TArray<Niflect::CNiflectType*> m_vecArchetype;
		CEntityArchetypeFieldInstancesArray m_vecEntityArcheFieldInstances;
	};

	class CArchetypeBuffer
	{
	public:
		void Create(const Niflect::TArray<Niflect::CNiflectType*>& vecArchetype)
		{
			ASSERT(m_vecArchetypeIdxToComponentBuffer.size() == 0);
			m_vecArchetypeIdxToComponentBuffer.resize(vecArchetype.size());
			for (uint32 idx0 = 0; idx0 < vecArchetype.size(); ++idx0)
			{
				auto& buffer = m_vecArchetypeIdxToComponentBuffer[idx0];
				buffer.Init(vecArchetype[idx0]);
			}
		}
		void AllocAndBind(const CEntityArchetypeFieldInstancesArray& vecEntityArcheFieldInstances)
		{
			auto archetypesCount = m_vecArchetypeIdxToComponentBuffer.size();

			Niflect::TArray<uint32> vecArchetypeIdxToOldEntitiesCount;
			for (auto& it : m_vecArchetypeIdxToComponentBuffer)
				vecArchetypeIdxToOldEntitiesCount.push_back(it.GetEntitiesCount());

			uint32 countToAdd = static_cast<uint32>(vecEntityArcheFieldInstances.size());
			for (uint32 idx0 = 0; idx0 < archetypesCount; ++idx0)
			{
				auto& buffer = m_vecArchetypeIdxToComponentBuffer[idx0];
				auto oldCount = buffer.GetEntitiesCount();
				buffer.Alloc(countToAdd);
			}
			for (uint32 idxEntity = 0; idxEntity < vecEntityArcheFieldInstances.size(); ++idxEntity)
			{
				auto& vecArchetypeFieldInstance = vecEntityArcheFieldInstances[idxEntity];
				ASSERT(archetypesCount == vecArchetypeFieldInstance.size());
				for (uint32 idxArchetype = 0; idxArchetype < archetypesCount; ++idxArchetype)
				{
					auto& it1 = vecArchetypeFieldInstance[idxArchetype];
					auto& buffer = m_vecArchetypeIdxToComponentBuffer[idxArchetype];
					auto& offset = vecArchetypeIdxToOldEntitiesCount[idxArchetype];
					it1->InitRef(this, idxArchetype, offset + buffer.m_type->GetTypeSize() * idxEntity);
				}
			}
		}
		template <typename T>
		T* GetMutableComponentBase(uint32 archetypeIdx)
		{
			auto& buffer = m_vecArchetypeIdxToComponentBuffer[archetypeIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_type == Niflect::StaticGetType<T>());
			return reinterpret_cast<T*>(base);
		}
		template <typename T>
		const T* GetComponentBase(uint32 archetypeIdx) const
		{
			auto& buffer = m_vecArchetypeIdxToComponentBuffer[archetypeIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_type == Niflect::StaticGetType<T>());
			return reinterpret_cast<const T*>(base);
		}

	public:
		Niflect::TArray<CComponentBuffer> m_vecArchetypeIdxToComponentBuffer;
	};

	class CSystem
	{
	public:
		CSystem()
			: m_entitiesCount(0)
		{
		}
		void InitArchetypeBuffer(const CSoaLayoutEntitiesFieldInstance& binder)
		{
			m_archetypeBuffer.Create(binder.m_vecArchetype);
			auto countToAdd = static_cast<uint32>(binder.m_vecEntityArcheFieldInstances.size());
			m_archetypeBuffer.AllocAndBind(binder.m_vecEntityArcheFieldInstances);
			m_entitiesCount += countToAdd;
		}

		CArchetypeBuffer m_archetypeBuffer;
		uint32 m_entitiesCount;
	};
}