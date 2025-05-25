#pragma once
#include "OOP/Accessor/Vector3Accessor.h"
#include "ECS/Vector3.h"

namespace ECS
{
	using namespace RwTree;

	void SaveCVector3ToRwNode(const CVector3& vec, CRwNode* rw);
	void LoadCVector3FromRwNode(const CRwNode* rw, CVector3& vec);

	class CVector3Accessor : public Niflect::CNiflectAccessor
	{
	protected:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override;
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override;
	};
}