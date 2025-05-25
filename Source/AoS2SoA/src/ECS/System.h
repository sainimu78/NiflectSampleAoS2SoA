#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/Node.h"
#include "ECS/Archecomponent.h"
#include "ECS/ArchetypeAndFieldBindings.h"

#include "ECS/Vector3.h"//未建立 ArchecomponentHandleType 与 Archetype 的绑定, 临时 include

namespace ECS
{
	using CEntitiesArchecomponentHandles = Niflect::TArray<Niflect::TArray<CArchecomponentHandle*> >;

	class CAosEntitiesSoaArchecomponentsBinder
	{
	public:
		void BindDefault(const Niflect::TArray<CSharedNode>& vecNode, const Niflect::TArray<Niflect::CNiflectType*>& vecSystemTypeBindFor)
		{
			auto firstNodeAsBindingTemplate = vecNode[0].Get();
			Niflect::TArray<Niflect::CNiflectType*> vecAosComponentType;
			for (auto& it : firstNodeAsBindingTemplate->m_vecComponent)
				vecAosComponentType.push_back(it->GetType());
			
			auto CheckAllNodesMeetRequirementFunc = [&]()
				{
					for (uint32 idx0 = 1; idx0 < vecNode.size(); ++idx0)
					{
						auto it0 = vecNode[idx0].Get();
						for (auto& it1 : it0->m_vecComponent)
							ASSERT(std::find(vecAosComponentType.begin(), vecAosComponentType.end(), it1->GetType()) != vecAosComponentType.end());
					}
					return true;
				};
			ASSERT(CheckAllNodesMeetRequirementFunc());
			
			CArchecomponentAndFieldBindings bindings;
			for (auto& it0 : vecAosComponentType)
			{
				for (auto& it1 : it0->GetFields())
				{
					if (auto nata = it1.GetNata())
					{
						auto compNata = Niflect::CastDerivedNata<CComponentNata>(it1.GetNata());
						for (auto& it2 : vecSystemTypeBindFor)
						{
							if (compNata->m_setCompatibleSysType.find(it2) != compNata->m_setCompatibleSysType.end())
							{
								Niflect::CNiflectType* soaArchetypeSpecified = NULL;
								//begin, 
								//1. 须实现自动注册, 建议通过 NIFRIEND 的 CThis 封装
								//2. 以首个 node 为绑定模板不合适, 应考虑通过独立的 Archecomponent 排列定义作为依据
								//3. System 在使用时应能通过标识获取 ArchecomponentBuffer, 以实现可复用的 entitiesBUffer
								if (it1.GetType() == Niflect::StaticGetType<CVector3>())
									soaArchetypeSpecified = Niflect::StaticGetType<CVector3::TArchetype>();
								//end
								ASSERT(soaArchetypeSpecified != NULL);
								auto aosFieldOwnerType = it0;
								auto aosOffsetSpecified = it1.GetOffset();
								bindings.Add2({ soaArchetypeSpecified, aosFieldOwnerType, aosOffsetSpecified });
								break;
							}
						}
					}
				}
			}

			this->Bind(vecNode, bindings);
		}
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
		CSoaEntitiesBuffer()
			: m_entitiesCount(0)
		{
		}
		void InitAllocBind(const CAosEntitiesSoaArchecomponentsBinder& binder)
		{
			this->Init(binder.m_vecArchecomponentType);
			auto countToAdd = static_cast<uint32>(binder.m_vecAosEntityArchecomponentHandles.size());
			this->AllocAndBind(binder.m_vecAosEntityArchecomponentHandles);
		}
		template <typename T>
		T* GetMutableBase(uint32 archecompIdx)
		{
			auto& buffer = m_vecArchecomponentBuffer[archecompIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_archetype == Niflect::StaticGetType<T>());
			return reinterpret_cast<T*>(base);
		}
		template <typename T>
		const T* GetBase(uint32 archecompIdx) const
		{
			auto& buffer = m_vecArchecomponentBuffer[archecompIdx];
			auto base = buffer.m_bytes.data();
			ASSERT(buffer.m_archetype == Niflect::StaticGetType<T>());
			return reinterpret_cast<const T*>(base);
		}

	private:
		void Init(const Niflect::TArray<Niflect::CNiflectType*>& vecArchecomponentType)
		{
			ASSERT(m_entitiesCount == 0);
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
			m_entitiesCount += countToAdd;
		}

	public:
		uint32 m_entitiesCount;
		Niflect::TArray<CArchecomponentBuffer> m_vecArchecomponentBuffer;
	};
}