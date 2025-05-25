#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/Node.h"
#include "ECS/ArchetypeAndFieldBindings.h"

namespace ECS
{
	using CEntitiesComponentHandles = Niflect::TArray<Niflect::TArray<CComponentHandle*> >;

	class CAosSoaBinder
	{
	public:
		void Bind(const Niflect::TArray<CSharedNode>& vecNode, const CArchetypeAndFieldBindings& archetypeFieldBindingArray)
		{
			auto soaComponentsCount = archetypeFieldBindingArray.m_vec.size();
			for (auto& it : archetypeFieldBindingArray.m_vec)
				m_vecSoaComponentArchetype.push_back(it.m_soaComponentArchetype);
			auto entitiesCount = vecNode.size();
			m_vecAosEntitySoaComponentHandles.resize(entitiesCount);
			for (uint32 idxEntity = 0; idxEntity < entitiesCount; ++idxEntity)
			{
				auto entity = vecNode[idxEntity].Get();
				auto& vecSoaComponentHandle = m_vecAosEntitySoaComponentHandles[idxEntity];
				for (uint32 idxComponent = 0; idxComponent < soaComponentsCount; ++idxComponent)
				{
					auto& binding = archetypeFieldBindingArray.m_vec[idxComponent];
					for (auto& it2 : entity->m_vecComponent)
					{
						if (it2->GetType() == binding.m_aosFieldOwnerType)
						{
							auto fieldBase = Niflect::GetOffsetAddr(it2.Get(), binding.m_aosFieldOffset);
							auto instance = static_cast<CComponentHandle*>(fieldBase);
							vecSoaComponentHandle.push_back(instance);
							break;
						}
					}
				}
				ASSERT(vecSoaComponentHandle.size() == soaComponentsCount);
			}
		}

		Niflect::TArray<Niflect::CNiflectType*> m_vecSoaComponentArchetype;
		CEntitiesComponentHandles m_vecAosEntitySoaComponentHandles;
	};

	class CSoaEntityBuffer
	{
	public:
		void Init(const Niflect::TArray<Niflect::CNiflectType*>& vecSoaComponentArchetype)
		{
			ASSERT(m_vecSoaComponentBuffer.size() == 0);
			m_vecSoaComponentBuffer.resize(vecSoaComponentArchetype.size());
			for (uint32 idx0 = 0; idx0 < vecSoaComponentArchetype.size(); ++idx0)
			{
				auto& buffer = m_vecSoaComponentBuffer[idx0];
				buffer.Init(vecSoaComponentArchetype[idx0]);
			}
		}
		void AllocAndBind(const CEntitiesComponentHandles& vecAosEntitySoaComponentHandles)
		{
			auto soaComponentsCount = m_vecSoaComponentBuffer.size();

			Niflect::TArray<uint32> vecComponentIdxToOldEntitiesCount;
			for (auto& it : m_vecSoaComponentBuffer)
				vecComponentIdxToOldEntitiesCount.push_back(it.GetEntitiesCount());

			uint32 countToAdd = static_cast<uint32>(vecAosEntitySoaComponentHandles.size());
			for (uint32 idx0 = 0; idx0 < soaComponentsCount; ++idx0)
			{
				auto& buffer = m_vecSoaComponentBuffer[idx0];
				auto oldCount = buffer.GetEntitiesCount();
				buffer.Alloc(countToAdd);
			}
			for (uint32 idxEntity = 0; idxEntity < vecAosEntitySoaComponentHandles.size(); ++idxEntity)
			{
				auto& vecSoaComponentHandle = vecAosEntitySoaComponentHandles[idxEntity];
				ASSERT(soaComponentsCount == vecSoaComponentHandle.size());
				for (uint32 componentIdx = 0; componentIdx < soaComponentsCount; ++componentIdx)
				{
					auto& it1 = vecSoaComponentHandle[componentIdx];
					auto& buffer = m_vecSoaComponentBuffer[componentIdx];
					auto& offset = vecComponentIdxToOldEntitiesCount[componentIdx];
					it1->InitHandle(this, componentIdx, offset + buffer.m_archetype->GetTypeSize() * idxEntity);
				}
			}
		}
		template <typename T>
		T* GetMutableComponentBase(uint32 componentIdx)
		{
			auto& buffer = m_vecSoaComponentBuffer[componentIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_archetype == Niflect::StaticGetType<T>());
			return reinterpret_cast<T*>(base);
		}
		template <typename T>
		const T* GetComponentBase(uint32 componentIdx) const
		{
			auto& buffer = m_vecSoaComponentBuffer[componentIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_archetype == Niflect::StaticGetType<T>());
			return reinterpret_cast<const T*>(base);
		}

	public:
		Niflect::TArray<CComponentBuffer> m_vecSoaComponentBuffer;
	};

	class CSystem
	{
	public:
		CSystem()
			: m_entitiesCount(0)
		{
		}
		void InitEntitiyBuffer(const CAosSoaBinder& binder)
		{
			m_entityBuffer.Init(binder.m_vecSoaComponentArchetype);
			auto countToAdd = static_cast<uint32>(binder.m_vecAosEntitySoaComponentHandles.size());
			m_entityBuffer.AllocAndBind(binder.m_vecAosEntitySoaComponentHandles);
			m_entitiesCount += countToAdd;
		}

		CSoaEntityBuffer m_entityBuffer;
		uint32 m_entitiesCount;
	};
}