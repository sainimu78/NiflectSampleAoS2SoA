#pragma once
#include "Niflect/NiflectType.h"
#include "ECS/Node.h"
#include "ECS/AosSoaBinder.h"

namespace ECS
{
	template <typename TField, typename TType>
	void FindAndBindFieldInComponent(CNode* node, TField TType::* fieldAddr, CAosSoaBinder& binder)
	{
		for (auto& it0 : node->m_vecComponent)
		{
			if (it0->GetType() == Niflect::StaticGetType<TType>())
			{
				binder.Add<TField, TType>(static_cast<TType*>(it0.Get()), fieldAddr);
				break;
			}
		}
	}
	class CSystem
	{
	public:
		CSystem()
			: m_entitiesCount(0)
		{
		}
		void InitEntityComponent(const Niflect::TArray<CSharedNode>& vecNode, uint32 entityStartIdx, const CAosSoaBinder& binder)
		{
			//ASSERT(m_vecArchetype.size() > 0);
			auto entitiesCount = static_cast<uint32>(vecNode.size());
			ASSERT(entitiesCount > 0);
			//CAosSoaBinder binder;
			//for (auto& it0 : vecNode)
			//{
			//	for (auto& it1 : it0->m_vecComponent)
			//	{
			//		for (auto& it2 : vecOopField)
			//		{
			//			for (auto& it3 : it1->GetType()->GetFields())
			//			{
			//				if (it2->GetName() == it3.GetName())
			//				{

			//				}
			//			}
			//		}
			//	}
			//	binder.Convert()

			//}
			m_componentBuffer.Alloc(entityStartIdx, entitiesCount, binder);
			m_entitiesCount = entitiesCount;
			//m_entityIdxToComponentBufferIdx.resize(entitiesCount);
			//for (uint32 idx = 0; idx < entitiesCount; ++idx)
			//	m_entityIdxToComponentBufferIdx[idx] = idx;
		}

		//Niflect::TArray<Niflect::CNiflectType*> m_vecArchetype;
		//Niflect::TArray<uint32> m_entityIdxToComponentBufferIdx;
		CComponentBuffer m_componentBuffer;
		uint32 m_entitiesCount;
	};
}