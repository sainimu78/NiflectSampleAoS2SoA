#pragma once
#include "ECS/ComponentHandle.h"

namespace ECS
{
	using namespace RwTree;

	template <typename THandle>
	class TComponentHandleAccessor : public Niflect::CNiflectAccessor
	{
	protected:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			auto& instance = *static_cast<const THandle::TArchetype*>(base);
			RwOverloads::SaveToRwNode(instance, rw);
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto& instance = *static_cast<THandle::TArchetype*>(base);
			ASSERT(rw->IsValue());
			RwOverloads::LoadFromRwNode(rw, instance);
			return true;
		}
	};
}