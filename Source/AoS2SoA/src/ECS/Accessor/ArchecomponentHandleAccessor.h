#pragma once
#include "ECS/ArchecomponentHandle.h"
#include "OOP/Accessor/Vector3Accessor.h"

namespace ECS
{
	using namespace RwTree;

	template <typename THandle>
	class TArchecomponentHandleAccessor : public CRwAccessor
	{
		typedef typename THandle::TArchetype TArchetype;
	protected:
#ifdef NIFLECTDEV_DECOUPLING_ACCESSOR_AND_TYPE
		virtual bool SaveImpl(Niflect::CNiflectType* type, const Niflect::InstanceType* base, CRwNode* rw) const override
#else
		virtual bool SaveImpl(const InstanceType* base, CRwNode* rw) const override
#endif
		{
			auto& instance = *static_cast<const TArchetype*>(base);
			RwOverloads::SaveToRwNode(instance, rw);
			return true;
		}
#ifdef NIFLECTDEV_DECOUPLING_ACCESSOR_AND_TYPE
		virtual bool LoadImpl(Niflect::CNiflectType* type, Niflect::InstanceType* base, const CRwNode* rw) const override
#else
		virtual bool LoadImpl(InstanceType* base, const CRwNode* rw) const override
#endif
		{
			auto& instance = *static_cast<TArchetype*>(base);
			ASSERT(rw->IsValue());
			RwOverloads::LoadFromRwNode(rw, instance);
			return true;
		}
	};
}