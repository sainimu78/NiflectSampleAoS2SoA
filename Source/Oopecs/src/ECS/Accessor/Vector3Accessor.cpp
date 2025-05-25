#include "ECS/Accessor/Vector3Accessor.h"
#include "Niflect/Util/StringUtil.h"

namespace ECS
{
	void SaveCVector3ToRwNode(const CVector3& vec, CRwNode* rw)
	{
		OOP::SaveCVector3ToRwNode(vec.Get(), rw);
	}
	void LoadCVector3FromRwNode(const CRwNode* rw, CVector3& vec)
	{
		OOP::LoadCVector3FromRwNode(rw, vec.GetMutable());
	}

	bool CVector3Accessor::SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const
	{
		auto& instance = *static_cast<const CVector3*>(base);
		SaveCVector3ToRwNode(instance, rw);//表明可任意编解码
		return true;
	}
	bool CVector3Accessor::LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const
	{
		auto& instance = *static_cast<CVector3*>(base);
		ASSERT(rw->IsValue());
		LoadCVector3FromRwNode(rw, instance);
		return true;
	}
}