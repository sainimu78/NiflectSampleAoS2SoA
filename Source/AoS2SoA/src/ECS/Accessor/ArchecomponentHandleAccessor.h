#pragma once
#include "ECS/ArchecomponentHandle.h"
#include "OOP/Accessor/Vector3Accessor.h"

namespace ECS
{
	using namespace RwTree;

	template <typename THandle>
	class TArchecomponentHandleAccessor : public Niflect::CAccessor
	{
		typedef typename THandle::TArchetype TArchetype;
	protected:
		virtual bool SaveImpl(const InstanceType* base, CRwNode* rw) const override
		{
			auto& instance = *static_cast<const TArchetype*>(base);
			RwOverloads::SaveToRwNode(instance, rw);
			return true;
		}
		virtual bool LoadImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto& instance = *static_cast<TArchetype*>(base);
			ASSERT(rw->IsValue());
			RwOverloads::LoadFromRwNode(rw, instance);
			return true;
		}
	};
}