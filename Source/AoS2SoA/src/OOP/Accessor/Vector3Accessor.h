#pragma once
#include "Niflect/Component/RwTree/RwAccessor.h"
#include "OOP/Vector3.h"

namespace OOP
{
	using namespace RwTree;

	void SaveCVector3ToRwNode(const CVector3& vec, CRwNode* rw);
	void LoadCVector3FromRwNode(const CRwNode* rw, CVector3& vec);

	class CVector3Accessor : public Niflect::CRwAccessor
	{
	protected:
		virtual bool SaveImpl(const InstanceType* base, CRwNode* rw) const override;
		virtual bool LoadImpl(InstanceType* base, const CRwNode* rw) const override;
	};
}

namespace RwOverloads
{
	using namespace RwTree;

	static void SaveToRwNode(const OOP::CVector3& vec, CRwNode* rw)
	{
		OOP::SaveCVector3ToRwNode(vec, rw);
	}
	static void LoadFromRwNode(const CRwNode* rw, OOP::CVector3& vec)
	{
		OOP::LoadCVector3FromRwNode(rw, vec);
	}
}