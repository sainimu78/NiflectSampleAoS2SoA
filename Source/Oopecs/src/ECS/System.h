#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/Node.h"
#include "ECS/Archecomponent.h"
#include "ECS/ArchetypeAndFieldBindings.h"

namespace ECS
{
	using CEntitiesArchecomponentHandles = Niflect::TArray<Niflect::TArray<CArchecomponentHandle*> >;

	class CAosEntitiesSoaArchecomponentsBinder
	{
	public:
		void Bind(const Niflect::TArray<CSharedNode>& vecNode, const CArchecomponentAndFieldBindings& archecompFieldBindings)
		{
			auto soaArchecompsCount = archecompFieldBindings.m_vec.size();
			for (auto& it : archecompFieldBindings.m_vec)
				m_vecArchecomponentType.push_back(it.m_soaArchecomponentType);
			auto entitiesCount = vecNode.size();
			m_vecAosEntityArchecomponentHandles.resize(entitiesCount);
			for (uint32 idxEntity = 0; idxEntity < entitiesCount; ++idxEntity)
			{
				auto entity = vecNode[idxEntity].Get();
				auto& vecArchecomponentHandle = m_vecAosEntityArchecomponentHandles[idxEntity];
				for (uint32 archecompIdx = 0; archecompIdx < soaArchecompsCount; ++archecompIdx)
				{
					auto& binding = archecompFieldBindings.m_vec[archecompIdx];
					for (auto& it2 : entity->m_vecComponent)
					{
						if (it2->GetType() == binding.m_aosFieldOwnerType)
						{
							auto fieldBase = Niflect::GetOffsetAddr(it2.Get(), binding.m_aosFieldOffset);
							auto instance = static_cast<CArchecomponentHandle*>(fieldBase);
							vecArchecomponentHandle.push_back(instance);
							break;
						}
					}
				}
				ASSERT(vecArchecomponentHandle.size() == soaArchecompsCount);
			}
		}

		Niflect::TArray<Niflect::CNiflectType*> m_vecArchecomponentType;
		CEntitiesArchecomponentHandles m_vecAosEntityArchecomponentHandles;
	};

	class CSoaEntitiesBuffer
	{
	public:
		void Init(const Niflect::TArray<Niflect::CNiflectType*>& vecArchecomponentType)
		{
			ASSERT(m_vecArchecomponentBuffer.size() == 0);
			m_vecArchecomponentBuffer.resize(vecArchecomponentType.size());
			for (uint32 idx0 = 0; idx0 < vecArchecomponentType.size(); ++idx0)
			{
				auto& buffer = m_vecArchecomponentBuffer[idx0];
				buffer.Init(vecArchecomponentType[idx0]);
			}
		}
		void AllocAndBind(const CEntitiesArchecomponentHandles& vecAosEntityArchecomponentHandles)
		{
			auto soaArchecompsCount = m_vecArchecomponentBuffer.size();

			Niflect::TArray<uint32> vecComponentIdxToOldEntitiesCount;
			for (auto& it : m_vecArchecomponentBuffer)
				vecComponentIdxToOldEntitiesCount.push_back(it.GetEntitiesCount());

			uint32 countToAdd = static_cast<uint32>(vecAosEntityArchecomponentHandles.size());
			for (uint32 idx0 = 0; idx0 < soaArchecompsCount; ++idx0)
			{
				auto& buffer = m_vecArchecomponentBuffer[idx0];
				auto oldCount = buffer.GetEntitiesCount();
				buffer.Alloc(countToAdd);
			}
			for (uint32 idxEntity = 0; idxEntity < vecAosEntityArchecomponentHandles.size(); ++idxEntity)
			{
				auto& vecSoaComponentHandle = vecAosEntityArchecomponentHandles[idxEntity];
				ASSERT(soaArchecompsCount == vecSoaComponentHandle.size());
				for (uint32 archecompIdx = 0; archecompIdx < soaArchecompsCount; ++archecompIdx)
				{
					auto& it1 = vecSoaComponentHandle[archecompIdx];
					auto& buffer = m_vecArchecomponentBuffer[archecompIdx];
					auto& offset = vecComponentIdxToOldEntitiesCount[archecompIdx];
					it1->InitHandle(this, archecompIdx, offset + buffer.m_archetype->GetTypeSize() * idxEntity);
				}
			}
		}
		template <typename T>
		T* GetMutableArchecomponents(uint32 archecompIdx)
		{
			auto& buffer = m_vecArchecomponentBuffer[archecompIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_archetype == Niflect::StaticGetType<T>());
			return reinterpret_cast<T*>(base);
		}
		template <typename T>
		const T* GetArchecomponents(uint32 archecompIdx) const
		{
			auto& buffer = m_vecArchecomponentBuffer[archecompIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_archetype == Niflect::StaticGetType<T>());
			return reinterpret_cast<const T*>(base);
		}

	public:
		Niflect::TArray<CArchecomponentBuffer> m_vecArchecomponentBuffer;
	};

	class CSystem
	{
	public:
		CSystem()
			: m_entitiesCount(0)
		{
		}
		void InitEntitiesBuffer(const CAosEntitiesSoaArchecomponentsBinder& binder)
		{
			m_entitiesBuffer.Init(binder.m_vecArchecomponentType);
			auto countToAdd = static_cast<uint32>(binder.m_vecAosEntityArchecomponentHandles.size());
			m_entitiesBuffer.AllocAndBind(binder.m_vecAosEntityArchecomponentHandles);
			m_entitiesCount += countToAdd;
		}

		CSoaEntitiesBuffer m_entitiesBuffer;
		uint32 m_entitiesCount;
	};
}